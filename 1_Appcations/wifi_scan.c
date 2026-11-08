#include "wifi_scan.h"
#include "main.h"       /* HAL_GetTick */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "printf.h"
#include "errno.h"
#include <string.h>

/* ==================== WiFi 列表刷新 ====================
 *
 * 参考 ESP_Epoch 工程的 app_wifi_list_refreshtask：
 *   后台任务按条件扫描（页面可见 && 开关打开），结果通过长度 1 的队列投递给 UI；
 *   UI 线程在 SettingsPage2Wireless1 的 tick 里非阻塞取队列并刷新 list_1
 *   （见 tickscreen.c）。队列只保留最新一份结果，UI 来不及时覆盖旧结果。
 */

#define WIFI_SCAN_PERIOD_MS   10000     /* 刷新周期（条件满足期间） */
#define WIFI_SCAN_TIMEOUT_MS  10000     /* 单次扫描应答超时（芯片扫描本身需数秒） */

/* 连接命令：UI 线程 → 扫描任务（与 WSCAN 共用 AT 通道，必须串行） */
typedef struct
{
    char ssid[33];
    char password[WIFI_CONNECT_PWD_MAX];
} WiFiConnectCmd;

static WiFiBtDevice *s_wifi = NULL;
static TaskHandle_t s_task = NULL;
static QueueHandle_t s_queue = NULL;       /* 扫描任务 → UI 线程，长度 1，元素 WiFiScanResult */
static QueueHandle_t s_cmd_queue = NULL;   /* UI 线程 → 扫描任务，长度 1，元素 WiFiConnectCmd */
static QueueHandle_t s_res_queue = NULL;   /* 扫描任务 → UI 线程，长度 1，元素 unsigned char */

static volatile unsigned char s_active = 0;     /* 刷新条件：页面可见 && 开关打开 */
static volatile unsigned char s_ready = 0;      /* WiFi 芯片初始化完成（连接请求可受理） */
static volatile unsigned char s_connecting = 0; /* 已受理连接请求、结果尚未回传 */

/* 当前已连接的热点名：连接成功时由本任务写入，失败时清空。
 * 放在任务侧而不是 UI 侧，是为了即使用户在连接完成前离开密码页
 * （UI 会丢弃结果队列），列表页仍能正确显示“已连接并排第一”。
 * 任务写、UI 读，用临界区保护 33 字节的拷贝。 */
static char s_connected_ssid[33];

static void WifiScanSetConnectedSsid(const char *ssid)
{
    taskENTER_CRITICAL();
    memset(s_connected_ssid, 0, sizeof(s_connected_ssid));
    if(NULL != ssid)
    {
        strncpy(s_connected_ssid, ssid, sizeof(s_connected_ssid) - 1);
    }
    taskEXIT_CRITICAL();
}

/* 扫描任务：条件满足时每 WIFI_SCAN_PERIOD_MS 扫描一次，结果投递给 UI */
static void WifiScanTask(void *argument)
{
    (void)argument;

    static WiFiApInfo apList[WIFI_SCAN_MAX_AP];
    static WiFiScanResult result;
    unsigned int last_scan = 0;

    /* 芯片初始化（含AT测试/RST/关回显/AP_STA模式/主动接收） */
    s_wifi = GetWIFIBTDevice();
    if(NULL == s_wifi || ESUCCESS != s_wifi->Init(s_wifi))
    {
        debugprintf("WiFi init failed, scan task stop\r\n");
        vTaskDelete(NULL);
        return;
    }
    debugprintf("WiFi init ok, scan every %dms while condition met\r\n", WIFI_SCAN_PERIOD_MS);
    s_ready = 1;

    while(1)
    {
        WiFiConnectCmd cmd;

        vTaskDelay(pdMS_TO_TICKS(200));

        /* 连接命令优先处理：与 WSCAN 共用 pUart/pRxBuffer，同一任务内天然串行。
         * 若已有扫描在进行，命令会等本次扫描结束后再执行（最长约一个扫描周期）。 */
        if(NULL != s_cmd_queue && pdTRUE == xQueueReceive(s_cmd_queue, &cmd, 0))
        {
            unsigned char ok = 0;
            debugprintf("WiFi connecting: SSID=%s\r\n", cmd.ssid);
            int ret = s_wifi->WIFIConnect(s_wifi, cmd.ssid, cmd.password);
            /* 只认返回值；dev_status 可能反映的是芯片历史状态，不能作为本次连接结果 */
            ok = (ESUCCESS == ret) ? 1 : 0;
            /* 成功记下热点名；失败时旧连接已被本次 WJAP 断开，清空（与 dev_status 同步） */
            WifiScanSetConnectedSsid(ok ? cmd.ssid : NULL);
            debugprintf("WiFi connect %s, ret=%d\r\n", ok ? "OK" : "FAIL", ret);
            xQueueOverwrite(s_res_queue, &ok);
            s_connecting = 0;
            last_scan = 0;         /* 回到列表页后立即重新扫描 */
            continue;
        }

        if(!s_active)              /* 条件不满足：不扫描；复位计时，下次使能立即扫 */
        {
            last_scan = 0;
            continue;
        }

        if(0 != last_scan && (HAL_GetTick() - last_scan) < WIFI_SCAN_PERIOD_MS)
            continue;

        last_scan = HAL_GetTick();
        int n = s_wifi->WIFIScan(s_wifi, apList, WIFI_SCAN_MAX_AP, WIFI_SCAN_TIMEOUT_MS);
        last_scan = HAL_GetTick();   /* 从扫描结束重新计时，避免扫描耗时≥周期时背靠背连续扫描 */

        if(!s_active)   continue;  /* 扫描期间条件失效（离开页面/关闭开关）：丢弃结果 */

        if(n < 0)
        {
            debugprintf("WSCAN failed, ret=%d\r\n", n);
            continue;
        }

        /* 投递结果给 UI 线程（队列长度 1：覆盖旧结果，永远保留最新一份） */
        result.count = (unsigned char)n;
        memcpy(result.list, apList, (unsigned int)n * sizeof(WiFiApInfo));
        xQueueOverwrite(s_queue, &result);

        /* 串口打印 */
        debugprintf("\r\n===== WSCAN: %d AP(s) =====\r\n", n);
        for(int i = 0; i < n; i++)
        {
            debugprintf("[%02d] SSID:%-20s CH:%-3d SEC:%-10s RSSI:%4d BSSID:%s\r\n",
                        i,
                        apList[i].ssid,
                        apList[i].channel,
                        apList[i].security,
                        apList[i].rssi,
                        apList[i].bssid);
        }
        debugprintf("==========================\r\n");
    }
}

