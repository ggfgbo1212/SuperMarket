/*
* Copyright 2026 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"
#include "lvgl_action.h"   /* 控件事件回调（setup 末尾绑定本页控件） */


void setup_scr_SettingsPage2Basic(lv_ui *ui)
{
	//Write codes SettingsPage2Basic
	ui->SettingsPage2Basic = lv_obj_create(NULL);
	lv_obj_set_size(ui->SettingsPage2Basic, 240, 320);
	lv_obj_set_scrollbar_mode(ui->SettingsPage2Basic, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage2Basic, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->SettingsPage2Basic, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage2Basic, lv_color_hex(0xebeef5), LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes SettingsPage2Basic_settings_p4_checkupdate1_title
	ui->SettingsPage2Basic_settings_p4_checkupdate1_title = lv_label_create(ui->SettingsPage2Basic);
	lv_label_set_text(ui->SettingsPage2Basic_settings_p4_checkupdate1_title, "基本设置");
	lv_label_set_long_mode(ui->SettingsPage2Basic_settings_p4_checkupdate1_title, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->SettingsPage2Basic_settings_p4_checkupdate1_title, 6, 10);
	lv_obj_set_size(ui->SettingsPage2Basic_settings_p4_checkupdate1_title, 114, 28);
	lv_obj_set_scrollbar_mode(ui->SettingsPage2Basic_settings_p4_checkupdate1_title, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage2Basic_settings_p4_checkupdate1_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->SettingsPage2Basic_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage2Basic_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->SettingsPage2Basic_settings_p4_checkupdate1_title, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->SettingsPage2Basic_settings_p4_checkupdate1_title, &lv_font_simsun_23, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->SettingsPage2Basic_settings_p4_checkupdate1_title, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->SettingsPage2Basic_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->SettingsPage2Basic_settings_p4_checkupdate1_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->SettingsPage2Basic_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->SettingsPage2Basic_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->SettingsPage2Basic_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->SettingsPage2Basic_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->SettingsPage2Basic_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage2Basic_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes SettingsPage2Basic_cont_1
	ui->SettingsPage2Basic_cont_1 = lv_obj_create(ui->SettingsPage2Basic);
	lv_obj_set_pos(ui->SettingsPage2Basic_cont_1, 12, 50);
	lv_obj_set_size(ui->SettingsPage2Basic_cont_1, 217, 75);
	lv_obj_set_scrollbar_mode(ui->SettingsPage2Basic_cont_1, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage2Basic_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->SettingsPage2Basic_cont_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->SettingsPage2Basic_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->SettingsPage2Basic_cont_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage2Basic_cont_1, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->SettingsPage2Basic_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage2Basic_cont_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->SettingsPage2Basic_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->SettingsPage2Basic_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->SettingsPage2Basic_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->SettingsPage2Basic_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage2Basic_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes SettingsPage2Basic_label_3
	ui->SettingsPage2Basic_label_3 = lv_label_create(ui->SettingsPage2Basic_cont_1);
	lv_label_set_text(ui->SettingsPage2Basic_label_3, "屏幕亮度\n");
	lv_label_set_long_mode(ui->SettingsPage2Basic_label_3, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->SettingsPage2Basic_label_3, 6, 7);
	lv_obj_set_size(ui->SettingsPage2Basic_label_3, 94, 19);
	lv_obj_set_scrollbar_mode(ui->SettingsPage2Basic_label_3, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage2Basic_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->SettingsPage2Basic_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage2Basic_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->SettingsPage2Basic_label_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->SettingsPage2Basic_label_3, &lv_font_simsun_21, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->SettingsPage2Basic_label_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->SettingsPage2Basic_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->SettingsPage2Basic_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->SettingsPage2Basic_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->SettingsPage2Basic_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->SettingsPage2Basic_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->SettingsPage2Basic_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->SettingsPage2Basic_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage2Basic_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes SettingsPage2Basic_slider_1
	ui->SettingsPage2Basic_slider_1 = lv_slider_create(ui->SettingsPage2Basic_cont_1);
	lv_slider_set_range(ui->SettingsPage2Basic_slider_1, 0,100);
	lv_slider_set_value(ui->SettingsPage2Basic_slider_1, 25, false);
	lv_obj_set_pos(ui->SettingsPage2Basic_slider_1, 8, 38);
	lv_obj_set_size(ui->SettingsPage2Basic_slider_1, 194, 13);
	lv_obj_set_scrollbar_mode(ui->SettingsPage2Basic_slider_1, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage2Basic_slider_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->SettingsPage2Basic_slider_1, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage2Basic_slider_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage2Basic_slider_1, 50, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_outline_width(ui->SettingsPage2Basic_slider_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage2Basic_slider_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for SettingsPage2Basic_slider_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->SettingsPage2Basic_slider_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage2Basic_slider_1, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage2Basic_slider_1, 50, LV_PART_INDICATOR|LV_STATE_DEFAULT);

	//Write style for SettingsPage2Basic_slider_1, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->SettingsPage2Basic_slider_1, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage2Basic_slider_1, lv_color_hex(0x2195f6), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage2Basic_slider_1, 50, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes SettingsPage2Basic_cont_2
	ui->SettingsPage2Basic_cont_2 = lv_obj_create(ui->SettingsPage2Basic);
	lv_obj_set_pos(ui->SettingsPage2Basic_cont_2, 12, 140);
	lv_obj_set_size(ui->SettingsPage2Basic_cont_2, 217, 75);
	lv_obj_set_scrollbar_mode(ui->SettingsPage2Basic_cont_2, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage2Basic_cont_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->SettingsPage2Basic_cont_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->SettingsPage2Basic_cont_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->SettingsPage2Basic_cont_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage2Basic_cont_2, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->SettingsPage2Basic_cont_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage2Basic_cont_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->SettingsPage2Basic_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->SettingsPage2Basic_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->SettingsPage2Basic_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->SettingsPage2Basic_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage2Basic_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes SettingsPage2Basic_label_4
	ui->SettingsPage2Basic_label_4 = lv_label_create(ui->SettingsPage2Basic_cont_2);
	lv_label_set_text(ui->SettingsPage2Basic_label_4, "按键声音\n");
	lv_label_set_long_mode(ui->SettingsPage2Basic_label_4, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->SettingsPage2Basic_label_4, 4, 26);
	lv_obj_set_size(ui->SettingsPage2Basic_label_4, 94, 19);
	lv_obj_set_scrollbar_mode(ui->SettingsPage2Basic_label_4, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage2Basic_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->SettingsPage2Basic_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage2Basic_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->SettingsPage2Basic_label_4, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->SettingsPage2Basic_label_4, &lv_font_simsun_21, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->SettingsPage2Basic_label_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->SettingsPage2Basic_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->SettingsPage2Basic_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->SettingsPage2Basic_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->SettingsPage2Basic_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->SettingsPage2Basic_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->SettingsPage2Basic_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->SettingsPage2Basic_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage2Basic_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes SettingsPage2Basic_sw_1
	ui->SettingsPage2Basic_sw_1 = lv_switch_create(ui->SettingsPage2Basic_cont_2);
	lv_obj_set_pos(ui->SettingsPage2Basic_sw_1, 142, 24);
	lv_obj_set_size(ui->SettingsPage2Basic_sw_1, 51, 24);
	lv_obj_set_scrollbar_mode(ui->SettingsPage2Basic_sw_1, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage2Basic_sw_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->SettingsPage2Basic_sw_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage2Basic_sw_1, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->SettingsPage2Basic_sw_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage2Basic_sw_1, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage2Basic_sw_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for SettingsPage2Basic_sw_1, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
	lv_obj_set_style_bg_opa(ui->SettingsPage2Basic_sw_1, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_color(ui->SettingsPage2Basic_sw_1, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_border_width(ui->SettingsPage2Basic_sw_1, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

	//Write style for SettingsPage2Basic_sw_1, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->SettingsPage2Basic_sw_1, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage2Basic_sw_1, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->SettingsPage2Basic_sw_1, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage2Basic_sw_1, 100, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes SettingsPage2Basic_btn_1
	ui->SettingsPage2Basic_btn_1 = lv_btn_create(ui->SettingsPage2Basic);
	ui->SettingsPage2Basic_btn_1_label = lv_label_create(ui->SettingsPage2Basic_btn_1);
	lv_label_set_text(ui->SettingsPage2Basic_btn_1_label, "<-返回");
	lv_label_set_long_mode(ui->SettingsPage2Basic_btn_1_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->SettingsPage2Basic_btn_1_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->SettingsPage2Basic_btn_1, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->SettingsPage2Basic_btn_1, 177, 3);
	lv_obj_set_size(ui->SettingsPage2Basic_btn_1, 58, 35);
	lv_obj_set_scrollbar_mode(ui->SettingsPage2Basic_btn_1, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage2Basic_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->SettingsPage2Basic_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage2Basic_btn_1, lv_color_hex(0xEBEEF5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->SettingsPage2Basic_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage2Basic_btn_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage2Basic_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->SettingsPage2Basic_btn_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->SettingsPage2Basic_btn_1, &lv_font_simsun_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->SettingsPage2Basic_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Update current screen layout.
	lv_obj_update_layout(ui->SettingsPage2Basic);

	/* 事件绑定（action 层，GUI Guider 重新导出后需重加）：
	 * 亮度滑条 → 背光 PWM；按键声音开关 → 触摸提示音；右上角 <-返回 → 上一界面 */
	lv_obj_add_event_cb(ui->SettingsPage2Basic_slider_1,
	                    settings_p2_slider_brightness, LV_EVENT_VALUE_CHANGED, NULL);
	lv_obj_add_event_cb(ui->SettingsPage2Basic_sw_1,
	                    settings_p2_switch_beep, LV_EVENT_VALUE_CHANGED, NULL);
	lv_obj_add_event_cb(ui->SettingsPage2Basic_btn_1,
	                    settings_back, LV_EVENT_CLICKED, NULL);
}
