# 面板支持列表

本文档列出所有支持的 MIPI LCD 面板及其配置参数。

## 已支持面板

### ST7789

- **分辨率**: 480×640
- **接口**: MIPI DSI 2-Lane
- **刷新率**: 60Hz
- **颜色**: 16.7M (24-bit)
- **状态**: ✅ 已测试

**初始化命令**:
```c
// 退出睡眠模式
0x01, 1, 0x00, 0x00
// 帧率设置
0xB2, 5, 0x00, 0x1E, 0x00, 0x04, 0x04
// ... 更多命令见 panel_st7789.h
```

---

### GC9503

- **分辨率**: 480×480 / 720×720
- **接口**: MIPI DSI 2-Lane / 4-Lane
- **刷新率**: 60Hz
- **颜色**: 16.7M (24-bit)
- **状态**: ✅ 已测试

**应用场景**: 智能手表、圆形屏

---

## 开发中面板

### NT35510

- **分辨率**: 800×1280
- **接口**: MIPI DSI 4-Lane
- **刷新率**: 60Hz
- **预计状态**: 2026 Q2

---

### R69429

- **分辨率**: 1200×1920 (10.1")
- **接口**: MIPI DSI 4-Lane
- **刷新率**: 60Hz
- **预计状态**: 2026 Q2

---

## 面板参数对照表

| 型号 | 宽 | 高 | Lane | 时钟(MHz) | RAM | 状态 |
|------|----|----|------|-----------|-----|------|
| ST7789 | 480 | 640 | 2 | 50 | 40KB | ✅ |
| GC9503 | 480 | 480 | 2 | 60 | 40KB | ✅ |
| GC9503 | 720 | 720 | 4 | 80 | 60KB | ✅ |
| NT35510 | 800 | 1280 | 4 | 100 | 80KB | 🔄 |
| R69429 | 1200 | 1920 | 4 | 150 | 120KB | 🔄 |

---

## 如何添加新面板

### 步骤 1: 创建面板头文件

在 `include/` 目录创建 `panel_xxx.h`:

```c
#ifndef PANEL_XXX_H
#define PANEL_XXX_H

#include "esp32_mipi_lvgl.h"

// 初始化命令序列
static const uint8_t xxx_init_cmds[] = {
    // 命令格式: [命令字节数, 参数...]
    0x01, 1, 0x00, 0x00,  // 命令 + 1 参数
    0xFF, 3, 0x01, 0x02, 0x03,  // 命令 + 3 参数
    0x00  // 结束标志
};

// 面板配置
static const mipi_panel_config_t xxx_panel = {
    .width = 480,
    .height = 640,
    .lanes = 2,
    .pixel_clock = 50000000,
    .init_cmds = (uint8_t *)xxx_init_cmds,
    .init_cmds_size = sizeof(xxx_init_cmds),
};

esp_err_t panel_xxx_init(void);

#endif
```

### 步骤 2: 实现初始化函数

在 `src/` 目录创建 `panel_xxx.c`:

```c
#include "panel_xxx.h"

esp_err_t panel_xxx_init(void)
{
    // 实现面板特定初始化
    // ...
    return ESP_OK;
}
```

### 步骤 3: 在 CMakeLists.txt 添加

```cmake
idf_build_get_property(target IDF_TARGET)
if(${target} STREQUAL "esp32p4")
    target_sources(${COMPONENT_LIB} PRIVATE
        src/panel_xxx.c
    )
endif()
```

---

## 常见面板问题排查

### 屏幕闪烁

- 检查 MIPI 时钟频率
- 检查电源稳定性

### 颜色异常

- 确认像素格式 (RGB/BGR)
- 检查初始化命令顺序

### 显示不全

- 检查分辨率设置
- 确认显示区域配置
