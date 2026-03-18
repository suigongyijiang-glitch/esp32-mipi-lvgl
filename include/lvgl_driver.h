/**
 * @file lvgl_driver.h
 * @brief LVGL Display Driver Component
 * @version 1.0.0
 */

#ifndef LVGL_DRIVER_H
#define LVGL_DRIVER_H

#include "esp32_mipi_lvgl.h"
#include "lvgl.h"
#include "lvgl_port.h"

#ifdef __cplusplus
extern "C" {
#endif

// LVGL 驱动配置
#define LVGL_TASK_PRIORITY   5
#define LVGL_TASK_STACK      4096
#define LVGL_TASK_CORE       0
#define LVGL_TASK_DELAY_MS   10

// 缓冲区配置
#define LVGL_BUF_LINES       20      // 每个缓冲区行数
#define LVGL_BUF_COUNT       2        // 缓冲区数量

/**
 * @brief LVGL 驱动配置
 */
typedef struct {
    uint32_t buffer_size;            // 缓冲区大小
    bool double_buffer;              // 双缓冲
    uint32_t task_delay_ms;          // 任务延时
    int task_priority;               // 任务优先级
    uint32_t task_stack;             // 任务栈大小
} lvgl_driver_config_t;

/**
 * @brief 初始化 LVGL 驱动
 * @return ESP_OK 成功
 */
esp_err_t lvgl_driver_init(void);

/**
 * @brief 初始化 LVGL 驱动 (自定义配置)
 * @param config 驱动配置
 * @return ESP_OK 成功
 */
esp_err_t lvgl_driver_init_with_config(const lvgl_driver_config_t *config);

/**
 * @brief 获取 LVGL 显示设备
 * @return 显示设备句柄
 */
lv_disp_t *lvgl_driver_get_disp(void);

/**
 * @brief 获取 LVGL 输入设备 (触摸)
 * @return 输入设备句柄
 */
lv_indev_t *lvgl_driver_get_indev(void);

/**
 * @brief 锁定 LVGL
 * @param timeout_ms 超时时间 (ms)
 * @return ESP_OK 成功
 */
esp_err_t lvgl_driver_lock(uint32_t timeout_ms);

/**
 * @brief 解锁 LVGL
 */
void lvgl_driver_unlock(void);

/**
 * @brief 暂停 LVGL 任务
 */
void lvgl_driver_pause(void);

/**
 * @brief 恢复 LVGL 任务
 */
void lvgl_driver_resume(void);

/**
 * @brief 反初始化 LVGL 驱动
 * @return ESP_OK 成功
 */
esp_err_t lvgl_driver_deinit(void);

#ifdef __cplusplus
}
#endif

#endif // LVGL_DRIVER_H
