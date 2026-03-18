/**
 * @file ui_components.c
 * @brief UI Components Implementation
 * @version 1.0.0
 */

#include "ui_components.h"
#include "lvgl.h"
#include "esp_log.h"
#include "esp_err.h"

static const char *TAG = "UI-Components";

// 主题样式
static lv_style_t style_btn_primary;
static lv_style_t style_btn_secondary;
static lv_style_t style_btn_outline;
static lv_style_t style_card;
static lv_style_t style_header;
static bool theme_initialized = false;

/**
 * @brief 初始化 UI 主题
 */
void ui_theme_init(void)
{
    if (theme_initialized) {
        return;
    }
    
    ESP_LOGI(TAG, "Initializing UI theme...");
    
    // 按钮主样式
    lv_style_init(&style_btn_primary);
    lv_style_set_bg_color(&style_btn_primary, UI_COLOR_PRIMARY);
    lv_style_set_bg_opa(&style_btn_primary, LV_OPA_COVER);
    lv_style_set_radius(&style_btn_primary, 8);
    lv_style_set_text_color(&style_btn_primary, LV_COLOR_WHITE);
    lv_style_set_text_font(&style_btn_primary, UI_FONT_BODY);
    lv_style_set_pad_ver(&style_btn_primary, 12);
    lv_style_set_pad_hor(&style_btn_primary, 20);
    
    // 按钮次样式
    lv_style_init(&style_btn_secondary);
    lv_style_set_bg_color(&style_btn_secondary, UI_COLOR_SECONDARY);
    lv_style_set_bg_opa(&style_btn_secondary, LV_OPA_COVER);
    lv_style_set_radius(&style_btn_secondary, 8);
    lv_style_set_text_color(&style_btn_secondary, LV_COLOR_WHITE);
    lv_style_set_text_font(&style_btn_secondary, UI_FONT_BODY);
    lv_style_set_pad_ver(&style_btn_secondary, 12);
    lv_style_set_pad_hor(&style_btn_secondary, 20);
    
    // 按钮轮廓样式
    lv_style_init(&style_btn_outline);
    lv_style_set_border_color(&style_btn_outline, UI_COLOR_PRIMARY);
    lv_style_set_border_width(&style_btn_outline, 2);
    lv_style_set_radius(&style_btn_outline, 8);
    lv_style_set_text_color(&style_btn_outline, UI_COLOR_PRIMARY);
    lv_style_set_text_font(&style_btn_outline, UI_FONT_BODY);
    lv_style_set_pad_ver(&style_btn_outline, 12);
    lv_style_set_pad_hor(&style_btn_outline, 20);
    
    // 卡片样式
    lv_style_init(&style_card);
    lv_style_set_bg_color(&style_card, UI_COLOR_BG_LIGHT);
    lv_style_set_bg_opa(&style_card, LV_OPA_COVER);
    lv_style_set_radius(&style_card, 12);
    lv_style_set_shadow_width(&style_card, 10);
    lv_style_set_shadow_ofs_y(&style_card, 5);
    lv_style_set_shadow_opa(&style_card, LV_OPA_30);
    lv_style_set_pad_all(&style_card, 16);
    
    // 顶部栏样式
    lv_style_init(&style_header);
    lv_style_set_bg_color(&style_header, UI_COLOR_BG);
    lv_style_set_bg_opa(&style_header, LV_OPA_COVER);
    lv_style_set_radius(&style_header, 0);
    lv_style_set_pad_all(&style_header, 16);
    
    theme_initialized = true;
    ESP_LOGI(TAG, "UI theme initialized");
}

/**
 * @brief 创建标签
 */
lv_obj_t *ui_label_create(lv_obj_t *parent, const char *text)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, UI_COLOR_TEXT, 0);
    lv_obj_set_style_text_font(label, UI_FONT_BODY, 0);
    return label;
}

/**
 * @brief 创建按钮
 */
lv_obj_t *ui_button_create(lv_obj_t *parent, const char *text)
{
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    
    // 默认使用主样式
    lv_obj_add_style(btn, &style_btn_primary, 0);
    
    return btn;
}

/**
 * @brief 设置按钮样式
 */
