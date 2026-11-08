# SuperMarket —— STM32F407 通用应用框架

基于 STM32F407VET6 + FreeRTOS + LVGL 的分层嵌入式应用框架，本仓库由 MQTT OTA 升级项目裁剪而来（已移除阿里云连接、OTA 升级与版本回退相关代码），作为后续新项目的通用底座。

## 一、框架组成

| 目录 | 内容 |
|---|---|
| `Core/` | CubeMX 生成的工程内核（时钟、GPIO、DMA、USART、I2C、SPI、FreeRTOS） |
| `1_Appcations/` | 应用入口 `gpio_test.c`（设备注册 + LVGL 显示任务），框架主干 |
| `2_Devices/` | 设备抽象层：GPIO / UART / IIC / SPI 总线设备注册，以及 W25Q64 SPI Flash、AT24C02 EEPROM、WiFi/蓝牙模块、ST7789 屏幕、XPT2046 触摸等具体设备 |
| `3_ModuleDrivers/` | 底层寄存器/HAL 驱动：drv_gpio / drv_uart / drv_iic / drv_spi / drv_flash |
| `4_include/` | 公共头文件（errno.h） |
| `5_libs/` | printf 重定向、环形缓冲区 |
| `6_Middlewares/lvgl/` | LVGL 图形库 + 移植层 + GUI Guider 生成目录 + action / tickscreen 两层 UI 架构 |
| `MDK-ARM/` | Keil MDK 工程（basic.uvprojx） |

## 二、UI 页面模式

框架采用「一页三件套」的组织方式（GUI Guider 生成或手写均可）：

1. **setup 层**：`SleepAid_UI/generated/setup_scr_XXX.c` —— 创建页面所有控件与样式，事件绑定写在文件末尾；
2. **action 层**：`lvgl/action/lvgl_action.c` —— 控件事件回调集中实现（GUI Guider 重新生成不会覆盖）；
3. **tick 层**：`lvgl/tickscreen/tickscreen.c` —— 每页一个周期函数，由 `ui_tick()` 按当前屏幕分发，页面周期逻辑（刷新状态等）写这里。

跨线程通信约定：LVGL 非线程安全，业务线程只写全局变量 / Give 信号量 / Send 队列，所有 `lv_*` 调用始终留在 UI 线程（DispTask）。

当前界面（GUI Guider 工程 `OTAUI.guiguider` 导出，上电首屏为 SettingsPage1）：
- `SettingsPage1` 设置主页
- `SettingsPage2Basic` 基础设置（滑条 / 开关）
- `SettingsPage2Wireless1` / `SettingsPage2Wireless2` 无线设置（列表 / 文本框）
- `SettingsPage3Remote1` 遥控器（文本框 / 按钮矩阵）

## 三、开发板硬件

| 项目 | 参数 |
|---|---|
| 主控 | STM32F407VET6（Cortex-M4F，168MHz） |
| 片内资源 | 512KB Flash + 192KB SRAM |
| 外部存储 | W25Q64 SPI Flash（8MB）、AT24C02 EEPROM |
| 屏幕 | ST7789 彩色 LCD（240×320）+ XPT2046 触摸 |
| 联网 | WiFi / 蓝牙模块（串口 AT 指令驱动，`2_Devices/wifi`） |
| 系统 | FreeRTOS 实时操作系统 |

![开发板实物图](images/board.png)

## 四、上手说明

1. 使用 Keil MDK 打开 `MDK-ARM/basic.uvprojx` 编译下载；
2. 上电后屏幕显示 HomeScreen 示例页，即说明框架运行正常；
3. 调试串口为 USART1，printf / debugprintf 已重定向，可直接输出日志；
4. 新增业务：在 `1_Appcations/` 建任务，按「UI 页面模式」小节添加界面。

## 五、许可

见 [LICENSE](LICENSE)。
