# API 参考手册

本文档提供 ESP32-P4 MIPI LVGL 驱动的完整 API 参考。

## 核心 API

### 显示驱动

#### `esp32_mipi_init`

```c
esp_err_t esp32_mipi_init(void);
```

初始化 MIPI LCD 面板和 LVGL。

**返回值**
- `ESP_OK` - 初始化成功
- `ESP_FAIL` - 初始化失败

---

#### `esp32_mipi_deinit`

```c
esp_err_t esp32_mipi_deinit(void);
```

关闭 MIPI LCD 面板。

**返回值**
- `ESP_OK` - 操作成功

---

#### `esp32_mipi_set_brightness`

```c
esp_err_t esp32_mipi_set_brightness(uint8_t brightness);
```

设置背光亮度。

**参数**
- `brightness` - 亮度值 (0-255)

**返回值**
- `ESP_OK` - 设置成功

**示例**
```c
// 设置 50% 亮度
esp32_mipi_set_brightness(128);

// 关闭背光
esp32_mipi_set_brightness(0);

// 最亮
esp32_mipi_set_brightness(255);
```

---

#### `esp32_mipi_get_size`

```c
void esp32_mipi_get_size(uint16_t *width, uint16_t *height);
```

获取显示尺寸。

**参数**
- `width` - 输出显示宽度
- `height` - 输出显示高度

**示例**
```c
uint16_t w, h;
esp32_mipi_get_size(&w, &h);
printf("Display: %dx%d\n", w, h);
```

---

### LVGL 回调

#### `mipi_lvgl_flush`

```c
void mipi_lvgl_flush(lv_disp_t *disp, const lv_area_t *area, lv_color_t *color_p);
```

LVGL 显示刷新回调函数。

> **注意**: 此函数由 LVGL 自动调用，用户不应直接调用。

---

#### `mipi_lvgl_read`

```c
void mipi_lvgl_read(lv_indev_t *indev, lv_point_t *point);
```

LVGL 触摸输入回调函数。

> **注意**: 此函数由 LVGL 自动调用。

---

## 面板驱动 API

### ST7789 面板

#### `panel_st7789_init`

```c
esp_err_t panel_st7789_init(void);
```

初始化 ST7789 面板。

**返回值**
- `ESP_OK` - 初始化成功
- `ESP_FAIL` - 初始化失败

---

### GT911 触摸

#### `touch_gt911_init`

```c
esp_err_t touch_gt911_init(gpio_num_t sda, gpio_num_t scl);
```

初始化 GT911 触摸屏驱动。

**参数**
- `sda` - I2C SDA 引脚
- `scl` - I2C SCL 引脚

**返回值**
- `ESP_OK` - 初始化成功

**示例**
```c
// 使用默认引脚初始化触摸
touch_gt911_init(GPIO_NUM_17, GPIO_NUM_18);
```

---

#### `touch_gt911_register_callback`

```c
esp_err_t touch_gt911_register_callback(gt911_touch_callback_t callback);
```

注册触摸事件回调。

**参数**
- `callback` - 回调函数

**返回值**
- `ESP_OK` - 注册成功

**回调函数类型**
```c
typedef void (*gt911_touch_callback_t)(const gt911_touch_event_t *event);
```

---

#### `touch_gt911_read`

```c
esp_err_t touch_gt911_read(gt911_touch_event_t *event);
```

读取触摸数据。

**参数**
- `event` - 触摸事件数据

**返回值**
- `ESP_OK` - 读取成功
- `ESP_FAIL` - 读取失败

---

## 数据类型

### `mipi_panel_config_t`

```c
typedef struct {
    uint16_t width;           // 显示宽度
    uint16_t height;          // 显示高度
    uint8_t lanes;            // MIPI 通道数 (2/4)
    uint32_t pixel_clock;     // 像素时钟 (Hz)
    uint8_t *init_cmds;       // 初始化命令
    size_t init_cmds_size;    // 命令长度
} mipi_panel_config_t;
```

---

### `mipi_pins_t`

```c
typedef struct {
    int dsi_clk_p;            // DSI 时钟正极
    int dsi_clk_n;            // DSI 时钟负极
    int dsi_data_p0;          // DSI 数据0正极
    int dsi_data_n0;          // DSI 数据0负极
    int dsi_data_p1;          // DSI 数据1正极
    int dsi_data_n1;          // DSI 数据1负极
    int dsi_data_p2;          // DSI 数据2正极
    int dsi_data_n2;          // DSI 数据2负极
    int dsi_data_p3;          // DSI 数据3正极
    int dsi_data_n3;          // DSI 数据3负极
    int rst_pin;              // 复位引脚
    int blk_pin;              // 背光引脚
} mipi_pins_t;
```

---

### `gt911_touch_event_t`

```c
typedef struct {
    uint8_t touch_num;                           // 触摸点数
    gt911_touch_point_t points[GT911_MAX_TOUCHES];
} gt911_touch_event_t;
```

---

## 配置选项

### sdkconfig 配置

```yaml
# MIPI DSI 配置
CONFIG_ESP32P4_DSI_HOST=y
CONFIG_ESP32P4_DSI_CONTROLLER=1

# LVGL 配置
CONFIG_LVGL_9=y
CONFIG_LVGL_BUFFER_DOUBLE=y
CONFIG_LVGL_COLOR_DEPTH_16=y
CONFIG_LVGL_MEM_CUSTOM=y

# 内存配置
CONFIG_LVGL_MEM_MAX_SIZE=524288
```

---

## 错误代码

| 错误代码 | 描述 |
|----------|------|
| `ESP_OK` | 操作成功 |
| `ESP_FAIL` | 操作失败 |
| `ESP_ERR_INVALID_ARG` | 参数无效 |
| `ESP_ERR_INVALID_STATE` | 状态错误 |
| `ESP_ERR_TIMEOUT` | 超时 |
