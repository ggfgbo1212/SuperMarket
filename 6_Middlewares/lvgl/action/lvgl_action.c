/*
 * lvgl_action.c
 * LVGL 控件事件回调模块：定义全局 UI 对象 guider_ui，集中存放各控件的事件回调函数。
 *
 * 职责：
 *   1. 定义 lv_ui guider_ui 全局变量（原定义在 1_Appcations/gpio_test.c）。
 *   2. 集中编写各屏幕控件的回调函数（GUI Guider 重新生成代码不会覆盖本文件）。
 *
 * 事件绑定策略（重要）：
 *   回调函数体写在本文件（action 层，GUI Guider 重新生成不覆盖），
 *   但"绑定"这一行写在对应屏幕的 setup_scr_XXX() 函数里
 *   （例如 setup_scr_HomeScreen.c 末尾调 lv_obj_add_event_cb）。
 *   这样每个屏幕 setup 时只绑定自己页面的控件，其他屏幕事件不会因懒创建而漏绑。
 *
 * 使用方式（见 gpio_test.c 的 DispTask）：
 *   init_scr_del_flag(&guider_ui);
 *   setup_scr_HomeScreen(&guider_ui);   // 各屏懒创建，默认页才 setup
 *   lv_scr_load(guider_ui.HomeScreen);
 *   回调函数在 setup_scr_xxx() 末尾绑定到对应控件。
 */

#include "lvgl_action.h"
#include "lvgl.h"
#include <string.h>                    /* memmove：导航历史栈满时移位 */
#include "dev_st7789.h"                /* lcd_set_brightness：背光 PWM 调光 */
#include "lv_port_indev_template.h"    /* lv_port_indev_beep_enable：触摸提示音开关 */

/* 全局 UI 对象：各屏控件句柄集中存放，外部模块通过 extern 访问 */
lv_ui guider_ui;

/* ==================== 页面导航：历史栈（支持「返回上一个界面」） ====================
 * 前进（卡片点击进入子页）用 nav_push_and_load()：把当前屏压栈再加载目标屏；
 * 子页右上角「<-返回」用 settings_back()：出栈加载上一屏。
 * 页面均为懒创建且从不销毁，栈里存 lv_obj_t* 一直有效。 */
#define NAV_STACK_MAX 8
static lv_obj_t *g_nav_stack[NAV_STACK_MAX];
static int8_t g_nav_top = -1;

static void nav_push_and_load(lv_obj_t *scr)
{
    if(NULL == scr || lv_scr_act() == scr)   return;
    if(g_nav_top + 1 >= NAV_STACK_MAX)       /* 栈满丢最旧一层，保证返回语义始终正确 */
    {
        memmove(g_nav_stack, g_nav_stack + 1, sizeof(g_nav_stack) - sizeof(g_nav_stack[0]));
        g_nav_top--;
    }
    g_nav_stack[++g_nav_top] = lv_scr_act();
    lv_scr_load(scr);
}

/* ==================== SettingsPage1 → 各子页 ==================== */

/* settings_p1_enter_basic：「基本设置」卡片点击 → 懒创建并跳转 SettingsPage2Basic。
 * 首次创建时同步默认状态：提示音关（sw 保持 GUI Guider 默认未勾选）、
 * 背光=滑条当前值；再次进入不重置，保留用户上次的设置。 */
void settings_p1_enter_basic(lv_event_t *e)
{
    (void)e;

    if(NULL == guider_ui.SettingsPage2Basic)
    {
        setup_scr_SettingsPage2Basic(&guider_ui);

        /* 默认状态：提示音关（需求：按键蜂鸣器默认关闭）、背光=滑条初值（GUI Guider 设的 25） */
        lv_port_indev_beep_enable(0);
        lcd_set_brightness((uint8_t)lv_slider_get_value(guider_ui.SettingsPage2Basic_slider_1));
    }

    nav_push_and_load(guider_ui.SettingsPage2Basic);
}

/* settings_p1_enter_wireless1：「无线设置」cont_1 点击 → 懒创建并跳转 SettingsPage2Wireless1 */
void settings_p1_enter_wireless1(lv_event_t *e)
{
    (void)e;

    if(NULL == guider_ui.SettingsPage2Wireless1)
        setup_scr_SettingsPage2Wireless1(&guider_ui);

    nav_push_and_load(guider_ui.SettingsPage2Wireless1);
}

/* settings_p1_enter_remote1：「远程管理」cont_2 点击 → 懒创建并跳转 SettingsPage3Remote1 */
void settings_p1_enter_remote1(lv_event_t *e)
{
    (void)e;

    if(NULL == guider_ui.SettingsPage3Remote1)
        setup_scr_SettingsPage3Remote1(&guider_ui);

    nav_push_and_load(guider_ui.SettingsPage3Remote1);
}

/* settings_back：各子页右上角「<-返回」→ 出栈加载上一个界面（栈空=已在首页，忽略） */
void settings_back(lv_event_t *e)
{
    (void)e;

    if(g_nav_top < 0)   return;
    lv_scr_load(g_nav_stack[g_nav_top--]);
}

/* ==================== SettingsPage2Basic 控件 ==================== */

/* settings_p2_slider_brightness：亮度滑条 → 背光 PWM 占空比（0=灭 ~ 100=全亮） */
void settings_p2_slider_brightness(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    lcd_set_brightness((uint8_t)lv_slider_get_value(slider));
}

/* settings_p2_switch_beep：按键声音开关 → 触摸提示音使能（勾选=响） */
void settings_p2_switch_beep(lv_event_t *e)
{
    lv_obj_t *sw = lv_event_get_target(e);
    lv_port_indev_beep_enable(lv_obj_has_state(sw, LV_STATE_CHECKED));
}
