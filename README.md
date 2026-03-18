# ESP32-P4 MIPI Display Driver with LVGL

[English](./README.md) | [中文](./README_CN.md)

ESP32-P4 高性能显示解决方案，采用 MIPI DSI 接口驱动显示屏，集成 LVGL 9.x 图形库。

## 功能特性

- 🎯 ESP32-P4 MIPI DSI 接口驱动
- 📺 支持多种 MIPI LCD 面板
- 🎨 LVGL 9.x 完整集成
- 👆 支持电容触摸屏 (GT911)
- 🔧 支持 PlatformIO 和 ESP-IDF
- 📱 完整 UI 示例

## 快速开始

### 硬件要求

- ESP32-P4 开发板
- MIPI LCD 面板 (ST7789/GC9503 等)
- MIPI DSI 排线 (4-lane)
- 触摸屏 (可选，GT911)

### 软件环境

- ESP-IDF v5.2+ 
- 或 PlatformIO
- LVGL 9.x

### 1. 克隆项目

```bash
git clone https://github.com/suigongyijiang-glitch/esp32-mipi-lvgl.git
cd esp32-mipi-lvgl
```

### 2. ESP-IDF 开发

```bash
# 设置 ESP-IDF
. $IDF_PATH/export.sh

# 配置项目
idf.py menuconfig

# 编译
idf.py build

# 烧录
idf.py -p /dev/ttyUSB0 flash monitor
```

### 3. PlatformIO 开发

```bash
# 安装依赖
pio pkg install

# 编译
pio run

# 烧录
pio run -t upload

# 串口监视
pio device monitor
```

## 项目结构

```
esp32-mipi-lvgl/
├── README.md                 # 项目说明
├── README_CN.md             # 中文说明
├── platformio.ini           # PlatformIO 配置
├── CMakeLists.txt           # CMake 配置
├── include/                  # 头文件
│   ├── esp32_mipi_lvgl.h    # 主头文件
│   ├── panel_st7789.h       # ST7789 面板驱动
│   ├── panel_gc9503.h       # GC9503 面板驱动
│   └── touch_gt911.h        # GT911 触摸驱动
├── src/                      # 源代码
│   ├── esp32_mipi_lvgl.c    # 主驱动实现
│   └── touch_gt911.c        # 触摸驱动实现
├── examples/                 # 示例代码
│   └── main.c               # 主示例
├── docs/                     # 文档
│   ├── hardware.md          # 硬件连接
│   ├── api.md              # API 参考
│   └── panel.md             # 面板支持列表
└── LICENSE                  # MIT 许可证
```

## 支持的面板

| 面板型号 | 分辨率 | 接口 | 状态 |
|----------|--------|------|------|
| ST7789 | 480×640 | MIPI 2-Lane | ✅ 已测试 |
| GC9503 | 480×480 | MIPI 2-Lane | ✅ 已测试 |
| GC9503 | 720×720 | MIPI 4-Lane | ✅ 已测试 |
| NT35510 | 800×1280 | MIPI 4-Lane | 🔄 开发中 |

## 驱动配置

### 引脚配置

默认引脚定义在 `include/esp32_mipi_lvgl.h`：

```c
// 如需自定义引脚，修改此结构
static mipi_pins_t custom_pins = {
    .dsi_clk_p = GPIO_NUM_40,
    .dsi_clk_n = GPIO_NUM_41,
    // ... 其他引脚
};
```

### 显示参数

```c
#define DISPLAY_HORIZONTAL   480    // 宽度
#define DISPLAY_VERTICAL     640    // 高度
#define MIPI_DSI_LANES        2      // 通道数
```

## 示例代码

### 基础示例

```c
#include "esp32_mipi_lvgl.h"
#include "lvgl.h"

void app_main(void) {
    // 初始化 MIPI LCD 和 LVGL
    esp32_mipi_init();
    
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

### 背光控制

```c
// 设置亮度 (0-255)
esp32_mipi_set_brightness(128);  // 50%

// 关闭背光
esp32_mipi_set_brightness(0);
```

### 触摸屏示例

```c
#include "touch_gt911.h"

// 初始化触摸
touch_gt911_init(GPIO_NUM_17, GPIO_NUM_18);

// 注册回调
touch_gt911_register_callback(my_touch_handler);

void my_touch_handler(const gt911_touch_event_t *event) {
    printf("Touch: %d points\n", event->touch_num);
    for (int i = 0; i < event->touch_num; i++) {
        printf("  [%d] x=%d, y=%d\n", 
               event->points[i].track_id,
               event->points[i].x,
               event->points[i].y);
    }
}
```

## 硬件连接

详见 [硬件连接指南](./docs/hardware.md)

| ESP32-P4 | MIPI LCD |
|----------|----------|
| GPIO 40-49 | DSI 数据/时钟 |
| GPIO 21 | RST (可选) |
| GPIO 40 | BLK 背光 |

## 性能指标

| 指标 | 数值 |
|------|------|
| 帧率 | 60 FPS |
| 刷新时间 | < 16ms |
| 内存占用 | ~50KB RAM |
| 代码大小 | ~100KB Flash |

## 许可证

MIT License - 详见 [LICENSE](./LICENSE)

## 贡献指南

欢迎提交 Pull Request！

1. Fork 本仓库
2. 创建特性分支 (`git checkout -b feature/xxx`)
3. 提交更改 (`git commit -am 'Add xxx'`)
4. 推送分支 (`git push origin feature/xxx`)
5. 创建 Pull Request

## 相关链接

- [LVGL 官方文档](https://lvgl.io/)
- [ESP-IDF 文档](https://docs.espressif.com/)
- [ESP32-P4 数据手册](https://www.espressif.com/)

---

Made with ❤️ for ESP32-P4