void ui_button_set_style(lv_obj_t *btn, ui_button_style_t style)
{
    lv_obj_remove_style(btn, NULL, 0);
    
    switch (style) {
        case UI_BUTTON_STYLE_PRIMARY:
            lv_obj_add_style(btn, &style_btn_primary, 0);
            break;
        case UI_BUTTON_STYLE_SECONDARY:
            lv_obj_add_style(btn, &style_btn_secondary, 0);
            break;
        case UI_BUTTON_STYLE_OUTLINE:
            lv_obj_add_style(btn, &style_btn_outline, 0);
            break;
        case UI_BUTTON_STYLE_TEXT:
            lv_obj_set_style_text_color(btn, UI_COLOR_PRIMARY, 0);
            break;
    }
}

/**
 * @brief 创建滑块
 */
lv_obj_t *ui_slider_create(lv_obj_t *parent, int32_t min, int32_t max, int32_t value)
{
    lv_obj_t *slider = lv_slider_create(parent);
    lv_slider_set_range(slider, min, max);
    lv_slider_set_value(slider, value, LV_ANIM_OFF);
    lv_obj_set_width(slider, lv_pct(80));
    return slider;
}

/**
 * @brief 创建开关
 */
lv_obj_t *ui_switch_create(lv_obj_t *parent, bool state)
{
    lv_obj_t *sw = lv_switch_create(parent);
    if (state) {
        lv_obj_add_state(sw, LV_STATE_CHECKED);
    }
    return sw;
}

/**
 * @brief 创建进度条
 */
lv_obj_t *ui_progressbar_create(lv_obj_t *parent, int32_t max)
{
    lv_obj_t *bar = lv_bar_create(parent);
    lv_bar_set_range(bar, 0, max);
    lv_bar_set_value(bar, 0, LV_ANIM_OFF);
    lv_obj_set_width(bar, lv_pct(80));
    return bar;
}

/**
 * @brief 创建消息弹窗
 */
void ui_msg_box(const char *title, const char *msg, lvAlertType_t type)
{
    lv_obj_t *mbox = lv_msgbox_create(NULL, title, msg, NULL, true);
    lv_obj_center(mbox);
}

/**
 * @brief 创建加载指示器
 */
lv_obj_t *ui_spinner_create(lv_obj_t *parent)
{
    lv_obj_t *spinner = lv_spinner_create(parent, 1000, 60);
    lv_obj_set_size(spinner, 50, 50);
    lv_obj_center(spinner);
    return spinner;
}

/**
 * @brief 创建列表项
 */
