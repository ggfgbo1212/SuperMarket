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


void setup_scr_SettingsPage2Wireless2(lv_ui *ui)
{
	//Write codes SettingsPage2Wireless2
	ui->SettingsPage2Wireless2 = lv_obj_create(NULL);
	lv_obj_set_size(ui->SettingsPage2Wireless2, 240, 320);
	lv_obj_set_scrollbar_mode(ui->SettingsPage2Wireless2, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage2Wireless2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->SettingsPage2Wireless2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage2Wireless2, lv_color_hex(0xebeef5), LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes SettingsPage2Wireless2_settings_p4_checkupdate1_title
	ui->SettingsPage2Wireless2_settings_p4_checkupdate1_title = lv_label_create(ui->SettingsPage2Wireless2);
	lv_label_set_text(ui->SettingsPage2Wireless2_settings_p4_checkupdate1_title, "无线设置");
	lv_label_set_long_mode(ui->SettingsPage2Wireless2_settings_p4_checkupdate1_title, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->SettingsPage2Wireless2_settings_p4_checkupdate1_title, 6, 10);
	lv_obj_set_size(ui->SettingsPage2Wireless2_settings_p4_checkupdate1_title, 114, 28);
	lv_obj_set_scrollbar_mode(ui->SettingsPage2Wireless2_settings_p4_checkupdate1_title, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage2Wireless2_settings_p4_checkupdate1_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->SettingsPage2Wireless2_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage2Wireless2_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->SettingsPage2Wireless2_settings_p4_checkupdate1_title, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->SettingsPage2Wireless2_settings_p4_checkupdate1_title, &lv_font_simsun_23, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->SettingsPage2Wireless2_settings_p4_checkupdate1_title, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->SettingsPage2Wireless2_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->SettingsPage2Wireless2_settings_p4_checkupdate1_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->SettingsPage2Wireless2_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->SettingsPage2Wireless2_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->SettingsPage2Wireless2_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->SettingsPage2Wireless2_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->SettingsPage2Wireless2_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage2Wireless2_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes SettingsPage2Wireless2_ta_1
	ui->SettingsPage2Wireless2_ta_1 = lv_textarea_create(ui->SettingsPage2Wireless2);
	lv_textarea_set_text(ui->SettingsPage2Wireless2_ta_1, "请输入无线密码");
	lv_obj_set_pos(ui->SettingsPage2Wireless2_ta_1, 13, 51);
	lv_obj_set_size(ui->SettingsPage2Wireless2_ta_1, 214, 55);
	lv_obj_set_scrollbar_mode(ui->SettingsPage2Wireless2_ta_1, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage2Wireless2_ta_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->SettingsPage2Wireless2_ta_1, lv_color_hex(0xcdcdcd), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->SettingsPage2Wireless2_ta_1, &lv_font_simsun_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->SettingsPage2Wireless2_ta_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->SettingsPage2Wireless2_ta_1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->SettingsPage2Wireless2_ta_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage2Wireless2_ta_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->SettingsPage2Wireless2_ta_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->SettingsPage2Wireless2_ta_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->SettingsPage2Wireless2_ta_1, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage2Wireless2_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->SettingsPage2Wireless2_ta_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->SettingsPage2Wireless2_ta_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->SettingsPage2Wireless2_ta_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage2Wireless2_ta_1, 12, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for SettingsPage2Wireless2_ta_1, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->SettingsPage2Wireless2_ta_1, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage2Wireless2_ta_1, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage2Wireless2_ta_1, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//Write codes SettingsPage2Wireless2_btn_1
	ui->SettingsPage2Wireless2_btn_1 = lv_btn_create(ui->SettingsPage2Wireless2);
	ui->SettingsPage2Wireless2_btn_1_label = lv_label_create(ui->SettingsPage2Wireless2_btn_1);
	lv_label_set_text(ui->SettingsPage2Wireless2_btn_1_label, "<-返回");
	lv_label_set_long_mode(ui->SettingsPage2Wireless2_btn_1_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->SettingsPage2Wireless2_btn_1_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->SettingsPage2Wireless2_btn_1, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->SettingsPage2Wireless2_btn_1, 177, 3);
	lv_obj_set_size(ui->SettingsPage2Wireless2_btn_1, 58, 35);
	lv_obj_set_scrollbar_mode(ui->SettingsPage2Wireless2_btn_1, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage2Wireless2_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->SettingsPage2Wireless2_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage2Wireless2_btn_1, lv_color_hex(0xEBEEF5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->SettingsPage2Wireless2_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage2Wireless2_btn_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage2Wireless2_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->SettingsPage2Wireless2_btn_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->SettingsPage2Wireless2_btn_1, &lv_font_simsun_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->SettingsPage2Wireless2_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Update current screen layout.
	lv_obj_update_layout(ui->SettingsPage2Wireless2);

	/* 事件绑定（action 层，GUI Guider 重新导出后需重加）：右上角 <-返回 → 上一界面 */
	lv_obj_add_event_cb(ui->SettingsPage2Wireless2_btn_1,
	                    settings_back, LV_EVENT_CLICKED, NULL);
}
