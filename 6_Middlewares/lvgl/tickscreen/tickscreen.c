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

void tick_screen_SettingsPage2Wireless1(void)
{
    /* 无线设置页1（开关/列表）：页面周期逻辑待填写 */
}

void tick_screen_SettingsPage2Wireless2(void)
{
    /* 无线设置页2（文本框）：页面周期逻辑待填写 */
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

    if(act == guider_ui.SettingsPage1)
        tick_screen_by_id(SCREEN_SETTINGSPAGE1);
    else if(act == guider_ui.SettingsPage2Basic)
        tick_screen_by_id(SCREEN_SETTINGSPAGE2BASIC);
    else if(act == guider_ui.SettingsPage2Wireless1)
        tick_screen_by_id(SCREEN_SETTINGSPAGE2WIRELESS1);
    else if(act == guider_ui.SettingsPage2Wireless2)
        tick_screen_by_id(SCREEN_SETTINGSPAGE2WIRELESS2);
    else if(act == guider_ui.SettingsPage3Remote1)
        tick_screen_by_id(SCREEN_SETTINGSPAGE3REMOTE1);
    else if(act == guider_ui.SettingsPage2_Basic)
        tick_screen_by_id(SCREEN_SETTINGSPAGE2__BASIC);
}
