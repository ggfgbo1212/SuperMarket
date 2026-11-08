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
#include <string.h>                    /* memmove/strncpy：导航历史栈、SSID 缓存 */
#include <stdint.h>                    /* uintptr_t：把行号放进事件 user_data */
#include "dev_st7789.h"                /* lcd_set_brightness：背光 PWM 调光 */
#include "lv_port_indev_template.h"    /* lv_port_indev_beep_enable：触摸提示音开关 */
#include "wifi_scan.h"                 /* WifiScan_Connect / WifiScan_FetchConnectResult */

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
    {
        setup_scr_SettingsPage2Wireless1(&guider_ui);
        wifi_list_ui_create();   /* 进入页面前预建好 15 个列表容器 */
    }

    nav_push_and_load(guider_ui.SettingsPage2Wireless1);
}

/* settings_p1_enter_remote1：「远程管理」cont_2 点击 → 懒创建并跳转 SettingsPage3Remote1。
 * 未连接 WiFi 时忽略（卡片此时已不可点击，这里再兜底一次，防止残留状态/程序化触发） */
void settings_p1_enter_remote1(lv_event_t *e)
{
    char ssid[33];
    (void)e;

    if(!WifiScan_GetConnectedSsid(ssid, sizeof(ssid)))   return;

    if(NULL == guider_ui.SettingsPage3Remote1)
        setup_scr_SettingsPage3Remote1(&guider_ui);

    nav_push_and_load(guider_ui.SettingsPage3Remote1);
}

/* ==================== SettingsPage1：联网相关卡片使能 ====================
 * cont_2「远程管理」、cont_3「管理系统」依赖 WiFi：
 *   - 未连接：清除 CLICKABLE 标志（整卡不可点击，label 本身也不可点击），并显示为灰色；
 *   - 已连接：恢复可点击与正常配色。
 * 由 tick_screen_SettingsPage1 周期调用；只有连接状态变化时才改样式，避免每帧刷新。 */

#define SETTINGS_P1_CARD_ON_BG       0xFFFFFF
#define SETTINGS_P1_CARD_ON_BORDER   0xFFFFFF
#define SETTINGS_P1_CARD_OFF_BG      0xF1F3F6
#define SETTINGS_P1_CARD_OFF_BORDER  0xE3E7ED
#define SETTINGS_P1_TEXT_ON          0x000000
#define SETTINGS_P1_TEXT_OFF         0xA8B0BB

static int8_t s_p1_net_cards_enabled = -1;   /* -1-尚未应用；0-禁用；1-使能 */

