/*
 * tickscreen.c
 * 每页 tick 循环函数 + 分发。
 *
 * 参考 EEZ Studio 生成的 screens.c 结构：
 *   - tick_screen_funcs[] 函数指针表按页面 ID 排列
 *   - ui_tick() 用 lv_scr_act() 判断当前页并分发
 * 与参考工程的差异：不维护 currentScreen 状态变量，
 * 用 lv_scr_act() 动态判断当前页，切页后自动生效。
 *
 * 注意：本文件所有函数都在 DispTask（UI 线程）里执行，可直接调 LVGL API。
 */

#include "tickscreen.h"
#include "lvgl.h"
#include "gui_guider.h"
#include "lvgl_action.h"   /* wifi_list_ui_*：WiFi 列表控件操作 */
#include "wifi_scan.h"     /* WifiScan_*：后台扫描控制与结果读取（队列） */

void tickscreen_init(void)
{
    /* 目前无页间信号量/队列需要创建；新增页面时在这里建 */
}

/* ==================== 各页面周期循环函数（在此填写页面逻辑） ==================== */

void tick_screen_SettingsPage1(void)
{
    /* 设置主页（上电首屏）：页面周期逻辑待填写 */
}

void tick_screen_SettingsPage2Basic(void)
{
    /* 基础设置页（滑条/开关）：页面周期逻辑待填写 */
}

/* 无线设置页：WiFi 列表刷新（参考 ESP_Epoch 工程的联动方式）。
 * 扫描任务每 10 秒扫描一次，结果通过长度 1 的队列投递；
 * 本页 tick 用 WifiScan_Fetch 非阻塞取结果并刷新 list_1。
 * 刷新条件 = 当前页面 == 本页（由 ui_tick 分发保证）&& 开关 sw_2 打开；
 * UI 线程缓存最近一次结果，开关重新打开/再次进入页面时立即恢复显示。 */
static WiFiScanResult s_wifi_result;          /* UI 线程私有：最近一次取到的扫描结果 */
static unsigned char s_wifi_result_valid = 0;

void tick_screen_SettingsPage2Wireless1(void)
{
    WiFiScanResult result;
    int tr;

    if(NULL == guider_ui.SettingsPage2Wireless1_sw_2)   return;   /* 页面尚未创建 */

    unsigned char on = lv_obj_has_state(guider_ui.SettingsPage2Wireless1_sw_2,
                                        LV_STATE_CHECKED) ? 1 : 0;

    tr = WifiScan_UpdateActive(on);

    if(!on)             /* 开关关闭：隐藏整个列表（参考工程同做法），后台停止扫描 */
    {
        if(tr < 0)   wifi_list_ui_hide_all();
        return;
    }

    if(tr > 0)          /* 开关刚打开：立即恢复上次刷新出的列表（参考工程推回缓存列表的做法） */
    {
        if(s_wifi_result_valid)
            wifi_list_ui_show(s_wifi_result.list, s_wifi_result.count);
        else
            wifi_list_ui_show(NULL, 0);   /* 尚无结果：先显示空列表，等待首轮扫描 */
    }

    if(WifiScan_Fetch(&result))   /* 队列有新结果：刷新显示（未满的行容器隐藏） */
    {
        s_wifi_result = result;
        s_wifi_result_valid = 1;
        wifi_list_ui_show(s_wifi_result.list, s_wifi_result.count);
    }
}

void tick_screen_SettingsPage2Wireless2(void)
{
    /* 无线设置页2（密码输入）：轮询后台连接结果。
     * 成功 → 自动返回 Wireless1；失败 → ta_1 内显示“密码错误”（见 lvgl_action.c） */
    wireless2_ui_tick();
}

void tick_screen_SettingsPage3Remote1(void)
{
    /* 遥控器页（文本框/按钮矩阵）：页面周期逻辑待填写 */
}

void tick_screen_SettingsPage2_Basic(void)
{
    /* 基本设置页副本（GUI Guider 工程里的重复页，导航未接入）：待填写 */
}

/* ==================== 分发 ==================== */

typedef void (*tick_screen_func_t)(void);

/* 函数指针表：与 screen_id_t 一一对应（顺序不能乱） */
static tick_screen_func_t tick_screen_funcs[SCREEN_NUM] = {
    tick_screen_SettingsPage1,
    tick_screen_SettingsPage2Basic,
    tick_screen_SettingsPage2Wireless1,
    tick_screen_SettingsPage2Wireless2,
    tick_screen_SettingsPage3Remote1,
    tick_screen_SettingsPage2_Basic,
};

void tick_screen_by_id(screen_id_t id)
{
    if(id < SCREEN_NUM)
        tick_screen_funcs[id]();
}

void ui_tick(void)
{
    lv_obj_t *act = lv_scr_act();
    if(NULL == act)
        return;   /* 屏幕未创建/未加载，跳过（避免 NULL==NULL 误匹配） */

    /* 当前不在密码页：清除其“连接在途”状态并丢弃残留结果
     * （连接中按返回离开后，否则列表行点击会一直被拒绝） */
    if(act != guider_ui.SettingsPage2Wireless2)
        wireless2_ui_deactivate();

    if(act == guider_ui.SettingsPage2Wireless1)
    {
        tick_screen_by_id(SCREEN_SETTINGSPAGE2WIRELESS1);
        return;
    }

    /* 不在无线设置页：不满足 WiFi 刷新条件（页面/开关状态由该页 tick 维护） */
    WifiScan_UpdateActive(0);

    if(act == guider_ui.SettingsPage1)
        tick_screen_by_id(SCREEN_SETTINGSPAGE1);
    else if(act == guider_ui.SettingsPage2Basic)
        tick_screen_by_id(SCREEN_SETTINGSPAGE2BASIC);
    else if(act == guider_ui.SettingsPage2Wireless2)
        tick_screen_by_id(SCREEN_SETTINGSPAGE2WIRELESS2);
    else if(act == guider_ui.SettingsPage3Remote1)
        tick_screen_by_id(SCREEN_SETTINGSPAGE3REMOTE1);
    else if(act == guider_ui.SettingsPage2_Basic)
        tick_screen_by_id(SCREEN_SETTINGSPAGE2__BASIC);
}
