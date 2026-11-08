/*
 * lvgl_action.h
 * LVGL 控件事件回调模块（应用层 UI 行为独立成层，与 GUI Guider 生成的
 * events_init.c 分开，避免 GUI Guider 重新生成代码时被覆盖）。
 *
 * - 全局 UI 对象 guider_ui 由 lvgl_action.c 定义，本头文件 extern 声明，
 *   任何模块（如 gpio_test.c 的 DispTask）包含本头文件即可访问。
 * - 各控件事件回调函数在本文件声明、在 lvgl_action.c 实现；
 *   "绑定"这一行写在对应屏幕的 setup_scr_XXX() 函数里。
 */

#ifndef LVGL_ACTION_H
#define LVGL_ACTION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gui_guider.h"   /* lv_ui 结构体类型 + 各屏控件句柄定义 */
#include "dev_wifi.h"     /* WiFiApInfo：WiFi 列表显示 */

/* 全局 UI 对象：外部模块用这个变量访问各屏控件（如 guider_ui.HomeScreen） */
extern lv_ui guider_ui;

/* ==================== 控件事件回调函数 ==================== */
/* 在对应的 setup_scr_XXX() 末尾绑定到控件，例如：
 *   lv_obj_add_event_cb(ui->HomeScreen_title_label,
 *                       home_screen_title_click, LV_EVENT_CLICKED, NULL);
 */

/* SettingsPage1「基本设置」卡片点击 → 进入 SettingsPage2Basic */
void settings_p1_enter_basic(lv_event_t *e);

/* SettingsPage1「无线设置」cont_1 点击 → 进入 SettingsPage2Wireless1 */
void settings_p1_enter_wireless1(lv_event_t *e);

/* SettingsPage1「远程管理」cont_2 点击 → 进入 SettingsPage3Remote1 */
void settings_p1_enter_remote1(lv_event_t *e);

/* 各子页面右上角「<-返回」btn_1 点击 → 返回上一个界面（导航历史栈出栈） */
void settings_back(lv_event_t *e);

/* SettingsPage2Basic 亮度滑条 → 调节屏幕背光（0~100） */
void settings_p2_slider_brightness(lv_event_t *e);

/* SettingsPage2Basic 按键声音开关 → 使能/关闭触摸提示音 */
void settings_p2_switch_beep(lv_event_t *e);

/* ==================== SettingsPage2Wireless1：WiFi 列表 ==================== */
/* list_1 中预建 15 个容器（每容器一个连接复选框 + 一个 SSID label），默认全部隐藏 */
#define WIFI_LIST_ROW_NUM   15

/* 创建 15 个列表容器（幂等，首次进入无线设置页时调用） */
void wifi_list_ui_create(void);

/* 显示整个列表并填充前 count 个热点名称，其余行隐藏（count=0 即空列表） */
void wifi_list_ui_show(const WiFiApInfo *aps, int count);

/* 隐藏整个列表（开关关闭时调用，参考 ESP_Epoch 工程行为） */
void wifi_list_ui_hide_all(void);

/* ==================== SettingsPage2Wireless2：WiFi 密码输入 + 连接 ==================== */
/* 页面 tick（UI 线程）：轮询后台连接结果。
 * 成功 → 自动返回 SettingsPage2Wireless1；失败 → ta_1 输入框内显示“密码错误” */
void wireless2_ui_tick(void);

/* 离开密码页时调用（UI 线程）：清除在途标记、丢弃残留连接结果，
 * 避免“连接中按返回离开”后列表行点击被永久屏蔽 */
void wireless2_ui_deactivate(void);

#ifdef __cplusplus
}
#endif

#endif /* LVGL_ACTION_H */
