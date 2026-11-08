/**
 * @file lv_port_indev_templ.h
 *
 */

/*Copy this file as "lv_port_indev.h" and set this value to "1" to enable content*/
#if 1

#ifndef LV_PORT_INDEV_TEMPL_H
#define LV_PORT_INDEV_TEMPL_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
/* 初始化触摸输入设备驱动（XPT2046，软件 SPI，注册到 LVGL indev） */
void lv_port_indev_init(void);

/* 触摸提示音使能开关（蜂鸣器=drv_gpio 注册的 BUZZER 对象，逻辑在本 .c 内） */
void lv_port_indev_beep_enable(uint8_t en);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_PORT_INDEV_TEMPL_H*/

#endif /*Disable/Enable content*/
