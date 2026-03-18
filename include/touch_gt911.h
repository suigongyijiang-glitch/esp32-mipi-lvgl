/**
 * @file touch_gt911.h
 * @brief Goodix GT911 Touch Panel Driver
 * @version 1.0.0
 */

#ifndef TOUCH_GT911_H
#define TOUCH_GT911_H

#include <stdint.h>
#include "esp_err.h"
#include "driver/i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GT911_I2C_ADDR         0x5D
#define GT911_I2C_PORT         I2C_NUM_0
#define GT911_I2C_SCL          GPIO_NUM_18
#define GT911_I2C_SDA          GPIO_NUM_17

#define GT911_MAX_TOUCHES      5

/**
 * @brief 触摸点数据结构
 */
typedef struct {
    uint8_t track_id;        // 触摸 ID
    uint16_t x;              // X 坐标
    uint16_t y;              // Y 坐标
    uint8_t weight;          // 压力权重
    uint8_t area;            // 触摸区域
} gt911_touch_point_t;

/**
 * @brief 触摸事件数据
 */
typedef struct {
    uint8_t touch_num;                       // 触摸点数
    gt911_touch_point_t points[GT911_MAX_TOUCHES];
} gt911_touch_event_t;

/**
 * @brief 触摸事件回调
 */
typedef void (*gt911_touch_callback_t)(const gt911_touch_event_t *event);

/**
 * @brief 初始化 GT911 触摸驱动
 * @param sda I2C SDA 引脚
 * @param scl I2C SCL 引脚
 * @return ESP_OK 成功
 */
esp_err_t touch_gt911_init(gpio_num_t sda, gpio_num_t scl);

/**
 * @brief 注册触摸回调
 * @param callback 回调函数
 * @return ESP_OK 成功
 */
esp_err_t touch_gt911_register_callback(gt911_touch_callback_t callback);

/**
 * @brief 读取触摸数据
 * @param event 触摸事件数据
 * @return ESP_OK 成功
 */
esp_err_t touch_gt911_read(gt911_touch_event_t *event);

/**
 * @brief 获取 GT911 产品信息
 */
esp_err_t touch_gt911_get_product_id(char *id);

#ifdef __cplusplus
}
#endif

#endif // TOUCH_GT911_H
