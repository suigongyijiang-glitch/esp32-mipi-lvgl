cmake_minimum_required(VERSION 3.5)

project("esp32-mipi-lvgl")

# 设置 ESP-IDF
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# 获取目标芯片
idf_build_get_property(target IDF_TARGET)

# 编译选项
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

# 添加组件目录
set(EXTRA_COMPONENT_DIRS "")

# 源文件列表
set(COMPONENT_SRCS
    "src/esp32_mipi_lvgl.c"
    "src/lvgl_driver.c"
    "src/ui_components.c"
    "src/touch_gt911.c"
)

# 头文件目录
set(COMPONENT_INCLUDEDIRS
    "include"
    "${IDF_PATH}/components/lvgl/lvgl"
)

# 注册组件
register_component()

# LVGL 配置头文件
target_compile_definitions(${COMPONENT_LIB} PUBLIC
    LVGL_9=1
    CONFIG_LVGL_COLOR_DEPTH_16=1
)
