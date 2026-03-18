/**
 * @file panel_st7789.h
 * @brief ST7789 MIPI LCD Panel Driver
 * @version 1.0.0
 */

#ifndef PANEL_ST7789_H
#define PANEL_ST7789_H

#include "esp32_mipi_lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ST7789 面板初始化命令
 */
static const uint8_t st7789_init_cmds[] = {
    // 退出睡眠
    0x01, 1, 0x00, 0x00,
    // 帧率设置
    0xB2, 5, 0x00, 0x1E, 0x00, 0x04, 0x04,
    // 胀气/胃气胀设置
    0xB7, 1, 0x34,
    // VCOM 设置
    0xBB, 1, 0x30,
    // LCM 设置
    0xC0, 1, 0x2C,
    // VDV VRH 设置
    0xC2, 2, 0x01, 0xFF,
    // VRH 设置
    0xC3, 1, 0x10,
    // VDV 设置
    0xC4, 1, 0x20,
    // 正常帧率设置
    0xC6, 1, 0x0F,
    // VCOM 设置
    0xD0, 2, 0xA4, 0xA1,
    // Gamma 设置
    0xE0, 14, 0xD0, 0x00, 0x02, 0x07, 0x0A, 0x28, 0x32, 0x44, 0x42, 0x06, 0x0E, 0x12, 0x14, 0x17,
    // Gamma 设置
    0xE1, 14, 0xD0, 0x00, 0x02, 0x07, 0x0A, 0x28, 0x31, 0x44, 0x42, 0x06, 0x0E, 0x12, 0x14, 0x17,
    // 打开显示
    0x21, 0, 0x00,
    0x29, 0, 0x00,
    // 像素格式 16bit
    0x3A, 1, 0x55,
    // 结束
    0x00
};

/**
 * @brief ST7789 面板配置
 */
static const mipi_panel_config_t st7789_panel = {
    .width = 480,
    .height = 640,
    .lanes = 2,
    .pixel_clock = 50000000,
    .init_cmds = (uint8_t *)st7789_init_cmds,
    .init_cmds_size = sizeof(st7789_init_cmds),
};

/**
 * @brief 初始化 ST7789 面板
 */
esp_err_t panel_st7789_init(void);

#ifdef __cplusplus
}
#endif

#endif // PANEL_ST7789_H