lv_obj_t *ui_list_item_create(lv_obj_t *parent, const char *icon, 
                                const char *title, const char *subtitle)
{
    lv_obj_t *item = lv_obj_create(parent);
    lv_obj_set_width(item, lv_pct(100));
    lv_obj_set_height(item, 60);
    lv_obj_set_flex_flow(item, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(item, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(item, LV_OBJ_FLAG_SCROLLABLE);
    
    // 图标
    if (icon) {
        lv_obj_t *img = lv_label_create(item);
        lv_label_set_text(img, icon);
        lv_obj_set_style_text_font(img, &lv_font_montserrat_24, 0);
    }
    
    // 标题
    lv_obj_t *title_label = lv_label_create(item);
    lv_label_set_text(title_label, title);
    lv_obj_set_style_text_font(title_label, UI_FONT_BODY, 0);
    lv_obj_set_style_text_color(title_label, UI_COLOR_TEXT, 0);
    lv_obj_set_flex_grow(title_label, 1);
    
    // 副标题
    if (subtitle) {
        lv_obj_t *sub_label = lv_label_create(item);
        lv_label_set_text(sub_label, subtitle);
        lv_obj_set_style_text_font(sub_label, UI_FONT_SMALL, 0);
        lv_obj_set_style_text_color(sub_label, UI_COLOR_TEXT_SEC, 0);
    }
    
    return item;
}

/**
 * @brief 创建卡片
 */
lv_obj_t *ui_card_create(lv_obj_t *parent)
{
    lv_obj_t *card = lv_obj_create(parent);
    lv_obj_add_style(card, &style_card, 0);
    lv_obj_set_flex_flow(card, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(card, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    return card;
}

/**
 * @brief 创建顶部栏
 */
lv_obj_t *ui_header_create(const char *title)
{
    lv_obj_t *scr = lv_scr_act();
    
    lv_obj_t *header = lv_obj_create(scr);
    lv_obj_set_size(header, LV_HOR_RES, 50);
    lv_obj_set_pos(header, 0, 0);
    lv_obj_add_style(header, &style_header, 0);
    lv_obj_clear_flag(header, LV_OBJ_FLAG_SCROLLABLE);
    
    lv_obj_t *title_label = lv_label_create(header);
    lv_label_set_text(title_label, title);
    lv_obj_set_style_text_font(title_label, UI_FONT_SUBTITLE, 0);
    lv_obj_set_style_text_color(title_label, UI_COLOR_TEXT, 0);
    lv_obj_center(title_label);
    
    return header;
}

/**
 * @brief 创建底部导航栏 (简化版)
 */
lv_obj_t *ui_tabview_create(const char **icons, uint8_t count)
{
    lv_obj_t *tabview = lv_tabview_create(lv_scr_act(), LV_DIR_BOTTOM, 50);
    
    for (uint8_t i = 0; i < count; i++) {
        lv_obj_t *tab = lv_tabview_add_tab(tabview, icons[i]);
        
        lv_obj_t *label = lv_label_create(tab);
        lv_label_set_text(label, icons[i]);
        lv_obj_center(label);
    }
    
    return tabview;
}

/**
 * @brief 创建圆形指示器
 */
lv_obj_t *ui_arc_create(lv_obj_t *parent)
{
    lv_obj_t *arc = lv_arc_create(parent);
    lv_obj_set_size(arc, 150, 150);
    lv_arc_set_rotation(arc, 135);
    lv_arc_set_bg_angles(arc, 0, 270);
    lv_arc_set_value(arc, 50);
    lv_obj_center(arc);
    return arc;
}

/**
 * @brief 创建仪表盘
 */
lv_obj_t *ui_gauge_create(lv_obj_t *parent, int32_t min, int32_t max)
{
    lv_obj_t *gauge = lv_gauge_create(parent);
    lv_gauge_set_range(gauge, min, max);
    lv_gauge_set_value(gauge, 0, 50);
    lv_obj_set_size(gauge, 200, 200);
    return gauge;
}

/**
 * @brief 创建图表
 */
lv_obj_t *ui_chart_create(lv_obj_t *parent)
{
    lv_obj_t *chart = lv_chart_create(parent);
    lv_obj_set_size(chart, lv_pct(90), 150);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(chart, 20);
    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_CIRCULAR);
    
    // 添加数据系列
    lv_chart_series_t *ser1 = lv_chart_add_series(chart, UI_COLOR_PRIMARY, LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_all_value(chart, ser1, 0);
    
    return chart;
}

/**
 * @brief 添加图表数据点
 */
void ui_chart_add_value(lv_obj_t *chart, uint8_t series, int32_t value)
{
    lv_chart_series_t *ser = lv_chart_get_series(chart, series);
    if (ser) {
        lv_chart_set_next_value(chart, ser, value);
    }
}

/**
 * @brief 淡入动画
 */
void ui_fade_in(lv_obj_t *obj, uint32_t duration)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_opa);
    lv_anim_set_time(&a, duration);
    lv_anim_set_values(&a, LV_OPA_TRANSP, LV_OPA_COVER);
    lv_anim_set_obj(&a, obj);
    lv_anim_start(&a);
}

/**
 * @brief 淡出动画
 */
void ui_fade_out(lv_obj_t *obj, uint32_t duration)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_opa);
    lv_anim_set_time(&a, duration);
    lv_anim_set_values(&a, LV_OPA_COVER, LV_OPA_TRANSP);
    lv_anim_set_obj(&a, obj);
    lv_anim_start(&a);
}

/**
 * @brief 创建确认对话框
 */
lv_obj_t *ui_confirm_dialog(const char *title, const char *msg,
                             const char *confirm_btn, const char *cancel_btn)
{
    static const char *btns[] = {confirm_btn, cancel_btn};
    lv_obj_t *dialog = lv_msgbox_create(NULL, title, msg, btns, true);
    lv_obj_center(dialog);
    return dialog;
}

/**
 * @brief 屏幕截图
 */
esp_err_t ui_screenshot(const char *path)
{
    // TODO: 实现截图功能
    ESP_LOGI(TAG, "Screenshot to: %s", path);
    return ESP_OK;
}
