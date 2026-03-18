/**
 * @file lvgl_driver.c
 * @brief LVGL Display Driver Component Implementation
 * @version 1.0.0
 */

#include "lvgl_driver.h"
#include "esp_log.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "LVGL-Driver";

// LVGL 任务句柄
static TaskHandle_t lvgl_task_handle = NULL;
static lv_disp_t *disp = NULL;
static lv_indev_t *indev = NULL;
static bool is_paused = false;
static bool is_initialized = false;

// 默认配置
static lvgl_driver_config_t default_config = {
    .buffer_size = 0,                // 自动计算
    .double_buffer = true,
    .task_delay_ms = LVGL_TASK_DELAY_MS,
    .task_priority = LVGL_TASK_PRIORITY,
    .task_stack = LVGL_TASK_STACK,
};

// LVGL 端口配置 (简化版)
static void lvgl_port_config(void)
{
    // 设置默认显示参数
    lv_disp_t *main_disp = lv_disp_get_default();
    if (main_disp) {
        lv_disp_set_refreshed_area(main_disp, true);
    }
}

/**
 * @brief LVGL 任务
 */
static void lvgl_task(void *param)
{
    uint32_t delay_ms = default_config.task_delay_ms;
    
    while (1) {
        if (!is_paused) {
            lv_task_handler();
        }
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
}

/**
 * @brief 显示 flush 回调
 */
static void disp_flush(lv_disp_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    if (!is_initialized) {
        lv_disp_flush_ready(disp);
        return;
    }
    
    // 这里调用实际的 MIPI LCD flush 函数
    // TODO: 实现实际的像素数据传输
    
    lv_disp_flush_ready(disp);
}

/**
 * @brief 触摸读取回调
 */
static void touch_read(lv_indev_t *indev, lv_point_t *point)
{
    // TODO: 实现实际触摸读取
    // 可以调用 touch_gt911_read() 获取触摸数据
    
    point->x = 0;
    point->y = 0;
}

/**
 * @brief Rounder 回调 (用于调整刷新区域)
 */
static void disp_rounder(lv_disp_t *disp, lv_area_t *area)
{
    // 调整刷新区域为 4 字节对齐
    area->x1 &= ~0x3;
    area->x2 = (area->x2 & ~0x3) + 3;
}

/**
 * @brief 设置像素颜色 (可选优化)
 */
static void disp_set_px(lv_disp_t *disp, lv_coord_t x, lv_coord_t y, lv_color_t color)
{
    // 可选的像素设置优化
    // 用于单像素绘制时的快速操作
}

/**
 * @brief 创建显示驱动
 */
static lv_disp_t *create_display_driver(void)
{
    // 获取显示尺寸
    uint16_t width, height;
    esp32_mipi_get_size(&width, &height);
    
    // 分配显示缓冲区
    uint32_t buf_size = width * LVGL_BUF_LINES * sizeof(lv_color_t);
    
    static lv_color_t *buf1 = NULL;
    static lv_color_t *buf2 = NULL;
    
    buf1 = (lv_color_t *)heap_caps_malloc(buf_size, MALLOC_CAP_DMA);
    if (!buf1) {
        ESP_LOGE(TAG, "Failed to allocate buffer 1");
        return NULL;
    }
    
    if (default_config.double_buffer) {
        buf2 = (lv_color_t *)heap_caps_malloc(buf_size, MALLOC_CAP_DMA);
        if (!buf2) {
            ESP_LOGE(TAG, "Failed to allocate buffer 2");
            free(buf1);
            return NULL;
        }
    }
    
    // 创建显示驱动
    lv_disp_draw_buf_t draw_buf;
    if (buf2) {
        lv_disp_draw_buf_init(&draw_buf, buf1, buf2, width * LVGL_BUF_LINES);
    } else {
        lv_disp_draw_buf_init(&draw_buf, buf1, NULL, width * LVGL_BUF_LINES);
    }
    
    // 显示驱动配置
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    
    disp_drv.draw_buf = &draw_buf;
    disp_drv.flush_cb = disp_flush;
    disp_drv.rounder_cb = disp_rounder;
    disp_drv.set_px_cb = disp_set_px;
    disp_drv.hor_res = width;
    disp_drv.ver_res = height;
    disp_drv.direct_mode = false;
    disp_driver_path.antialiasing = true;
    disp_drv.full_refresh = false;
    disp_drv.sw_rotate = false;
    disp_drv.rotated = LV_DISP_ROT_NONE;
    
    return lv_disp_drv_register(&disp_drv);
}

/**
 * @brief 创建输入驱动 (触摸)
 */
static lv_indev_t *create_input_driver(void)
{
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touch_read;
    
    return lv_indev_drv_register(&indev_drv);
}

esp_err_t lvgl_driver_init(void)
{
    return lvgl_driver_init_with_config(NULL);
}

esp_err_t lvgl_driver_init_with_config(const lvgl_driver_config_t *config)
{
    if (is_initialized) {
        ESP_LOGW(TAG, "Already initialized");
        return ESP_OK;
    }
    
    ESP_LOGI(TAG, "Initializing LVGL Driver...");
    
    // 使用自定义配置
    if (config) {
        default_config = *config;
    }
    
    // 初始化 LVGL 核心
    lv_init();
    
    // LVGL 端口配置
    lvgl_port_config();
    
    // 创建显示驱动
    disp = create_display_driver();
    if (!disp) {
        ESP_LOGE(TAG, "Failed to create display driver");
        return ESP_FAIL;
    }
    
    // 创建输入驱动 (触摸)
    indev = create_input_driver();
    
    // 创建 LVGL 任务
    BaseType_t ret = xTaskCreatePinnedToCore(
        lvgl_task,
        "lvgl_task",
        default_config.task_stack,
        NULL,
        default_config.task_priority,
        &lvgl_task_handle,
        LVGL_TASK_CORE
    );
    
    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create LVGL task");
        return ESP_FAIL;
    }
    
    is_initialized = true;
    ESP_LOGI(TAG, "LVGL Driver initialized successfully");
    
    return ESP_OK;
}

lv_disp_t *lvgl_driver_get_disp(void)
{
    return disp;
}

lv_indev_t *lvgl_driver_get_indev(void)
{
    return indev;
}

esp_err_t lvgl_driver_lock(uint32_t timeout_ms)
{
    // LVGL 7.x 兼容的锁实现
    // 注意: LVGL 8+ 使用不同机制
    return ESP_OK;
}

void lvgl_driver_unlock(void)
{
    // LVGL 解锁
}

void lvgl_driver_pause(void)
{
    is_paused = true;
    ESP_LOGI(TAG, "LVGL paused");
}

void lvgl_driver_resume(void)
{
    is_paused = false;
    ESP_LOGI(TAG, "LVGL resumed");
}

esp_err_t lvgl_driver_deinit(void)
{
    if (!is_initialized) {
        return ESP_OK;
    }
    
    // 删除任务
    if (lvgl_task_handle) {
        vTaskDelete(lvgl_task_handle);
        lvgl_task_handle = NULL;
    }
    
    // 删除驱动
    if (disp) {
        lv_disp_remove(disp);
        disp = NULL;
    }
    
    if (indev) {
        lv_indev_remove(indev);
        indev = NULL;
    }
    
    is_initialized = false;
    ESP_LOGI(TAG, "LVGL Driver deinitialized");
    
    return ESP_OK;
}