void WifiScan_Init(void)
{
    if(NULL != s_task)   return;

    if(NULL == s_queue)
    {
        s_queue = xQueueCreate(1, sizeof(WiFiScanResult));
        if(NULL == s_queue)
        {
            debugprintf("WiFi refresh queue create failed\r\n");
            return;
        }
    }

    /* 连接命令/结果队列（长度 1：UI 侧保证一次只有一个连接在途） */
    if(NULL == s_cmd_queue)
        s_cmd_queue = xQueueCreate(1, sizeof(WiFiConnectCmd));
    if(NULL == s_res_queue)
        s_res_queue = xQueueCreate(1, sizeof(unsigned char));
    if(NULL == s_cmd_queue || NULL == s_res_queue)
    {
        debugprintf("WiFi connect queue create failed\r\n");
        return;
    }

    /* 优先级必须低于 configTIMER_TASK_PRIORITY（当前为 2）！
     * LVGL 节拍由 1ms 软件定时器 lv_tick_inc 提供，而软件定时器运行在 timer daemon 任务中；
     * 扫描驱动用 HAL_Delay 忙等轮询串口（不阻塞），若本任务优先级更高，扫描期间（数秒~10秒）
     * timer daemon 得不到调度 → lv_tick 停止 → 触摸/刷新/滚动动画全部停摆，表现为界面卡死。
     * 取 1：低于 timer daemon(2) 和 UI 任务(4)，扫描期间二者均可正常抢占。 */
    if(pdPASS != xTaskCreate(WifiScanTask, "WifiScanTask", 512, NULL, 1, &s_task))
    {
        debugprintf("WifiScanTask create failed\r\n");
        s_task = NULL;
    }
}

int WifiScan_UpdateActive(unsigned char active)
{
    active = active ? 1 : 0;
    if(active == s_active)   return 0;

    s_active = active;
    return active ? 1 : -1;
}

unsigned char WifiScan_Fetch(WiFiScanResult *out)
{
    if(NULL == out || NULL == s_queue)   return 0;

    return (pdTRUE == xQueueReceive(s_queue, out, 0)) ? 1 : 0;
}

int WifiScan_Connect(const char *ssid, const char *password)
{
    WiFiConnectCmd cmd;
    size_t ssid_len;
    size_t pwd_len;

    if(!s_ready || NULL == s_cmd_queue || NULL == ssid || NULL == password)   return 0;
    if(s_connecting)   return 0;              /* 一次只允许一个连接在途 */

    ssid_len = strlen(ssid);
    pwd_len  = strlen(password);
    if(0 == ssid_len || ssid_len >= sizeof(cmd.ssid))   return 0;
    if(pwd_len >= sizeof(cmd.password))                 return 0;

    memset(&cmd, 0, sizeof(cmd));
    memcpy(cmd.ssid, ssid, ssid_len);
    memcpy(cmd.password, password, pwd_len);

    s_connecting = 1;
    if(pdTRUE != xQueueSend(s_cmd_queue, &cmd, 0))
    {
        s_connecting = 0;
        return 0;
    }
    return 1;
}

unsigned char WifiScan_FetchConnectResult(unsigned char *success)
{
    unsigned char ok;

    if(NULL == success || NULL == s_res_queue)   return 0;
    if(pdTRUE != xQueueReceive(s_res_queue, &ok, 0))   return 0;

    *success = ok;
    return 1;
}

unsigned char WifiScan_GetConnectedSsid(char *out, unsigned int size)
{
    unsigned char has_connected;

    if(NULL == out || 0 == size)   return 0;

    taskENTER_CRITICAL();
    has_connected = ('\0' != s_connected_ssid[0]) ? 1 : 0;
    if(has_connected)
    {
        strncpy(out, s_connected_ssid, size - 1);
        out[size - 1] = '\0';
    }
    else
    {
        out[0] = '\0';
    }
    taskEXIT_CRITICAL();

    return has_connected;
}
