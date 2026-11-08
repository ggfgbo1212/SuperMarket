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


void setup_scr_SettingsPage1(lv_ui *ui)
{
	//Write codes SettingsPage1
	ui->SettingsPage1 = lv_obj_create(NULL);
	lv_obj_set_size(ui->SettingsPage1, 240, 320);
	lv_obj_set_scrollbar_mode(ui->SettingsPage1, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->SettingsPage1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage1, lv_color_hex(0xebeef5), LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes SettingsPage1_settings_p4_info_image
	ui->SettingsPage1_settings_p4_info_image = lv_img_create(ui->SettingsPage1);
	lv_obj_add_flag(ui->SettingsPage1_settings_p4_info_image, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->SettingsPage1_settings_p4_info_image, &_trademark_alpha_34x34);
	lv_img_set_pivot(ui->SettingsPage1_settings_p4_info_image, 50,50);
	lv_img_set_angle(ui->SettingsPage1_settings_p4_info_image, 0);
	lv_obj_set_pos(ui->SettingsPage1_settings_p4_info_image, 201, 2);
	lv_obj_set_size(ui->SettingsPage1_settings_p4_info_image, 34, 34);
	lv_obj_set_scrollbar_mode(ui->SettingsPage1_settings_p4_info_image, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage1_settings_p4_info_image, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->SettingsPage1_settings_p4_info_image, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes SettingsPage1_settings_info_device_model_panel
	ui->SettingsPage1_settings_info_device_model_panel = lv_obj_create(ui->SettingsPage1);
	lv_obj_set_pos(ui->SettingsPage1_settings_info_device_model_panel, 12, 56);
	lv_obj_set_size(ui->SettingsPage1_settings_info_device_model_panel, 217, 55);
	lv_obj_set_scrollbar_mode(ui->SettingsPage1_settings_info_device_model_panel, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage1_settings_info_device_model_panel, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->SettingsPage1_settings_info_device_model_panel, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->SettingsPage1_settings_info_device_model_panel, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->SettingsPage1_settings_info_device_model_panel, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage1_settings_info_device_model_panel, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->SettingsPage1_settings_info_device_model_panel, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage1_settings_info_device_model_panel, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->SettingsPage1_settings_info_device_model_panel, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->SettingsPage1_settings_info_device_model_panel, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->SettingsPage1_settings_info_device_model_panel, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->SettingsPage1_settings_info_device_model_panel, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage1_settings_info_device_model_panel, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes SettingsPage1_settings_info_device_model_label
	ui->SettingsPage1_settings_info_device_model_label = lv_label_create(ui->SettingsPage1_settings_info_device_model_panel);
	lv_label_set_text(ui->SettingsPage1_settings_info_device_model_label, "基本设置\n");
	lv_label_set_long_mode(ui->SettingsPage1_settings_info_device_model_label, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->SettingsPage1_settings_info_device_model_label, 41, 14);
	lv_obj_set_size(ui->SettingsPage1_settings_info_device_model_label, 130, 30);
	lv_obj_set_scrollbar_mode(ui->SettingsPage1_settings_info_device_model_label, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage1_settings_info_device_model_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->SettingsPage1_settings_info_device_model_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage1_settings_info_device_model_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->SettingsPage1_settings_info_device_model_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->SettingsPage1_settings_info_device_model_label, &lv_font_simsun_23, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->SettingsPage1_settings_info_device_model_label, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->SettingsPage1_settings_info_device_model_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->SettingsPage1_settings_info_device_model_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->SettingsPage1_settings_info_device_model_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->SettingsPage1_settings_info_device_model_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->SettingsPage1_settings_info_device_model_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->SettingsPage1_settings_info_device_model_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->SettingsPage1_settings_info_device_model_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage1_settings_info_device_model_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes SettingsPage1_cont_1
	ui->SettingsPage1_cont_1 = lv_obj_create(ui->SettingsPage1);
	lv_obj_set_pos(ui->SettingsPage1_cont_1, 12, 119);
	lv_obj_set_size(ui->SettingsPage1_cont_1, 217, 55);
	lv_obj_set_scrollbar_mode(ui->SettingsPage1_cont_1, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage1_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->SettingsPage1_cont_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->SettingsPage1_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->SettingsPage1_cont_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage1_cont_1, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->SettingsPage1_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage1_cont_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->SettingsPage1_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->SettingsPage1_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->SettingsPage1_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->SettingsPage1_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage1_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes SettingsPage1_label_1
	ui->SettingsPage1_label_1 = lv_label_create(ui->SettingsPage1_cont_1);
	lv_label_set_text(ui->SettingsPage1_label_1, "无线设置\n");
	lv_label_set_long_mode(ui->SettingsPage1_label_1, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->SettingsPage1_label_1, 41, 14);
	lv_obj_set_size(ui->SettingsPage1_label_1, 130, 30);
	lv_obj_set_scrollbar_mode(ui->SettingsPage1_label_1, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage1_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->SettingsPage1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->SettingsPage1_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->SettingsPage1_label_1, &lv_font_simsun_23, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->SettingsPage1_label_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->SettingsPage1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->SettingsPage1_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->SettingsPage1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->SettingsPage1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->SettingsPage1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->SettingsPage1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->SettingsPage1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes SettingsPage1_cont_2
	ui->SettingsPage1_cont_2 = lv_obj_create(ui->SettingsPage1);
	lv_obj_set_pos(ui->SettingsPage1_cont_2, 12, 183);
	lv_obj_set_size(ui->SettingsPage1_cont_2, 217, 55);
	lv_obj_set_scrollbar_mode(ui->SettingsPage1_cont_2, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage1_cont_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->SettingsPage1_cont_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->SettingsPage1_cont_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->SettingsPage1_cont_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage1_cont_2, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->SettingsPage1_cont_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage1_cont_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->SettingsPage1_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->SettingsPage1_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->SettingsPage1_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->SettingsPage1_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage1_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes SettingsPage1_label_2
	ui->SettingsPage1_label_2 = lv_label_create(ui->SettingsPage1_cont_2);
	lv_label_set_text(ui->SettingsPage1_label_2, "远程管理\n\n");
	lv_label_set_long_mode(ui->SettingsPage1_label_2, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->SettingsPage1_label_2, 41, 14);
	lv_obj_set_size(ui->SettingsPage1_label_2, 130, 30);
	lv_obj_set_scrollbar_mode(ui->SettingsPage1_label_2, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage1_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->SettingsPage1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->SettingsPage1_label_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->SettingsPage1_label_2, &lv_font_simsun_23, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->SettingsPage1_label_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->SettingsPage1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->SettingsPage1_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->SettingsPage1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->SettingsPage1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->SettingsPage1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->SettingsPage1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->SettingsPage1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes SettingsPage1_cont_3
	ui->SettingsPage1_cont_3 = lv_obj_create(ui->SettingsPage1);
	lv_obj_set_pos(ui->SettingsPage1_cont_3, 12, 248);
	lv_obj_set_size(ui->SettingsPage1_cont_3, 217, 55);
	lv_obj_set_scrollbar_mode(ui->SettingsPage1_cont_3, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage1_cont_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->SettingsPage1_cont_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->SettingsPage1_cont_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->SettingsPage1_cont_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage1_cont_3, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->SettingsPage1_cont_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->SettingsPage1_cont_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->SettingsPage1_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->SettingsPage1_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->SettingsPage1_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->SettingsPage1_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage1_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes SettingsPage1_label_4
	ui->SettingsPage1_label_4 = lv_label_create(ui->SettingsPage1_cont_3);
	lv_label_set_text(ui->SettingsPage1_label_4, "管理系统\n\n\n");
	lv_label_set_long_mode(ui->SettingsPage1_label_4, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->SettingsPage1_label_4, 41, 13);
	lv_obj_set_size(ui->SettingsPage1_label_4, 130, 30);
	lv_obj_set_scrollbar_mode(ui->SettingsPage1_label_4, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage1_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->SettingsPage1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->SettingsPage1_label_4, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->SettingsPage1_label_4, &lv_font_simsun_23, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->SettingsPage1_label_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->SettingsPage1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->SettingsPage1_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->SettingsPage1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->SettingsPage1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->SettingsPage1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->SettingsPage1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->SettingsPage1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage1_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes SettingsPage1_label_3
	ui->SettingsPage1_label_3 = lv_label_create(ui->SettingsPage1);
	lv_label_set_text(ui->SettingsPage1_label_3, "无人自助超市\n管理系统\n");
	lv_label_set_long_mode(ui->SettingsPage1_label_3, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->SettingsPage1_label_3, 42, 2);
	lv_obj_set_size(ui->SettingsPage1_label_3, 156, 47);
	lv_obj_set_scrollbar_mode(ui->SettingsPage1_label_3, LV_SCROLLBAR_MODE_OFF);

	//Write style for SettingsPage1_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->SettingsPage1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->SettingsPage1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->SettingsPage1_label_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->SettingsPage1_label_3, &lv_font_simsun_23, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->SettingsPage1_label_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->SettingsPage1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->SettingsPage1_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->SettingsPage1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->SettingsPage1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->SettingsPage1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->SettingsPage1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->SettingsPage1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->SettingsPage1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Update current screen layout.
	lv_obj_update_layout(ui->SettingsPage1);

	/* 事件绑定（action 层，GUI Guider 重新导出后需重加）：
	 * 「基本设置」卡片 → SettingsPage2Basic；「无线设置」cont_1 → SettingsPage2Wireless1；
	 * 「远程管理」cont_2 → SettingsPage3Remote1（cont_3「管理系统」暂无对应页面） */
	lv_obj_add_flag(ui->SettingsPage1_settings_info_device_model_panel, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_event_cb(ui->SettingsPage1_settings_info_device_model_panel,
	                    settings_p1_enter_basic, LV_EVENT_CLICKED, NULL);
	lv_obj_add_flag(ui->SettingsPage1_cont_1, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_event_cb(ui->SettingsPage1_cont_1,
	                    settings_p1_enter_wireless1, LV_EVENT_CLICKED, NULL);
	lv_obj_add_flag(ui->SettingsPage1_cont_2, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_event_cb(ui->SettingsPage1_cont_2,
	                    settings_p1_enter_remote1, LV_EVENT_CLICKED, NULL);
}
