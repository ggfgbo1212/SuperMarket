/*
 * tickscreen.h
 * 每个页面一个 tick 循环函数（模仿 EEZ Studio 的 tick_screen 机制）。
 *
 * 工作机制：
 *   DispTask 主循环每轮调用 ui_tick()：
 *     1. 用 lv_scr_act() 判断当前显示的是哪个页面（不维护状态变量，切页自动生效）
 *     2. 分发到对应页面的 tick_screen_xxx() 周期函数
 *   各页面的周期逻辑（刷新状态、更新控件）写在对应的 tick_screen_xxx() 里。
 *
 * 注意：本模块所有函数都在 DispTask（UI 线程）里执行，可直接调 LVGL API。
 */

#ifndef TICKSCREEN_H
#define TICKSCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gui_guider.h"   /* lv_ui 类型 + guider_ui（各屏控件句柄） */

/* 页面 ID：与 tick_screen_funcs[] 数组一一对应，顺序对齐 guider_ui 字段 */
typedef enum {
    SCREEN_SETTINGSPAGE1 = 0,
    SCREEN_SETTINGSPAGE2BASIC,
    SCREEN_SETTINGSPAGE2WIRELESS1,
    SCREEN_SETTINGSPAGE2WIRELESS2,
    SCREEN_SETTINGSPAGE3REMOTE1,
    SCREEN_SETTINGSPAGE2__BASIC,   /* GUI Guider 工程里的重复页（导航未接入） */
    SCREEN_NUM   /* 页面总数 = tick_screen_funcs[] 长度 */
} screen_id_t;

/* ==================== 每页一个周期循环函数 ==================== */
/* 写各页面周期逻辑的地方（在 tickscreen.c 里实现） */
void tick_screen_SettingsPage1(void);
void tick_screen_SettingsPage2Basic(void);
void tick_screen_SettingsPage2Wireless1(void);
void tick_screen_SettingsPage2Wireless2(void);
void tick_screen_SettingsPage3Remote1(void);
void tick_screen_SettingsPage2_Basic(void);

/* 按页面 ID 分发到对应 tick 函数（ui_tick 内部调用，外部一般不用） */
void tick_screen_by_id(screen_id_t id);

/* 每轮 UI 循环调用：tick 当前显示的页面 */
void ui_tick(void);

/* 页间通信资源（信号量/队列）在此创建：新增页面需要跨线程通信时，
 * 业务线程只 Give/Send，UI tick 只 Take/Receive，
 * 所有 lv_* 调用始终保持在 UI 线程（DispTask），LVGL 非线程安全。 */
void tickscreen_init(void);

#ifdef __cplusplus
}
#endif

#endif /* TICKSCREEN_H */
