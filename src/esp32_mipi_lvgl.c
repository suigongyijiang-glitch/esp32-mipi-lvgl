/**
 * @file esp32_mipi_lvgl.c
 * @brief ESP32-P4 MIPI Display Driver with LVGL Implementation
 * @version 1.0.0
 */

#include "esp32_mipi_lvgl.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_mipi_dsi.h"
#include "lvgl.h"
#include "lvgl_port.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "ESP32-MIPI-LVGL";

static bool is_initialized = false;
static uint16_t disp_width = DISPLAY_HORIZONTAL;
static uint16_t disp_height = DISPLAY_VERTICAL;
static uint8_t current_brightness = 128;

// 默认引脚配置
static mipi_pins_t default_pins = {
    .dsi_clk_p = GPIO_NUM_40,
    .dsi_clk_n = GPIO_NUM_41,
    .dsi_data_p0 = GPIO_NUM_42,
    .dsi_data_n0 = GPIO_NUM_43,
    .dsi_data_p1 = GPIO_NUM_44,
    .dsi_data_n1 = GPIO_NUM_45,
    .dsi_data_p2 = GPIO_NUM_46,
    .dsi_data_n2 = GPIO_NUM_47,
    .dsi_data_p3 = GPIO_NUM_48,
    .dsi_data_n3 = GPIO_NUM_49,
    .rst_pin = GPIO_NUM_21,
    .blk_pin = GPIO_NUM_40,
};

static mipi_pins_t *current_pins = &default_pins;

/**
 * @brief MIPI DSI 初始化
 */
static esp_err_t mipi_dsi_init(void)
{
    ESP_LOGI(TAG, "Initializing MIPI DSI...");

    // 配置 DSI 主机
    dsi_host_config_t host_config = {
        .host_id = DSI_HOST_0,
        .controller_num = 0,
        .clk_ipu = 0,
        .clk_phy = 0,
        .phy_config = NULL,
    };

    ESP_ERROR_CHECK(dsi_host_init(&host_config));

    // 配置 DSI 设备
    dsi_device_config_t device_config = {
        .device_id = 0x123456,        // 面板 ID，根据实际修改
        .virtual_channel_id = 0,
        .panel_color_format = MIPI_DSI_COLOR_FMT_24BIT,
        .video_mode = MIPI_DSI_VIDEO_SYNC_PULSE,
        .dpi_config = {
            .dpi_clock_mhz = 80,       // 根据面板调整
            .video_size = {
                .width = disp_width,
                .height = disp_height,
            },
            .hsa = 0,
            .hbp = 20,
            .hline = disp_width + 20,
            .vsa = 0,
            .vbp = 20,
            .vfp = 20,
        },
    };

    ESP_ERROR_CHECK(dsi_device_init(0, &device_config));

    ESP_LOGI(TAG, "MIPI DSI initialized successfully");
    return ESP_OK;
}

/**
 * @brief 背光 PWM 初始化
 */
static esp_err_t backlight_init(void)
{
    ESP_LOGI(TAG, "Initializing backlight...");

    ledc_timer_config_t timer_conf = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .freq_hz = BLK_PWM_FREQ,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer_conf));

    ledc_channel_config_t channel_conf = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = BLK_PWM_CHANNEL,
        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = current_pins->blk_pin,
        .duty = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&channel_conf));

    ESP_LOGI(TAG, "Backlight initialized");
    return ESP_OK;
}

/**
 * @brief GPIO 初始化
 */
static esp_err_t gpio_init(void)
{
    // RST 引脚
    if (current_pins->rst_pin >= 0) {
        gpio_config_t rst_conf = {
            .pin_bit_mask = (1ULL << current_pins->rst_pin),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
        };
        ESP_ERROR_CHECK(gpio_config(&rst_conf));

        // 复位序列
        gpio_set_level(current_pins->rst_pin, 0);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level(current_pins->rst_pin, 1);
        vTaskDelay(pdMS_TO_TICKS(50));
    }

    return ESP_OK;
}

/**
 * @brief LVGL 显示驱动 flush 回调
 */
void mipi_lvgl_flush(lv_disp_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    if (!is_initialized) {
        lv_disp_flush_ready(disp);
        return;
    }

    // 这里调用实际的面板 flush 函数
    // esp_lcd_panel_draw_bitmap(...)

    lv_disp_flush_ready(disp);
}

/**
 * @brief LVGL 读点回调 (触摸屏)
 */
void mipi_lvgl_read(lv_indev_t *indev, lv_point_t *point)
{
    // TODO: 实现触摸屏读取
    point->x = 0;
    point->y = 0;
}

/**
 * @brief 初始化 MIPI LCD
 */
esp_err_t esp32_mipi_init(void)
{
    if (is_initialized) {
        ESP_LOGW(TAG, "Already initialized");
        return ESP_OK;
    }

    ESP_LOGI(TAG, "Initializing ESP32-P4 MIPI LCD with LVGL...");

    // 初始化 GPIO
    ESP_ERROR_CHECK(gpio_init());

    // 初始化 MIPI DSI
    ESP_ERROR_CHECK(mipi_dsi_init());

    // 初始化背光
    ESP_ERROR_CHECK(backlight_init());

    // 初始化 LVGL
    lv_init();
    
    // LVGL 端口配置
    lvgl_port_cfg_t lvgl_cfg = {
        .task_priority = LVGL_TASK_PRIORITY,
        .task_stack = LVGL_TASK_STACK,
        .task_affinity = 0,
        .task_max_sleep_ms = 500,
        .timer_period_ms = 10,
    };
    ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));

    // 添加显示设备
    lvgl_port_display_cfg_t display_cfg = {
        .buffer_size = LVGL_BUFFER_SIZE,
        .double_buffer = true,
        .flags = {
            .swap_bytes = false,
        }
    };
    
    // 注意: 实际使用需要创建 esp_lcd_panel_handle_t
    // lvgl_port_add_disp(&display_cfg);

    // 默认亮度 50%
    esp32_mipi_set_brightness(128);

    is_initialized = true;
    ESP_LOGI(TAG, "ESP32-P4 MIPI LCD initialized successfully");

    return ESP_OK;
}

/**
 * @brief 关闭 MIPI LCD
 */
esp_err_t esp32_mipi_deinit(void)
{
    if (!is_initialized) {
        return ESP_OK;
    }

    esp32_mipi_set_brightness(0);
    is_initialized = false;

    ESP_LOGI(TAG, "MIPI LCD deinitialized");
    return ESP_OK;
}

/**
 * @brief 设置背光亮度
 */
esp_err_t esp32_mipi_set_brightness(uint8_t brightness)
{
    current_brightness = brightness;
    
    uint32_t duty = (brightness * 255) / 255;
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, BLK_PWM_CHANNEL, duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, BLK_PWM_CHANNEL));

    ESP_LOGD(TAG, "Backlight brightness: %d", brightness);
    return ESP_OK;
}

/**
 * @brief 获取显示尺寸
 */
void esp32_mipi_get_size(uint16_t *width, uint16_t *height)
{
    *width = disp_width;
    *height = disp_height;
}
