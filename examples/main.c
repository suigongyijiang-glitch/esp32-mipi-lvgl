/**
 * @file main.c
 * @brief ESP32-P4 MIPI LCD with LVGL Example
 * @version 1.0.0
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp32_mipi_lvgl.h"
#include "lvgl.h"
#include "esp_log.h"

static const char *TAG = "MAIN";

/**
 * @brief 创建示例 UI
 */
static void create_demo_ui(void)
{
    // 创建标签
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "ESP32-P4 MIPI + LVGL");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 30);

    // 创建按钮
    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn, 120, 50);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
    
    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "Click Me");

    // 创建滑块
    lv_obj_t *slider = lv_slider_create(lv_scr_act());
    lv_obj_set_size(slider, 200, 20);
    lv_obj_align(slider, LV_ALIGN_BOTTOM_MID, 0, -50);
    lv_slider_set_value(slider, 50, LV_ANIM_OFF);

    // 底部标签
    lv_obj_t *footer = lv_label_create(lv_scr_act());
    lv_label_set_text(footer, "MIPI DSI 4-Lane @ 80MHz");
    lv_obj_set_style_text_font(footer, &lv_font_montserrat_12, 0);
    lv_obj_align(footer, LV_ALIGN_BOTTOM_MID, 0, -10);
}

/**
 * @brief LVGL 任务处理
 */
static void lvgl_task(void *param)
{
    while (1) {
        lv_task_handler();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Starting ESP32-P4 MIPI LCD Demo...");

    // 初始化 MIPI LCD 和 LVGL
    esp_err_t ret = esp32_mipi_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize MIPI LCD: %s", esp_err_to_name(ret));
        return;
    }

    // 创建示例 UI
    create_demo_ui();

    // 启动 LVGL 任务
    xTaskCreate(lvgl_task, "lvgl_task", 4096, NULL, 5, NULL);

    ESP_LOGI(TAG, "Demo started successfully!");
}
