/**
 * @file esp32_mipi_lvgl.h
 * @brief ESP32-P4 MIPI Display Driver with LVGL
 * @version 1.0.0
 */

#ifndef ESP32_MIPI_LVGL_H
#define ESP32_MIPI_LVGL_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"
#include "driver/dlpı.h"

#ifdef __cplusplus
extern "C" {
#endif

// MIPI DSI 配置
#define MIPI_DSI_LANES         4
#define MIPI_DSI_FORMAT        DSI_COLOR_FORMAT_24BIT
#define MIPI_DSI_MODE          DSI_CMD_MODE

// 显示配置
#define DISPLAY_HORIZONTAL     800
#define DISPLAY_VERTICAL       1280
#define DISPLAY_BITS_PER_PIXEL 16

// LVGL 配置
#define LVGL_BUFFER_SIZE       (DISPLAY_HORIZONTAL * DISPLAY_VERTICAL * 2 / 10)
#define LVGL_TASK_PRIORITY    5
#define LVGL_TASK_STACK       4096

// 背光 PWM 通道
#define BLK_PWM_CHANNEL        0
#define BLK_PWM_FREQ           1000

/**
 * @brief MIPI LCD 面板配置
 */
typedef struct {
    uint16_t width;
    uint16_t height;
    uint8_t lanes;
    uint32_t pixel_clock;
    uint8_t *init_cmds;      // 初始化命令序列
    size_t init_cmds_size;
} mipi_panel_config_t;

/**
 * @brief 显示驱动配置
 */
typedef struct {
    int dsi_clk_p;           // DSI 时钟正极
    int dsi_clk_n;           // DSI 时钟负极
    int dsi_data_p0;         // DSI 数据0正极
    int dsi_data_n0;         // DSI 数据0负极
    int dsi_data_p1;         // DSI 数据1正极
    int dsi_data_n1;         // DSI 数据1负极
    int dsi_data_p2;         // DSI 数据2正极
    int dsi_data_n2;         // DSI 数据2负极
    int dsi_data_p3;         // DSI 数据3正极
    int dsi_data_n3;         // DSI 数据3负极
    int rst_pin;             // 复位引脚
    int blk_pin;             // 背光引脚
} mipi_pins_t;

/**
 * @brief 初始化 MIPI LCD
 * @param config 面板配置
 * @param pins 引脚配置
 * @return ESP_OK 成功
 */
esp_err_t esp32_mipi_init(void);

/**
 * @brief 关闭 MIPI LCD
 * @return ESP_OK 成功
 */
esp_err_t esp32_mipi_deinit(void);

/**
 * @brief 设置背光亮度
 * @param brightness 亮度 0-255
 * @return ESP_OK 成功
 */
esp_err_t esp32_mipi_set_brightness(uint8_t brightness);

/**
 * @brief LVGL 显示器 flush 回调
 */
void mipi_lvgl_flush(lv_disp_t *disp, const lv_area_t *area, lv_color_t *color_p);

/**
 * @brief LVGL 读点回调 (用于触摸屏)
 */
void mipi_lvgl_read(lv_indev_t *indev, lv_point_t *point);

/**
 * @brief 获取显示尺寸
 */
void esp32_mipi_get_size(uint16_t *width, uint16_t *height);

#ifdef __cplusplus
}
#endif

#endif // ESP32_MIPI_LVGL_H
