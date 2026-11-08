/*
* Copyright 2026 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
  
	lv_obj_t *SettingsPage1;
	bool SettingsPage1_del;
	lv_obj_t *SettingsPage1_settings_p4_info_image;
	lv_obj_t *SettingsPage1_settings_info_device_model_panel;
	lv_obj_t *SettingsPage1_settings_info_device_model_label;
	lv_obj_t *SettingsPage1_cont_1;
	lv_obj_t *SettingsPage1_label_1;
	lv_obj_t *SettingsPage1_cont_2;
	lv_obj_t *SettingsPage1_label_2;
	lv_obj_t *SettingsPage1_cont_3;
	lv_obj_t *SettingsPage1_label_4;
	lv_obj_t *SettingsPage1_label_3;
	lv_obj_t *SettingsPage2Basic;
	bool SettingsPage2Basic_del;
	lv_obj_t *SettingsPage2Basic_settings_p4_checkupdate1_title;
	lv_obj_t *SettingsPage2Basic_cont_1;
	lv_obj_t *SettingsPage2Basic_label_3;
	lv_obj_t *SettingsPage2Basic_slider_1;
	lv_obj_t *SettingsPage2Basic_cont_2;
	lv_obj_t *SettingsPage2Basic_label_4;
	lv_obj_t *SettingsPage2Basic_sw_1;
	lv_obj_t *SettingsPage2Basic_btn_1;
	lv_obj_t *SettingsPage2Basic_btn_1_label;
	lv_obj_t *SettingsPage2Wireless1;
	bool SettingsPage2Wireless1_del;
	lv_obj_t *SettingsPage2Wireless1_settings_p4_checkupdate1_title;
	lv_obj_t *SettingsPage2Wireless1_cont_2;
	lv_obj_t *SettingsPage2Wireless1_label_5;
	lv_obj_t *SettingsPage2Wireless1_sw_2;
	lv_obj_t *SettingsPage2Wireless1_list_1;
	lv_obj_t *SettingsPage2Wireless1_btn_1;
	lv_obj_t *SettingsPage2Wireless1_btn_1_label;
	lv_obj_t *SettingsPage2Wireless2;
	bool SettingsPage2Wireless2_del;
	lv_obj_t *SettingsPage2Wireless2_settings_p4_checkupdate1_title;
	lv_obj_t *SettingsPage2Wireless2_ta_1;
	lv_obj_t *SettingsPage2Wireless2_btn_1;
	lv_obj_t *SettingsPage2Wireless2_btn_1_label;
	lv_obj_t *SettingsPage3Remote1;
	bool SettingsPage3Remote1_del;
	lv_obj_t *SettingsPage3Remote1_settings_p4_checkupdate1_title;
	lv_obj_t *SettingsPage3Remote1_ta_1;
	lv_obj_t *SettingsPage3Remote1_btnm_1;
	lv_obj_t *SettingsPage3Remote1_btn_1;
	lv_obj_t *SettingsPage3Remote1_btn_1_label;
	lv_obj_t *SettingsPage2_Basic;
	bool SettingsPage2_Basic_del;
	lv_obj_t *SettingsPage2_Basic_settings_p4_checkupdate1_title;
	lv_obj_t *SettingsPage2_Basic_cont_1;
	lv_obj_t *SettingsPage2_Basic_label_6;
	lv_obj_t *SettingsPage2_Basic_slider_2;
	lv_obj_t *SettingsPage2_Basic_cont_2;
	lv_obj_t *SettingsPage2_Basic_label_5;
	lv_obj_t *SettingsPage2_Basic_sw_2;
	lv_obj_t *SettingsPage2_Basic_btn_1;
	lv_obj_t *SettingsPage2_Basic_btn_1_label;
}lv_ui;

void ui_init_style(lv_style_t * style);
void init_scr_del_flag(lv_ui *ui);
void setup_ui(lv_ui *ui);
extern lv_ui guider_ui;

void setup_scr_SettingsPage1(lv_ui *ui);
void setup_scr_SettingsPage2Basic(lv_ui *ui);
void setup_scr_SettingsPage2Wireless1(lv_ui *ui);
void setup_scr_SettingsPage2Wireless2(lv_ui *ui);
void setup_scr_SettingsPage3Remote1(lv_ui *ui);
void setup_scr_SettingsPage2_Basic(lv_ui *ui);
LV_IMG_DECLARE(_trademark_alpha_34x34);

LV_FONT_DECLARE(lv_font_simsun_23)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_simsun_21)
LV_FONT_DECLARE(lv_font_simsun_18)
LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_simsun_20)
LV_FONT_DECLARE(lv_font_Abel_regular_20)


#ifdef __cplusplus
}
#endif
#endif
