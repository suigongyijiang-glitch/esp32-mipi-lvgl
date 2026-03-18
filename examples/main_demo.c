/**
 * @file main_demo.c
 * @brief UI Components Demo
 * @version 1.0.0
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp32_mipi_lvgl.h"
#include "lvgl_driver.h"
#include "ui_components.h"
#include "esp_log.h"

static const char *TAG = "UI-Demo";

/**
 * @brief 主界面
 */
static void create_main_screen(void)
{
    lv_obj_t *scr = lv_scr_act();
    
    // 标题
    lv_obj_t *title = ui_label_create(scr, "ESP32-P4 LVGL Demo");
    lv_obj_set_style_text_font(title, UI_FONT_TITLE, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 30);
    
    // 卡片容器
    lv_obj_t *card = ui_card_create(scr);
    lv_obj_set_size(card, 280, 200);
    lv_obj_align(card, LV_ALIGN_CENTER, 0, -20);
    
    // 卡片内内容
    lv_obj_t *label1 = ui_label_create(card, "Welcome to ESP32-P4!");
    lv_obj_set_style_text_font(label1, UI_FONT_SUBTITLE, 0);
    
    // 滑块
    lv_obj_t *slider_label = ui_label_create(card, "Brightness");
    lv_obj_t *slider = ui_slider_create(card, 0, 255, 128);
    lv_obj_set_style_margin_top(slider, 10);
    
    // 按钮
    lv_obj_t *btn = ui_button_create(card, "Click Me!");
    lv_obj_set_style_margin_top(btn, 20);
    
    // 底部提示
    lv_obj_t *footer = ui_label_create(scr, "MIPI DSI + LVGL 9.x");
    lv_obj_set_style_text_font(footer, UI_FONT_SMALL, 0);
    lv_obj_set_style_text_color(footer, UI_COLOR_TEXT_SEC, 0);
    lv_obj_align(footer, LV_ALIGN_BOTTOM_MID, 0, -20);
}

/**
 * @brief 仪表盘界面
 */
static void create_gauge_screen(void)
{
    lv_obj_t *scr = lv_scr_act();
    lv_obj_clean(scr);
    
    // 标题
    lv_obj_t *title = ui_label_create(scr, "Dashboard");
    lv_obj_set_style_text_font(title, UI_FONT_TITLE, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);
    
    // 仪表盘
    lv_obj_t *gauge = ui_gauge_create(scr, 0, 100);
    lv_gauge_set_value(gauge, 0, 65);
    lv_obj_align(gauge, LV_ALIGN_CENTER, 0, 0);
    
    // 进度条
    lv_obj_t *label = ui_label_create(scr, "Progress");
    lv_obj_align(label, LV_ALIGN_CENTER, -80, 80);
    lv_obj_t *bar = ui_progressbar_create(scr, 100);
    lv_bar_set_value(bar, 70, LV_ANIM_ON);
    lv_obj_align(bar, LV_ALIGN_CENTER, 40, 80);
    
    // 返回按钮
    lv_obj_t *btn = ui_button_create(scr, "Back");
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -30);
}

/**
 * @brief 设置界面
 */
static void create_settings_screen(void)
{
    lv_obj_t *scr = lv_scr_act();
    lv_obj_clean(scr);
    
    // 标题
    lv_obj_t *title = ui_label_create(scr, "Settings");
    lv_obj_set_style_text_font(title, UI_FONT_TITLE, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);
    
    // 设置项容器
    lv_obj_t *container = lv_obj_create(scr);
    lv_obj_set_size(container, 300, 200);
    lv_obj_align(container, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);
    
    // WiFi 开关
    lv_obj_t *wifi_label = ui_label_create(container, "WiFi");
    lv_obj_t *wifi_sw = ui_switch_create(container, true);
    
    // 蓝牙开关
    lv_obj_t *bt_label = ui_label_create(container, "Bluetooth");
    lv_obj_t *bt_sw = ui_switch_create(container, false);
    
    // 自动亮度开关
    lv_obj_t *auto_br_label = ui_label_create(container, "Auto Brightness");
    lv_obj_t *auto_br_sw = ui_switch_create(container, true);
    
    // 返回按钮
    lv_obj_t *btn = ui_button_create(scr, "Back");
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -30);
}

/**
 * @brief 图表界面
 */
static void create_chart_screen(void)
{
    lv_obj_t *scr = lv_scr_act();
    lv_obj_clean(scr);
    
    // 标题
    lv_obj_t *title = ui_label_create(scr, "Sensor Data");
    lv_obj_set_style_text_font(title, UI_FONT_TITLE, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);
    
    // 图表
    lv_obj_t *chart = ui_chart_create(scr);
    lv_obj_align(chart, LV_ALIGN_CENTER, 0, -20);
    
    // 添加数据 (模拟)
    for (int i = 0; i < 20; i++) {
        int32_t value = 50 + (esp_random() % 50);
        ui_chart_add_value(chart, 0, value);
    }
    
    // 返回按钮
    lv_obj_t *btn = ui_button_create(scr, "Back");
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -30);
}

/**
 * @brief 演示任务
 */
static void demo_task(void *param)
{
    uint32_t counter = 0;
    
    while (1) {
        // 周期性更新 UI
        vTaskDelay(pdMS_TO_TICKS(1000));
        
        // 演示数据更新
        counter++;
        if (counter % 10 == 0) {
            ESP_LOGI(TAG, "Demo running: %lu seconds", counter);
        }
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Starting UI Demo...");
    
    // 初始化 MIPI LCD
    esp_err_t ret = esp32_mipi_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize MIPI LCD: %s", esp_err_to_name(ret));
        return;
    }
    
    // 初始化 LVGL 驱动
    ret = lvgl_driver_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize LVGL Driver: %s", esp_err_to_name(ret));
        return;
    }
    
    // 初始化 UI 主题
    ui_theme_init();
    
    // 创建主界面
    create_main_screen();
    
    // 启动演示任务
    xTaskCreate(demo_task, "demo_task", 2048, NULL, 3, NULL);
    
    ESP_LOGI(TAG, "UI Demo started successfully!");
}