static void settings_p1_net_card_set(lv_obj_t *card, lv_obj_t *label, unsigned char enabled)
{
    if(NULL == card)   return;

    if(enabled)   lv_obj_add_flag(card, LV_OBJ_FLAG_CLICKABLE);
    else          lv_obj_clear_flag(card, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_set_style_bg_color(card,
        lv_color_hex(enabled ? SETTINGS_P1_CARD_ON_BG : SETTINGS_P1_CARD_OFF_BG),
        LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(card,
        lv_color_hex(enabled ? SETTINGS_P1_CARD_ON_BORDER : SETTINGS_P1_CARD_OFF_BORDER),
        LV_PART_MAIN|LV_STATE_DEFAULT);

    if(NULL != label)
        lv_obj_set_style_text_color(label,
            lv_color_hex(enabled ? SETTINGS_P1_TEXT_ON : SETTINGS_P1_TEXT_OFF),
            LV_PART_MAIN|LV_STATE_DEFAULT);
}

void settings_p1_net_cards_update(void)
{
    char ssid[33];
    unsigned char enabled = WifiScan_GetConnectedSsid(ssid, sizeof(ssid)) ? 1 : 0;

    if((int8_t)enabled == s_p1_net_cards_enabled)   return;   /* 状态未变：不重设样式 */
    s_p1_net_cards_enabled = (int8_t)enabled;

    settings_p1_net_card_set(guider_ui.SettingsPage1_cont_2,
                             guider_ui.SettingsPage1_label_2, enabled);
    settings_p1_net_card_set(guider_ui.SettingsPage1_cont_3,
                             guider_ui.SettingsPage1_label_4, enabled);
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

/* ==================== SettingsPage2Wireless1：WiFi 列表 ==================== */

/* 列表布局模仿 ESP_Epoch 参考工程（esp32_firmware 的 settings_p2_wireless1_ui_create）：
 *   - 卡片 216×60、圆角 8、卡片间距 4px；本工程 list_1 内容区 203×141，
 *     一屏约 2~3 个卡片，其余上下滑动查看
 *   - 名称 label 左缩进 31px（给锁/已连接图标留位）、垂直居中，长度超出省略号截断；
 *     卡片右侧留 40px（给信号强度图标留位）
 *   - 开关关闭时隐藏整个 list_1（参考工程 action_settings_p2_wireless1_wifilist_show_hide 同做法） */
#define WIFI_LIST_ROW_W       203    /* list_1 内容区宽度（213 - 左右 padding 5） */
#define WIFI_LIST_ROW_H       60     /* 参考工程 PANEL_HEIGHT */
#define WIFI_LIST_ROW_GAP     4      /* 参考工程 PANEL_SPACING */
#define WIFI_LIST_LABEL_X     31     /* 参考工程 label x 坐标：左侧图标位 */
#define WIFI_LIST_LABEL_W     (WIFI_LIST_ROW_W - WIFI_LIST_LABEL_X - 40)
#define WIFI_LIST_LABEL_H     16
#define WIFI_LIST_CHECK_X     4      /* 复选框 x 坐标（放在 label 左侧的图标位） */

static lv_obj_t *s_wifi_rows[WIFI_LIST_ROW_NUM];        /* 每行的容器 */
static lv_obj_t *s_wifi_row_labels[WIFI_LIST_ROW_NUM];  /* 容器里的 SSID label */
static lv_obj_t *s_wifi_checks[WIFI_LIST_ROW_NUM];      /* 容器里的连接状态复选框 */
static char s_wifi_ssid[WIFI_LIST_ROW_NUM][33];         /* 每行的完整 SSID（label 只显示截断文本） */

/* 行点击 → 记录选中的热点并进入 SettingsPage2Wireless2（定义见下方无线密码页） */
static void wifi_row_clicked_cb(lv_event_t *e);

/* wifi_list_ui_create：在 list_1 中预建 15 个容器（各含一个复选框 + 名字 label），默认隐藏。
 * 幂等：只在首次进入无线设置页时创建一次；GUI Guider 重新导出不影响本文件。 */
void wifi_list_ui_create(void)
{
    lv_obj_t *list = guider_ui.SettingsPage2Wireless1_list_1;
    if(NULL == list || NULL != s_wifi_rows[0])   return;

    /* 卡片间距 4px；sw_2 默认关闭，整个列表先隐藏（打开开关再显示） */
    lv_obj_set_style_pad_row(list, WIFI_LIST_ROW_GAP, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_flag(list, LV_OBJ_FLAG_HIDDEN);

    for(int i = 0; i < WIFI_LIST_ROW_NUM; i++)
    {
        /* 行容器：白色圆角卡片，隐藏时脱离 list 的纵向 flex 布局 */
        lv_obj_t *row = lv_obj_create(list);
        lv_obj_clear_flag(row, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_size(row, WIFI_LIST_ROW_W, WIFI_LIST_ROW_H);
        lv_obj_set_style_bg_opa(row, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(row, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(row, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_radius(row, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_pad_all(row, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_add_flag(row, LV_OBJ_FLAG_HIDDEN);

        /* 连接状态复选框：不接收点击（点击穿透到整行卡片），只由程序勾选。
         * 勾选态 = 已连接热点（显示在列表第一行，见 wifi_list_ui_show）。
         * 默认主题的对勾是 LV_SYMBOL_OK 字形，montserrat_14 含该字符。 */
        lv_obj_t *cb = lv_checkbox_create(row);
        lv_checkbox_set_text(cb, "");
        lv_obj_clear_flag(cb, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(cb, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
        lv_obj_align(cb, LV_ALIGN_LEFT_MID, WIFI_LIST_CHECK_X, 0);
        lv_obj_set_style_pad_all(cb, 2, LV_PART_INDICATOR|LV_STATE_DEFAULT);   /* 方框 16+2*2=20px */
        lv_obj_set_style_radius(cb, 4, LV_PART_INDICATOR|LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(cb, 2, LV_PART_INDICATOR|LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(cb, lv_color_hex(0xB8C0CC), LV_PART_INDICATOR|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(cb, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(cb, lv_color_hex(0x0D3055), LV_PART_INDICATOR|LV_STATE_CHECKED);
        lv_obj_set_style_border_color(cb, lv_color_hex(0x0D3055), LV_PART_INDICATOR|LV_STATE_CHECKED);
        lv_obj_set_style_text_color(cb, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR|LV_STATE_CHECKED);
        lv_obj_set_style_text_font(cb, &lv_font_montserrat_14, LV_PART_INDICATOR|LV_STATE_CHECKED);

        /* 热点名称 label：左缩进 31px、垂直居中，长度超出用省略号截断。
         * 左侧 31px 图标位放连接状态复选框，右侧 40px 预留给信号强度图标 */
        lv_obj_t *label = lv_label_create(row);
        lv_label_set_text(label, "");
        lv_label_set_long_mode(label, LV_LABEL_LONG_DOT);
        lv_obj_set_size(label, WIFI_LIST_LABEL_W, WIFI_LIST_LABEL_H);
        lv_obj_align(label, LV_ALIGN_LEFT_MID, WIFI_LIST_LABEL_X, 0);
        lv_obj_set_style_text_font(label, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(label, lv_color_hex(0x0D3055), LV_PART_MAIN|LV_STATE_DEFAULT);

        /* 点击整张卡片（含名称文字）都进入密码页：label 不拦截点击，事件落在行容器上 */
        lv_obj_clear_flag(label, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(row, wifi_row_clicked_cb, LV_EVENT_CLICKED, (void *)(uintptr_t)i);

        s_wifi_rows[i] = row;
        s_wifi_row_labels[i] = label;
        s_wifi_checks[i] = cb;
    }
}

/* wifi_list_ui_show：显示列表并填充前 count 个热点名，未满的行隐藏（count=0 即空列表）。
 * 已连接热点：复选框打勾，并像参考工程 settings_p2_wireless1_update_wifi_list_display
 * 那样与第 0 个元素交换位置，永远显示在第一行。 */
void wifi_list_ui_show(const WiFiApInfo *aps, int count)
{
    lv_obj_t *list = guider_ui.SettingsPage2Wireless1_list_1;
    int order[WIFI_LIST_ROW_NUM];
    char connected[33];
    int connected_row = -1;

    if(NULL == aps || count < 0)   count = 0;
    if(count > WIFI_LIST_ROW_NUM)  count = WIFI_LIST_ROW_NUM;

    if(NULL != list)   lv_obj_clear_flag(list, LV_OBJ_FLAG_HIDDEN);   /* 开关打开：显示整个列表 */

    /* 显示顺序：默认按扫描顺序；找到已连接热点则交换到第一个 */
    for(int i = 0; i < WIFI_LIST_ROW_NUM; i++)   order[i] = i;

    if(WifiScan_GetConnectedSsid(connected, sizeof(connected)))
    {
        for(int i = 0; i < count; i++)
        {
            if(0 == strcmp(aps[i].ssid, connected))
            {
                if(i != 0)   /* 交换显示顺序（等价参考工程交换 list[0] 与 list[i]） */
                {
                    int temp = order[0];
                    order[0] = order[i];
                    order[i] = temp;
                }
                connected_row = 0;
                break;
            }
        }
    }

    for(int i = 0; i < WIFI_LIST_ROW_NUM; i++)
    {
        if(NULL == s_wifi_rows[i])   return;   /* 容器还没创建 */

        if(i < count)
        {
            const WiFiApInfo *ap = &aps[order[i]];

            lv_label_set_text(s_wifi_row_labels[i], ap->ssid);
            strncpy(s_wifi_ssid[i], ap->ssid, sizeof(s_wifi_ssid[i]) - 1);
            s_wifi_ssid[i][sizeof(s_wifi_ssid[i]) - 1] = '\0';
            lv_obj_clear_flag(s_wifi_rows[i], LV_OBJ_FLAG_HIDDEN);

            /* 只有真正找到已连接热点时第一行才打勾；热点不在扫描结果里则都不勾 */
            if(i == connected_row)
                lv_obj_add_state(s_wifi_checks[i], LV_STATE_CHECKED);
            else
                lv_obj_clear_state(s_wifi_checks[i], LV_STATE_CHECKED);
        }
        else
        {
            s_wifi_ssid[i][0] = '\0';
            lv_obj_add_flag(s_wifi_rows[i], LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_state(s_wifi_checks[i], LV_STATE_CHECKED);
        }
    }
}

/* wifi_list_ui_hide_all：开关关闭 → 隐藏整个列表（参考工程同做法） */
void wifi_list_ui_hide_all(void)
{
    lv_obj_t *list = guider_ui.SettingsPage2Wireless1_list_1;
    if(NULL != list)
        lv_obj_add_flag(list, LV_OBJ_FLAG_HIDDEN);
}

/* ==================== SettingsPage2Wireless2：密码输入 + 连接 ====================
 *
 * 参考 ESP_Epoch 工程（settings_p2_wireless2 的键盘确认 + wireless_wifi_connect_task）：
 *   1. 列表页点击任意热点卡片 → 记录完整 SSID、懒创建本页；
 *   2. 页面下方键盘绑定 ta_1，输入密码后按键盘 OK（LV_EVENT_READY）确认；
 *   3. 连接请求交给后台扫描任务（与 WSCAN 串行），UI 不阻塞；
 *   4. 成功 → 自动返回 SettingsPage2Wireless1；
 *      失败 → ta_1 输入框内显示“密码错误”（再次输入时消失）。
 *
 * 注意：本文件所有 lv_* 调用都在 UI 线程（事件回调 / 页面 tick）里。
 */

#define WIRELESS2_TA_W        214     /* 生成代码里 ta_1 的尺寸/位置 */
#define WIRELESS2_TA_H        55
#define WIRELESS2_TA_X        13
#define WIRELESS2_TA_Y        51
#define WIRELESS2_PWD_MAX     63      /* WPA2 PSK 最长 63 字符 */

LV_IMG_DECLARE(wifi_pwd_error_214x55);   /* 预渲染的“密码错误”提示图（见 wifi_pwd_error_img.c） */

static char s_selected_ssid[33];               /* 列表页选中的热点（完整 SSID） */
static lv_obj_t *s_wireless2_kb = NULL;            /* 底部键盘 */
static lv_obj_t *s_wireless2_err_img = NULL;       /* “密码错误”提示图（整块覆盖 ta_1） */
static lv_obj_t *s_wireless2_status_label = NULL;  /* 连接中的状态提示 */
static unsigned char s_connect_pending = 0;        /* 已发出连接请求，等待后台结果 */

/* 显示“密码错误”：状态提示让位，错误图盖到 ta_1 上 */
static void wireless2_show_error(void)
{
    if(NULL != s_wireless2_status_label)
        lv_obj_add_flag(s_wireless2_status_label, LV_OBJ_FLAG_HIDDEN);

    if(NULL != s_wireless2_err_img)
    {
        lv_obj_clear_flag(s_wireless2_err_img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(s_wireless2_err_img);
    }
}

/* ta_1 内容改变/被点击：隐藏“密码错误”，让用户继续修改密码 */
static void wireless2_ta_edit_cb(lv_event_t *e)
{
    (void)e;

    if(NULL != s_wireless2_err_img)
        lv_obj_add_flag(s_wireless2_err_img, LV_OBJ_FLAG_HIDDEN);
}

/* 键盘 OK（LV_EVENT_READY）→ 读取 ta_1 密码并投递连接请求 */
static void wireless2_kb_ready_cb(lv_event_t *e)
{
    (void)e;

    lv_obj_t *ta = guider_ui.SettingsPage2Wireless2_ta_1;
    const char *pwd;

    if(0 != s_connect_pending)   return;          /* 一次只发一个请求 */
    if(NULL == ta || '\0' == s_selected_ssid[0])   return;

    pwd = lv_textarea_get_text(ta);
    if(NULL == pwd)   pwd = "";

    if(!WifiScan_Connect(s_selected_ssid, pwd))
    {
        wireless2_show_error();                   /* 任务未就绪/正忙：按失败提示 */
        return;
    }

    s_connect_pending = 1;
    if(NULL != s_wireless2_err_img)
        lv_obj_add_flag(s_wireless2_err_img, LV_OBJ_FLAG_HIDDEN);
    if(NULL != s_wireless2_status_label)
        lv_obj_clear_flag(s_wireless2_status_label, LV_OBJ_FLAG_HIDDEN);
}

/* 首次进入密码页时创建键盘/提示图（幂等；页面本身由 setup_scr_ 懒创建） */
static void wireless2_ui_create(void)
{
    lv_obj_t *scr = guider_ui.SettingsPage2Wireless2;
    lv_obj_t *ta  = guider_ui.SettingsPage2Wireless2_ta_1;

    if(NULL == scr || NULL == ta || NULL != s_wireless2_kb)   return;

    /* ta_1：生成代码是多行文本框 + 真实文字提示；改成单行密码输入 + 占位提示 */
    lv_textarea_set_one_line(ta, true);
    lv_obj_set_size(ta, WIRELESS2_TA_W, WIRELESS2_TA_H);   /* one_line 会切成内容高度，恢复原尺寸 */
    lv_textarea_set_max_length(ta, WIRELESS2_PWD_MAX);
    /* 占位提示“请输入无线密码”：用 UTF-8 字节转义写，避免 Keil 按本地编码解析中文字符串出错 */
    lv_textarea_set_placeholder_text(ta,
                                     "\xE8\xAF\xB7\xE8\xBE\x93\xE5\x85\xA5"
                                     "\xE6\x97\xA0\xE7\xBA\xBF\xE5\xAF\x86\xE7\xA0\x81");
    lv_obj_set_style_text_color(ta, lv_color_hex(0x333333), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ta, lv_color_hex(0xb0b0b0),
                                LV_PART_TEXTAREA_PLACEHOLDER|LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ta, wireless2_ta_edit_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(ta, wireless2_ta_edit_cb, LV_EVENT_CLICKED, NULL);

    /* 页面下方键盘：绑定 ta_1；OK 键发出 LV_EVENT_READY，作为“确认连接” */
    s_wireless2_kb = lv_keyboard_create(scr);
    lv_obj_set_size(s_wireless2_kb, 240, 160);
    lv_obj_align(s_wireless2_kb, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_keyboard_set_textarea(s_wireless2_kb, ta);
    lv_obj_add_event_cb(s_wireless2_kb, wireless2_kb_ready_cb, LV_EVENT_READY, NULL);

    /* “密码错误”提示：当前字库缺“错/误”字形，用预渲染图片整块覆盖 ta_1；
     * 图片不可点击，点击穿透到 ta_1（点击后隐藏图片、恢复输入） */
    s_wireless2_err_img = lv_img_create(scr);
    lv_img_set_src(s_wireless2_err_img, &wifi_pwd_error_214x55);
    lv_obj_set_pos(s_wireless2_err_img, WIRELESS2_TA_X, WIRELESS2_TA_Y);
    lv_obj_clear_flag(s_wireless2_err_img, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(s_wireless2_err_img, LV_OBJ_FLAG_HIDDEN);

    /* 连接中提示（ASCII；标题与输入框之间，键盘上方） */
    s_wireless2_status_label = lv_label_create(scr);
    lv_label_set_text(s_wireless2_status_label, "Connecting...");
    lv_obj_set_style_text_font(s_wireless2_status_label, &lv_font_montserratMedium_16,
                               LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(s_wireless2_status_label, lv_color_hex(0x0D3055),
                                LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_align(s_wireless2_status_label, LV_ALIGN_TOP_MID, 0, 112);
    lv_obj_add_flag(s_wireless2_status_label, LV_OBJ_FLAG_HIDDEN);
}

/* 离开密码页时调用（UI 线程）：清除在途标记并丢弃残留结果。
 * 若连接结果回传前用户已按返回键离开，不清除 pending 的话，
 * wifi_row_clicked_cb 会一直拒绝列表行点击，界面表现为 WiFi 列表点不动。 */
void wireless2_ui_deactivate(void)
{
    unsigned char stale;

    s_connect_pending = 0;
    while(WifiScan_FetchConnectResult(&stale)) { }
}

/* 每次进入密码页：清空输入、清掉上次的错误/连接中状态、丢弃残留结果 */
static void wireless2_ui_reset(void)
{
    lv_obj_t *ta = guider_ui.SettingsPage2Wireless2_ta_1;

    wireless2_ui_deactivate();   /* 连接期间离开过页面：丢弃旧结果、清在途标记 */

    if(NULL != ta)
        lv_textarea_set_text(ta, "");
    if(NULL != s_wireless2_err_img)
        lv_obj_add_flag(s_wireless2_err_img, LV_OBJ_FLAG_HIDDEN);
    if(NULL != s_wireless2_status_label)
        lv_obj_add_flag(s_wireless2_status_label, LV_OBJ_FLAG_HIDDEN);

    if(NULL != s_wireless2_kb && NULL != ta)
    {
        lv_keyboard_set_textarea(s_wireless2_kb, ta);
        lv_obj_clear_flag(s_wireless2_kb, LV_OBJ_FLAG_HIDDEN);
    }
}

/* 列表行点击：选中热点 → 懒创建/重置密码页 → 入导航栈跳转 */
static void wifi_row_clicked_cb(lv_event_t *e)
{
    int idx = (int)(uintptr_t)lv_event_get_user_data(e);

    if(0 != s_connect_pending)   return;
    if(idx < 0 || idx >= WIFI_LIST_ROW_NUM)   return;
    if('\0' == s_wifi_ssid[idx][0])   return;     /* 空行/已隐藏 */

    strncpy(s_selected_ssid, s_wifi_ssid[idx], sizeof(s_selected_ssid) - 1);
    s_selected_ssid[sizeof(s_selected_ssid) - 1] = '\0';

    if(NULL == guider_ui.SettingsPage2Wireless2)
        setup_scr_SettingsPage2Wireless2(&guider_ui);

    wireless2_ui_create();    /* 幂等：首次创建键盘/提示控件 */
    wireless2_ui_reset();
    nav_push_and_load(guider_ui.SettingsPage2Wireless2);
}

/* 密码页 tick（UI 线程）：轮询后台连接结果 */
void wireless2_ui_tick(void)
{
    unsigned char ok;

    if(NULL == guider_ui.SettingsPage2Wireless2)   return;
    if(!WifiScan_FetchConnectResult(&ok))          return;

    if(0 == s_connect_pending)   return;   /* 残留结果：已离开并重新进入过页面，丢弃 */

    s_connect_pending = 0;

    if(ok)   /* 连接成功：自动返回 WiFi 列表页 */
    {
        if(NULL != s_wireless2_status_label)
            lv_obj_add_flag(s_wireless2_status_label, LV_OBJ_FLAG_HIDDEN);
        settings_back(NULL);
    }
    else     /* 连接失败：ta_1 输入框内显示“密码错误” */
    {
        wireless2_show_error();
    }
}
