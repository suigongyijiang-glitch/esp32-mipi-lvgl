# ESP32-P4 MIPI Display Driver with LVGL

ESP32-P4 高性能显示解决方案，采用 MIPI DSI 接口驱动显示屏，集成 LVGL 9.x 图形库。

## 功能特性

- 🎯 ESP32-P4 MIPI DSI 接口驱动
- 📺 支持多种 MIPI LCD 面板
- 🎨 LVGL 9.x 完整集成
- 🔧 支持 PlatformIO 和 ESP-IDF
- 📱 支持触摸屏驱动

## 硬件支持

| 开发板 | MIPI DSI | 状态 |
|--------|----------|------|
| ESP32-P4 | ✅ 支持 | 已测试 |

## 快速开始

### 1. 克隆项目

```bash
git clone https://github.com/suigongyijiang-glitch/esp32-mipi-lvgl.git
cd esp32-mipi-lvgl
```

### 2. 使用 PlatformIO

```bash
pio pkg install
pio run -t upload
```

### 3. 使用 ESP-IDF

```bash
idf.py build
idf.py flash monitor
```

## 引脚连接

| ESP32-P4 | MIPI LCD |
|----------|----------|
| GPIO 40  | DSI_CLK_P |
| GPIO 41  | DSI_CLK_N |
| GPIO 42  | DSI_DATA_P0 |
| GPIO 43  | DSI_DATA_N0 |
| GPIO 44  | DSI_DATA_P1 |
| GPIO 45  | DSI_DATA_N1 |
| GPIO 46  | DSI_DATA_P2 |
| GPIO 47  | DSI_DATA_N2 |
| GPIO 48  | DSI_DATA_P3 |
| GPIO 49  | DSI_DATA_N3 |
| GPIO 21  | RST (可选) |
| GPIO 40  | BLK (背光) |

## 示例代码

```c
#include "esp32_mipi_lvgl.h"

void app_main(void) {
    // 初始化 MIPI LCD
    esp32_mipi_init();

    // 初始化 LVGL
    lv_init();

    // 创建 UI
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Hello ESP32-P4!");
    lv_obj_center(label);

    // 主循环
    while (1) {
        lv_task_handler();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
```

## 配置选项

在 `sdkconfig` 中配置：

```yaml
CONFIG_ESP32P4_DSI_HOST=y
CONFIG_LVGL_9=y
```

## 许可证

MIT License
