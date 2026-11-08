#ifndef __WIFI_SCAN_H__
#define __WIFI_SCAN_H__

#include "dev_wifi.h"

/* ==================== WiFi 列表刷新（后台扫描 + 队列投递） ====================
 *
 * 参考 ESP_Epoch 工程（_app_wifi_station 的 app_wifi_list_refreshtask +
 * gui_refresh_queue + wireless1 tick 的 xQueueReceive）：
 *   - 后台任务按周期扫描，扫描结果放进长度为 1 的队列（最新结果覆盖旧结果）；
 *   - UI 线程在自己的页面 tick 里非阻塞取队列，取到就刷新 list_1；
 *   - 开关关闭时任务不扫描，UI 隐藏整个列表。
 *
 * 线程约定：
 *   - WifiScanTask（后台任务）负责扫描与投递；
 *   - WifiScan_UpdateActive / WifiScan_Fetch 只在 UI 线程（DispTask）调用；
 *   - 刷新条件（页面可见 && 开关打开）由 UI 线程判定后通过 UpdateActive 传入。
 *     参考工程是在 WiFi 任务里直接判断 lv_scr_act()/开关变量，但本工程 LVGL
 *     未做跨线程锁，lv_* 只能在 UI 线程调用，因此把条件判断放在 UI 侧，
 *     语义与参考工程一致。
 */

#define WIFI_SCAN_MAX_AP      15        /* 最多保存/显示的热点数 */
#define WIFI_CONNECT_PWD_MAX  64        /* 密码最大长度（含结束符；WPA2 PSK 最长 63） */

/* 一次扫描的完整结果：队列元素（UI 侧收到后直接拿来刷新列表） */
typedef struct
{
    unsigned char count;                          /* 有效热点数 0..WIFI_SCAN_MAX_AP */
    WiFiApInfo    list[WIFI_SCAN_MAX_AP];         /* 热点信息（按扫描顺序） */
} WiFiScanResult;

/* 创建扫描任务 + 结果队列（app_test 中调用一次；任务内完成 WiFi 芯片初始化） */
void WifiScan_Init(void);

/* UI 线程每轮调用：设置刷新条件状态
 * 返回 1-刚使能（调用方应立刻显示上次结果） -1-刚关闭 0-无变化 */
int WifiScan_UpdateActive(unsigned char active);

/* UI 线程非阻塞取结果：队列中有新结果返回 1 并拷贝到 out，否则返回 0 */
unsigned char WifiScan_Fetch(WiFiScanResult *out);

/* ==================== WiFi 连接（与扫描共用 AT 通道，由同一任务串行执行） ====================
 *
 * 参考 ESP_Epoch 工程的 wireless_wifi_connect_task：
 *   - UI 线程把「SSID + 密码」投递给后台任务，立即返回，不阻塞界面；
 *   - 后台任务先等正在进行的 WSCAN 结束，再执行 AT+WJAP 连接（最长约 5~10 秒）；
 *   - 结果通过长度为 1 的结果队列回传，UI 在页面 tick 里非阻塞取出。
 */

/* UI 线程调用：请求连接指定热点。
 * 返回 1-请求已入队 0-任务未就绪/正在连接/参数非法（调用方按失败处理） */
int WifiScan_Connect(const char *ssid, const char *password);

/* UI 线程非阻塞取连接结果：有结果返回 1 并写入 *success（1-成功 0-失败），否则返回 0 */
unsigned char WifiScan_FetchConnectResult(unsigned char *success);

/* UI 线程读取当前已连接的热点名（连接成功时由后台任务记录，失败时清空）。
 * 返回 1-已连接并写入 out；0-当前未连接（out 置空串）。
 * WiFi 列表用它把已连接热点打勾并排到第一个（参考 ESP_Epoch 工程做法）。 */
unsigned char WifiScan_GetConnectedSsid(char *out, unsigned int size);

#endif /* __WIFI_SCAN_H__ */
