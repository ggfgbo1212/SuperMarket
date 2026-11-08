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


void setup_scr_SettingsPage3Remote1(lv_ui *ui)
{
	//Write codes SettingsPage3Remote1
	ui->SettingsPage3Remote1 = lv_obj_create(NULL);
	lv_obj_set_size(ui->SettingsPage3Remote1, 240, 320);
	lv_obj_set_scrollbar_mode(ui->SettingsPage3Remote1, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage3Remote1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->SettingsPage3Remote1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage3Remote1, lv_color_hex(0xebeef5), LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes SettingsPage3Remote1_settings_p4_checkupdate1_title
	ui->SettingsPage3Remote1_settings_p4_checkupdate1_title = lv_label_create(ui->SettingsPage3Remote1);
	lv_label_set_text(ui->SettingsPage3Remote1_settings_p4_checkupdate1_title, "远程管理");
	lv_label_set_long_mode(ui->SettingsPage3Remote1_settings_p4_checkupdate1_title, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->SettingsPage3Remote1_settings_p4_checkupdate1_title, 6, 10);
	lv_obj_set_size(ui->SettingsPage3Remote1_settings_p4_checkupdate1_title, 114, 28);
	lv_obj_set_scrollbar_mode(ui->SettingsPage3Remote1_settings_p4_checkupdate1_title, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage3Remote1_settings_p4_checkupdate1_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->SettingsPage3Remote1_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage3Remote1_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->SettingsPage3Remote1_settings_p4_checkupdate1_title, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->SettingsPage3Remote1_settings_p4_checkupdate1_title, &lv_font_simsun_23, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->SettingsPage3Remote1_settings_p4_checkupdate1_title, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->SettingsPage3Remote1_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->SettingsPage3Remote1_settings_p4_checkupdate1_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->SettingsPage3Remote1_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->SettingsPage3Remote1_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->SettingsPage3Remote1_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->SettingsPage3Remote1_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->SettingsPage3Remote1_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage3Remote1_settings_p4_checkupdate1_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes SettingsPage3Remote1_ta_1
	ui->SettingsPage3Remote1_ta_1 = lv_textarea_create(ui->SettingsPage3Remote1);
	lv_textarea_set_text(ui->SettingsPage3Remote1_ta_1, "点击输入远端IP");
	lv_obj_set_pos(ui->SettingsPage3Remote1_ta_1, 13, 49);
	lv_obj_set_size(ui->SettingsPage3Remote1_ta_1, 214, 55);
	lv_obj_set_scrollbar_mode(ui->SettingsPage3Remote1_ta_1, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage3Remote1_ta_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->SettingsPage3Remote1_ta_1, lv_color_hex(0xcdcdcd), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->SettingsPage3Remote1_ta_1, &lv_font_simsun_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->SettingsPage3Remote1_ta_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->SettingsPage3Remote1_ta_1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->SettingsPage3Remote1_ta_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage3Remote1_ta_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->SettingsPage3Remote1_ta_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->SettingsPage3Remote1_ta_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->SettingsPage3Remote1_ta_1, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage3Remote1_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->SettingsPage3Remote1_ta_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->SettingsPage3Remote1_ta_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->SettingsPage3Remote1_ta_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage3Remote1_ta_1, 12, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for SettingsPage3Remote1_ta_1, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->SettingsPage3Remote1_ta_1, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage3Remote1_ta_1, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage3Remote1_ta_1, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//Write codes SettingsPage3Remote1_btnm_1
	ui->SettingsPage3Remote1_btnm_1 = lv_btnmatrix_create(ui->SettingsPage3Remote1);
	static const char *SettingsPage3Remote1_btnm_1_text_map[] = {"1", "2", "3", "\n", "4", "5", "6", "\n", "7", "8", "9", "\n", ".", "0", "enter", "",};
	lv_btnmatrix_set_map(ui->SettingsPage3Remote1_btnm_1, SettingsPage3Remote1_btnm_1_text_map);
	lv_obj_set_pos(ui->SettingsPage3Remote1_btnm_1, 26, 104);
	lv_obj_set_size(ui->SettingsPage3Remote1_btnm_1, 189, 209);
	lv_obj_set_scrollbar_mode(ui->SettingsPage3Remote1_btnm_1, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage3Remote1_btnm_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->SettingsPage3Remote1_btnm_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->SettingsPage3Remote1_btnm_1, 16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->SettingsPage3Remote1_btnm_1, 16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->SettingsPage3Remote1_btnm_1, 16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->SettingsPage3Remote1_btnm_1, 16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_row(ui->SettingsPage3Remote1_btnm_1, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_column(ui->SettingsPage3Remote1_btnm_1, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage3Remote1_btnm_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->SettingsPage3Remote1_btnm_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage3Remote1_btnm_1, lv_color_hex(0xEBEEF5), LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for SettingsPage3Remote1_btnm_1, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->SettingsPage3Remote1_btnm_1, 0, LV_PART_ITEMS|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->SettingsPage3Remote1_btnm_1, lv_color_hex(0x0c2b49), LV_PART_ITEMS|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->SettingsPage3Remote1_btnm_1, &lv_font_Abel_regular_20, LV_PART_ITEMS|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage3Remote1_btnm_1, 4, LV_PART_ITEMS|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->SettingsPage3Remote1_btnm_1, 255, LV_PART_ITEMS|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage3Remote1_btnm_1, lv_color_hex(0x93e3ff), LV_PART_ITEMS|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage3Remote1_btnm_1, 0, LV_PART_ITEMS|LV_STATE_DEFAULT);

	//Write style for SettingsPage3Remote1_btnm_1, Part: LV_PART_ITEMS, State: LV_STATE_PRESSED.
	lv_obj_set_style_border_width(ui->SettingsPage3Remote1_btnm_1, 0, LV_PART_ITEMS|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->SettingsPage3Remote1_btnm_1, lv_color_hex(0xffffff), LV_PART_ITEMS|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->SettingsPage3Remote1_btnm_1, &lv_font_Abel_regular_20, LV_PART_ITEMS|LV_STATE_PRESSED);
	lv_obj_set_style_radius(ui->SettingsPage3Remote1_btnm_1, 4, LV_PART_ITEMS|LV_STATE_PRESSED);
	lv_obj_set_style_bg_opa(ui->SettingsPage3Remote1_btnm_1, 255, LV_PART_ITEMS|LV_STATE_PRESSED);
	lv_obj_set_style_bg_color(ui->SettingsPage3Remote1_btnm_1, lv_color_hex(0x93e3ff), LV_PART_ITEMS|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->SettingsPage3Remote1_btnm_1, 2, LV_PART_ITEMS|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->SettingsPage3Remote1_btnm_1, lv_color_hex(0x93e3ff), LV_PART_ITEMS|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->SettingsPage3Remote1_btnm_1, 255, LV_PART_ITEMS|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->SettingsPage3Remote1_btnm_1, 2, LV_PART_ITEMS|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->SettingsPage3Remote1_btnm_1, 0, LV_PART_ITEMS|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->SettingsPage3Remote1_btnm_1, 1, LV_PART_ITEMS|LV_STATE_PRESSED);

	//Write codes SettingsPage3Remote1_btn_1
	ui->SettingsPage3Remote1_btn_1 = lv_btn_create(ui->SettingsPage3Remote1);
	ui->SettingsPage3Remote1_btn_1_label = lv_label_create(ui->SettingsPage3Remote1_btn_1);
	lv_label_set_text(ui->SettingsPage3Remote1_btn_1_label, "<-返回");
	lv_label_set_long_mode(ui->SettingsPage3Remote1_btn_1_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->SettingsPage3Remote1_btn_1_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->SettingsPage3Remote1_btn_1, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->SettingsPage3Remote1_btn_1, 177, 3);
	lv_obj_set_size(ui->SettingsPage3Remote1_btn_1, 58, 35);
	lv_obj_set_scrollbar_mode(ui->SettingsPage3Remote1_btn_1, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage3Remote1_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->SettingsPage3Remote1_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage3Remote1_btn_1, lv_color_hex(0xEBEEF5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->SettingsPage3Remote1_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage3Remote1_btn_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage3Remote1_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->SettingsPage3Remote1_btn_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->SettingsPage3Remote1_btn_1, &lv_font_simsun_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->SettingsPage3Remote1_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Update current screen layout.
	lv_obj_update_layout(ui->SettingsPage3Remote1);

	/* 事件绑定（action 层，GUI Guider 重新导出后需重加）：右上角 <-返回 → 上一界面 */
	lv_obj_add_event_cb(ui->SettingsPage3Remote1_btn_1,
	                    settings_back, LV_EVENT_CLICKED, NULL);
}
