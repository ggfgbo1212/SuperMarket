/**
 * @file lv_port_indev_templ.c
 * XPT2046 电阻触摸接入 LVGL（软件 SPI）
 *
 * 流程：lv_port_indev_init() 注册一个 LV_INDEV_TYPE_POINTER 输入设备，
 * LVGL 每 LV_INDEV_DEF_READ_PERIOD（30ms）在 lv_timer_handler() 里自动
 * 调用 touchpad_read_cb() 轮询触摸，命中检测由 LVGL 内部完成并向控件发事件。
 */

/*Copy this file as "lv_port_indev.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_indev_template.h"
#include <stdbool.h>

#include "xpt2046.h"
#include "errno.h"
#include "dev_gpio.h"     /* GPIODev_Find：蜂鸣器走 drv_gpio 注册的 BUZZER 对象 */

/*********************
 *      DEFINES
 *********************/
/* 屏幕分辨率（与 lv_port_disp_template.c 保持一致：竖屏 240×320） */
#define MY_INDEV_HOR_RES  240
#define MY_INDEV_VER_RES  320

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void touchpad_init(void);

static void touchpad_read_cb(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);

static void touch_beep_init(void);
static void touch_beep_start(void);
static void touch_beep_poll(void);

/**********************
 *  STATIC VARIABLES
 **********************/
static XPT2046Device *pTouch = NULL;

/* ==================== 触摸提示音（蜂鸣器 PE8，低电平触发） ====================
 * 蜂鸣器只是一个 GPIO，对象注册在 drv_gpio.c（名字 "BUZZER"），不单独建驱动文件。
 * 本文件持有的是"触摸反馈策略"：按下沿响一声（非阻塞）、到时自动停、使能开关。
 * 引脚配置也在这里做（CubeMX 未配置 PE8）。 */
#define BEEP_DURATION_MS  80    /* 响一声时长 */

static GPIODevice *g_beep_dev = NULL;    /* drv_gpio 注册的 BUZZER 对象 */
static volatile uint8_t g_beep_en = 0;   /* 使能开关（默认关闭，SettingsPage2Basic 的 sw_1 控制） */
static volatile uint8_t g_beeping = 0;   /* 正在响（重入保护：连按只续一声） */
static volatile uint32_t g_beep_stop_tick = 0;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/* 使能/关闭触摸提示音（lvgl_action.c 里 SettingsPage2Basic 按键声音开关调用） */
void lv_port_indev_beep_enable(uint8_t en)
{
    g_beep_en = en ? 1 : 0;
}

void lv_port_indev_init(void)
{
    touchpad_init();
    touch_beep_init();   /* 蜂鸣器 PE8 引脚配置 + 查找 BUZZER 对象（需在 GPIO_DevRegis 之后） */

    /* 注册触摸输入设备（LVGL 按 LV_INDEV_DEF_READ_PERIOD=30ms 周期自动调 read_cb） */
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);

    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read_cb;

    lv_indev_drv_register(&indev_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/* 获取 XPT2046 设备对象并初始化（含 SetRotation 与 ST7789 保持同步） */
static void touchpad_init(void)
{
    pTouch = GetXPT2046Device();
    if(NULL == pTouch)   return;

    if(ESUCCESS != pTouch->Init(pTouch))   return;
    pTouch->SetRotation(pTouch, 2);   /* 与 ST7789 rotation 2 一致：竖屏 240×320 */
}

/* ---------- 触摸提示音 ---------- */

/* PE8 引脚配置（低电平触发，上电先置高=不响）并挂接 drv_gpio 的 BUZZER 对象 */
static void touch_beep_init(void)
{
    __HAL_RCC_GPIOE_CLK_ENABLE();
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);   /* 先写电平再配脚，避免初始化毛刺响一声 */

    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = GPIO_PIN_8;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &gpio);

    g_beep_dev = GPIODev_Find("BUZZER");
    if(NULL == g_beep_dev)   return;
    g_beep_dev->Write(g_beep_dev, 1);   /* 确保默认不响 */
}

/* 按下沿触发：非阻塞，只拉低+记下停响时刻，到时由 touch_beep_poll 拉高 */
static void touch_beep_start(void)
{
    if(!g_beep_en || g_beeping || NULL == g_beep_dev)   return;
    g_beeping = 1;
    g_beep_stop_tick = HAL_GetTick() + BEEP_DURATION_MS;
    g_beep_dev->Write(g_beep_dev, 0);
}

/* 到时自动停响：由 read_cb 周期调用（30ms 粒度，实际响声 80~110ms，无感差异） */
static void touch_beep_poll(void)
{
    if(g_beeping && (int32_t)(HAL_GetTick() - g_beep_stop_tick) >= 0)
    {
        g_beeping = 0;
        if(NULL != g_beep_dev)   g_beep_dev->Write(g_beep_dev, 1);
    }
}

/* 坐标变换（实测校正）：
 * 本屏 XPT2046 的 X 通道对应屏幕"垂直"方向（顶部小、底部大）；
 * Y 通道对应屏幕"水平"方向（左大、右小，反相）。
 * 驱动输出 x∈[0,239]（垂直压缩）、y∈[0,319]（水平反相），
 * 线性还原为显示像素坐标（0,0 = 屏幕左上角）：
 *   dy = x × 320 / 240
 *   dx = 239 − y × 240 / 320
 */
static void touchpad_read_cb(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    (void)indev_drv;

    touch_beep_poll();   /* 顺带检查提示音是否到时停响（read_cb 每 30ms 一次） */

    /* 上一轮状态：按下沿检测用（read_cb 每 30ms 轮询一次，按住不动只算一次按下） */
    static lv_indev_state_t last_state = LV_INDEV_STATE_RELEASED;

    uint16_t tx = 0, ty = 0;

    if(pTouch && pTouch->IsPressed(pTouch) && pTouch->ReadXY(pTouch, &tx, &ty))
    {
        data->point.x = (lv_coord_t)(239 - (uint32_t)ty * 240 / 320);
        data->point.y = (lv_coord_t)((uint32_t)tx * 320 / 240);
        data->state   = LV_INDEV_STATE_PRESSED;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }

    /* 触摸提示音：松开→按下 的边沿响一声（touch_beep_start 内部判断使能开关，非阻塞） */
    if(LV_INDEV_STATE_PRESSED == data->state && LV_INDEV_STATE_RELEASED == last_state)
    {
        touch_beep_start();
    }
    last_state = data->state;
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
