/**
 * @file ui_components.h
 * @brief UI Components Wrapper
 * @version 1.0.0
 */

#ifndef UI_COMPONENTS_H
#define UI_COMPONENTS_H

#include "lvgl.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

// 主题颜色
#define UI_COLOR_PRIMARY       LV_COLOR_MAKE(0x00, 0x86, 0xFF)
#define UI_COLOR_SECONDARY     LV_COLOR_MAKE(0xFF, 0x95, 0x00)
#define UI_COLOR_SUCCESS       LV_COLOR_MAKE(0x00, 0xC7, 0x53)
#define UI_COLOR_WARNING       LV_COLOR_MAKE(0xFF, 0xA7, 0x00)
#define UI_COLOR_ERROR         LV_COLOR_MAKE(0xFF, 0x3B, 0x30)
#define UI_COLOR_BG            LV_COLOR_MAKE(0x1A, 0x1A, 0x1A)
#define UI_COLOR_BG_LIGHT     LV_COLOR_MAKE(0x2A, 0x2A, 0x2A)
#define UI_COLOR_TEXT          LV_COLOR_MAKE(0xFF, 0xFF, 0xFF)
#define UI_COLOR_TEXT_SEC      LV_COLOR_MAKE(0xAA, 0xAA, 0xAA)

// 字体大小
#define UI_FONT_TITLE         &lv_font_montserrat_24
#define UI_FONT_SUBTITLE      &lv_font_montserrat_18
#define UI_FONT_BODY          &lv_font_montserrat_14
#define UI_FONT_SMALL         &lv_font_montserrat_12

/**
 * @brief 按钮样式
 */
typedef enum {
    UI_BUTTON_STYLE_PRIMARY,
    UI_BUTTON_STYLE_SECONDARY,
    UI_BUTTON_STYLE_OUTLINE,
    UI_BUTTON_STYLE_TEXT,
} ui_button_style_t;

/**
 * @brief 初始化 UI 主题
 */
void ui_theme_init(void);

/**
 * @brief 创建标签
 * @param parent 父对象
 * @param text 文本
 * @return 标签对象
 */
lv_obj_t *ui_label_create(lv_obj_t *parent, const char *text);

/**
 * @brief 创建按钮
 * @param parent 父对象
 * @param text 按钮文本
 * @return 按钮对象
 */
lv_obj_t *ui_button_create(lv_obj_t *parent, const char *text);

/**
 * @brief 设置按钮样式
 * @param btn 按钮对象
 * @param style 样式
 */
void ui_button_set_style(lv_obj_t *btn, ui_button_style_t style);

/**
 * @brief 创建滑块
 * @param parent 父对象
 * @param min 最小值
 * @param max 最大值
 * @param value 当前值
 * @return 滑块对象
 */
lv_obj_t *ui_slider_create(lv_obj_t *parent, int32_t min, int32_t max, int32_t value);

/**
 * @brief 创建开关
 * @param parent 父对象
 * @param state 初始状态
 * @return 开关对象
 */
lv_obj_t *ui_switch_create(lv_obj_t *parent, bool state);

/**
 * @brief 创建进度条
 * @param parent 父对象
 * @param max 最大值
 * @return 进度条对象
 */
lv_obj_t *ui_progressbar_create(lv_obj_t *parent, int32_t max);

/**
 * @brief 创建消息弹窗
 * @param title 标题
 * @param msg 消息内容
 * @param type 消息类型 (info/success/warning/error)
 */
void ui_msg_box(const char *title, const char *msg, lvAlertType_t type);

/**
 * @brief 创建加载指示器
 * @param parent 父对象
 * @return 加载对象
 */
lv_obj_t *ui_spinner_create(lv_obj_t *parent);

/**
 * @brief 创建列表项
 * @param parent 父对象
 * @param icon 图标 (LVGL 符号)
 * @param title 标题
 * @param subtitle 副标题
 * @return 列表项对象
 */
lv_obj_t *ui_list_item_create(lv_obj_t *parent, const char *icon, const char *title, const char *subtitle);

/**
 * @brief 创建卡片容器
 * @param parent 父对象
 * @return 卡片对象
 */
lv_obj_t *ui_card_create(lv_obj_t *parent);

/**
 * @brief 创建顶部栏
 * @param title 标题
 * @return 顶部栏对象
 */
lv_obj_t *ui_header_create(const char *title);

/**
 * @brief 创建底部导航栏
 * @param items 图标数组
 * @param count 项目数量
 * @return 底部栏对象
 */
lv_obj_t *ui_tabview_create(const char **icons, uint8_t count);

/**
 * @brief 创建圆形指示器
 * @param parent 父对象
 * @return 指示器对象
 */
lv_obj_t *ui_arc_create(lv_obj_t *parent);

/**
 * @brief 创建仪表盘
 * @param parent 父对象
 * @param min 最小值
 * @param max 最大值
 * @return 仪表盘对象
 */
lv_obj_t *ui_gauge_create(lv_obj_t *parent, int32_t min, int32_t max);

/**
 * @brief 创建图表
 * @param parent 父对象
 * @return 图表对象
 */
lv_obj_t *ui_chart_create(lv_obj_t *parent);

/**
 * @brief 添加图表数据点
 * @param chart 图表对象
 * @param series 系列索引
 * @param value 数据值
 */
void ui_chart_add_value(lv_obj_t *chart, uint8_t series, int32_t value);

/**
 * @brief 淡入显示对象
 * @param obj 目标对象
 * @param duration 动画时长 (ms)
 */
void ui_fade_in(lv_obj_t *obj, uint32_t duration);

/**
 * @brief 淡出隐藏对象
 * @param obj 目标对象
 * @param duration 动画时长 (ms)
 */
void ui_fade_out(lv_obj_t *obj, uint32_t duration);

/**
 * @brief 创建确认对话框
 * @param title 标题
 * @param msg 消息
 * @param confirm_btn 确认按钮文本
 * @param cancel_btn 取消按钮文本
 * @return 对话框对象
 */
lv_obj_t *ui_confirm_dialog(const char *title, const char *msg, 
                             const char *confirm_btn, const char *cancel_btn);

/**
 * @brief 屏幕截图 (保存到 SPIFFS)
 * @param path 保存路径
 * @return ESP_OK 成功
 */
esp_err_t ui_screenshot(const char *path);

#ifdef __cplusplus
}
#endif

#endif // UI_COMPONENTS_H
