#include "dev_gpio.h"
#include "dev_uart.h"
#include "dev_iic.h"
#include "dev_spi.h"
#include "dev_wifi.h"    /* WiFiBtDevice：WIFIScan 扫描周围热点 */
#include <string.h>
#include "printf.h"
#include "errno.h"
#include "drv_flash.h"
#include "ring_buffer.h"
#include "dev_st7789.h"
#include "xpt2046.h"

/* ==================== LVGL ==================== */
#include "lvgl.h"
#include "gui_guider.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl_action.h"   /* guider_ui 全局对象 + 控件事件绑定（事件回调在 action 层） */
#include "tickscreen.h"    /* 每个页面一个 tick 循环函数（ui_tick 分发） */

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/* ==================== LVGL 界面 ==================== */

/* guider_ui 全局对象定义在 6_Middlewares/lvgl/action/lvgl_action.c，经 lvgl_action.h extern 使用 */

static TaskHandle_t gDispTaskHandle = NULL;
static TaskHandle_t gWifiScanTaskHandle = NULL;

/* ==================== WiFi 列表刷新任务 ==================== */

#define WIFI_SCAN_INTERVAL_MS   10000   /* 每10秒刷新一次列表 */
#define WIFI_SCAN_MAX_AP        20      /* 一次最多保存的热点数 */
#define WIFI_SCAN_TIMEOUT_MS    10000   /* 扫描超时（芯片扫描本身需数秒） */

/* 每10秒扫描一次周围WiFi热点，串口打印扫描结果 */
static void WifiScanTask(void *argument)
{
    (void)argument;

    WiFiBtDevice *pWifi = GetWIFIBTDevice();
    static WiFiApInfo apList[WIFI_SCAN_MAX_AP];

    /* 芯片初始化（含AT测试/RST/关回显/AP_STA模式/主动接收） */
    if(NULL == pWifi || ESUCCESS != pWifi->Init(pWifi))
    {
        debugprintf("WiFi init failed, scan task stop\r\n");
        vTaskDelete(NULL);
        return;
    }
    debugprintf("WiFi init ok, scan every %dms\r\n", WIFI_SCAN_INTERVAL_MS);

    while(1)
    {
        int n = pWifi->WIFIScan(pWifi, apList, WIFI_SCAN_MAX_AP, WIFI_SCAN_TIMEOUT_MS);

        if(n >= 0)
        {
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
        else
        {
            debugprintf("WSCAN failed, ret=%d\r\n", n);
        }

        vTaskDelay(pdMS_TO_TICKS(WIFI_SCAN_INTERVAL_MS));
    }
}

/* LVGL 节拍：FreeRTOS 软件定时器每 1ms 回调一次（configTICK_RATE_HZ = 1000） */
static void gui_timer_cb(TimerHandle_t handle)
{
    (void)handle;
    lv_tick_inc(1);
    /* 蜂鸣器提示音逻辑已并入 lv_port_indev_template.c（read_cb 轮询内自动停响），此处不再轮询 */
}

/* lv_timer_handler() 返回值的上下限：用它做动态休眠，减少空转（参考 ESP-Epoch lvgl_port_task） */
#define LVGL_TASK_MIN_DELAY_MS   5
#define LVGL_TASK_MAX_DELAY_MS   100

/* 显示设备任务：初始化 LVGL + ST7789 驱动，周期调用 ui_tick + lv_timer_handler() 驱动渲染 */
static void DispTask(void *argument)
{
    (void)argument;

    lv_init();               /* LVGL 核心初始化 */
    lv_port_disp_init();     /* 注册显示驱动（ST7789 rotation 0 竖屏 240×320，单缓冲区） */
    lv_port_indev_init();    /* 注册触摸输入设备（XPT2046 软件 SPI） */

    tickscreen_init();   /* 创建页间通信资源（信号量/队列等） */

    /* 上电默认显示第一个界面——SettingsPage1（设置主页） */
    init_scr_del_flag(&guider_ui);                       /* 初始化各屏删除标志（setup_ui 里的那步，别漏） */
    if(NULL == guider_ui.SettingsPage1)
        setup_scr_SettingsPage1(&guider_ui);
    lv_scr_load(guider_ui.SettingsPage1);

    /* 各屏事件在对应 setup_scr_XXX() 末尾绑定（见 generated/ 下文件），此处无需集中绑定 */

    /* 软件定时器驱动 LVGL 节拍（1ms） */
    TimerHandle_t guiTimer = xTimerCreate("LVGL Tick", pdMS_TO_TICKS(1), pdTRUE, NULL, gui_timer_cb);
    if(NULL != guiTimer)
    {
        xTimerStart(guiTimer, 0);
    }

    uint32_t task_delay_ms = LVGL_TASK_MAX_DELAY_MS;
    while(1)
    {
        ui_tick();               /* 当前页面周期逻辑（每页一个 tick 函数） */

        task_delay_ms = lv_timer_handler();  /* LVGL 渲染与事件处理，返回下次需处理的时间 */

        if(task_delay_ms > LVGL_TASK_MAX_DELAY_MS)
            task_delay_ms = LVGL_TASK_MAX_DELAY_MS;
        else if(task_delay_ms < LVGL_TASK_MIN_DELAY_MS)
            task_delay_ms = LVGL_TASK_MIN_DELAY_MS;

        vTaskDelay(pdMS_TO_TICKS(task_delay_ms));
    }
}

void app_test()
{
    GPIO_DevRegis();//全局GPIO设备注册
    UART_DevRegis();//全局串口设备注册
    IIC_DevRegis();//全局IIC设备注册
    SPI_DevRegis();//全局SPI设备注册

    /* 创建 LVGL 显示任务（软件定时器驱动节拍，任务内做 lv_init + lv_port_disp_init）
     * 蜂鸣器（PE8）不再单独驱动：drv_gpio 已注册 BUZZER 对象，触摸提示音逻辑在
     * lv_port_indev_init() 内初始化并轮询，见 lv_port_indev_template.c */
    if(pdPASS != xTaskCreate(DispTask, "DisplayTask", 1024, NULL, 4, &gDispTaskHandle))
    {
        debugprintf("DisplayTask create failed\r\n");
    }

    /* 创建 WiFi 扫描任务：每10秒扫描一次周围热点并串口打印 */
    if(pdPASS != xTaskCreate(WifiScanTask, "WifiScanTask", 512, NULL, 3, &gWifiScanTaskHandle))
    {
        debugprintf("WifiScanTask create failed\r\n");
    }

}
