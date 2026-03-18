/**
 * @file touch_gt911.c
 * @brief Goodix GT911 Touch Panel Driver Implementation
 * @version 1.0.0
 */

#include "touch_gt911.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"

static const char *TAG = "GT911-Touch";

// GT911 寄存器地址
#define GT911_REG_PRODUCT_ID      0x0100
#define GT911_REG_STATUS          0x010E
#define GT911_REG_POINT_NUM        0x010F
#define GT911_REG_POINTS          0x0110
#define GT911_REG_COMMAND         0x8040

// 触摸点信息结构 (5 bytes)
typedef struct {
    uint8_t track_id;
    uint8_t x_high : 4,
            res : 4;
    uint8_t x_low;
    uint8_t y_high : 4,
            res2 : 4;
    uint8_t y_low;
    uint8_t weight;
    uint8_t area;
} __attribute__((packed)) gt911_point_t;

static gt911_touch_callback_t touch_callback = NULL;
static bool is_initialized = false;

/**
 * @brief I2C 初始化
 */
static esp_err_t i2c_master_init(gpio_num_t sda, gpio_num_t scl)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda,
        .scl_io_num = scl,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000,
    };
    
    ESP_ERROR_CHECK(i2c_param_config(GT911_I2C_PORT, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(GT911_I2C_PORT, conf.mode, 0, 0, 0));
    
    return ESP_OK;
}

/**
 * @brief I2C 读取寄存器
 */
static esp_err_t i2c_read_reg(uint16_t reg, uint8_t *data, size_t len)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (GT911_I2C_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, (reg >> 8) & 0xFF, true);
    i2c_master_write_byte(cmd, reg & 0xFF, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (GT911_I2C_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, data, len, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    
    esp_err_t ret = i2c_master_cmd_begin(GT911_I2C_PORT, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    
    return ret;
}

/**
 * @brief I2C 写入寄存器
 */
static esp_err_t i2c_write_reg(uint16_t reg, uint8_t data)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (GT911_I2C_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, (reg >> 8) & 0xFF, true);
    i2c_master_write_byte(cmd, reg & 0xFF, true);
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);
    
    esp_err_t ret = i2c_master_cmd_begin(GT911_I2C_PORT, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    
    return ret;
}

esp_err_t touch_gt911_init(gpio_num_t sda, gpio_num_t scl)
{
    if (is_initialized) {
        return ESP_OK;
    }
    
    ESP_LOGI(TAG, "Initializing GT911 Touch...");
    
    // 初始化 I2C
    ESP_ERROR_CHECK(i2c_master_init(sda, scl));
    
    // 复位 GT911
    gpio_config_t rst_conf = {
        .pin_bit_mask = (1ULL << GPIO_NUM_20),
        .mode = GPIO_MODE_OUTPUT,
    };
    gpio_config(&rst_conf);
    
    gpio_set_level(GPIO_NUM_20, 0);
    vTaskDelay(pdMS_TO_TICKS(10));
    gpio_set_level(GPIO_NUM_20, 1);
    vTaskDelay(pdMS_TO_TICKS(100));
    
    is_initialized = true;
    ESP_LOGI(TAG, "GT911 Touch initialized");
    
    return ESP_OK;
}

esp_err_t touch_gt911_register_callback(gt911_touch_callback_t callback)
{
    touch_callback = callback;
    return ESP_OK;
}

esp_err_t touch_gt911_read(gt911_touch_event_t *event)
{
    if (!is_initialized || !event) {
        return ESP_FAIL;
    }
    
    uint8_t status;
    uint8_t point_num;
    
    // 读取状态和触摸点数量
    ESP_ERROR_CHECK(i2c_read_reg(GT911_REG_STATUS, &status, 1));
    ESP_ERROR_CHECK(i2c_read_reg(GT911_REG_POINT_NUM, &point_num, 1));
    
    // 清除状态
    i2c_write_reg(GT911_REG_STATUS, 0x00);
    
    event->touch_num = point_num > GT911_MAX_TOUCHES ? GT911_MAX_TOUCHES : point_num;
    
    if (point_num == 0 || point_num > GT911_MAX_TOUCHES) {
        return ESP_OK;
    }
    
    // 读取触摸点数据
    uint8_t point_data[GT911_MAX_TOUCHES * sizeof(gt911_point_t)];
    ESP_ERROR_CHECK(i2c_read_reg(GT911_REG_POINTS, point_data, sizeof(point_data)));
    
    for (int i = 0; i < event->touch_num; i++) {
        gt911_point_t *point = (gt911_point_t *)&point_data[i * sizeof(gt911_point_t)];
        
        event->points[i].track_id = point->track_id;
        event->points[i].x = (point->x_high << 8) | point->x_low;
        event->points[i].y = (point->y_high << 8) | point->y_low;
        event->points[i].weight = point->weight;
        event->points[i].area = point->area;
    }
    
    // 触发回调
    if (touch_callback) {
        touch_callback(event);
    }
    
    return ESP_OK;
}

esp_err_t touch_gt911_get_product_id(char *id)
{
    uint8_t data[4];
    esp_err_t ret = i2c_read_reg(GT911_REG_PRODUCT_ID, data, 4);
    
    if (ret == ESP_OK) {
        memcpy(id, data, 4);
        id[4] = '\0';
    }
    
    return ret;
}
