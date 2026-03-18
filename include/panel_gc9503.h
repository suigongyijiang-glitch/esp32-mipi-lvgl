/**
 * @file panel_gc9503.h
 * @brief GC9503 MIPI LCD Panel Driver
 * @version 1.0.0
 */

#ifndef PANEL_GC9503_H
#define PANEL_GC9503_H

#include "esp32_mipi_lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief GC9503 面板初始化命令
 */
static const uint8_t gc9503_init_cmds[] = {
    // 软件复位
    0x01, 1, 0x00, 0x00,
    vTaskDelay(pdMS_TO_TICKS(120)),
    // 退出睡眠
    0x11, 0, 0x00,
    vTaskDelay(pdMS_TO_TICKS(10)),
    // 接口设置
    0xF0, 2, 0x5A, 0x69,
    // 电源设置
    0xF0, 2, 0x5A, 0x66,
    // VREG1/2 倍率
    0xC8, 4, 0xFF, 0xF3, 0x0F, 0x00,
    // VCOM 设置
    0xC0, 3, 0x13, 0x13, 0x13,
    // Gamma 设置
    0xE0, 14, 0x00, 0x04, 0x0E, 0x0F, 0x11, 0x12, 0x13, 0x19, 0x1B, 0x1E, 0x21, 0x23, 0x24, 0x26,
    0xE1, 14, 0x00, 0x03, 0x0D, 0x0E, 0x10, 0x11, 0x12, 0x19, 0x1B, 0x1E, 0x21, 0x23, 0x24, 0x26,
    // 像素格式
    0x3A, 1, 0x55,
    // 打开显示
    0x29, 0, 0x00,
    // 结束
    0x00
};

/**
 * @brief GC9503 面板配置 (常见 480x480 圆形屏)
 */
static const mipi_panel_config_t gc9503_panel = {
    .width = 480,
    .height = 480,
    .lanes = 2,
    .pixel_clock = 60000000,
    .init_cmds = (uint8_t *)gc9503_init_cmds,
    .init_cmds_size = sizeof(gc9503_init_cmds),
};

/**
 * @brief 初始化 GC9503 面板
 */
esp_err_t panel_gc9503_init(void);

#ifdef __cplusplus
}
#endif

#endif // PANEL_GC9503_H
