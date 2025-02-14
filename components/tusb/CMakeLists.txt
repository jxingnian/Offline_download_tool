# 注册组件，指定源文件、包含目录和依赖项
idf_component_register(SRCS "usb_descriptors.c"
                    INCLUDE_DIRS "."
                    REQUIRES "espressif__tinyusb"
                    PRIV_REQUIRES "dap"
)

# 获取 tinyusb 库的引用
idf_component_get_property(tusb_lib espressif__tinyusb COMPONENT_LIB)

# 为 tinyusb 库添加公共包含目录
target_include_directories(${tusb_lib} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# 为 tinyusb 库添加额外的源文件
target_sources(${tusb_lib} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/usb_descriptors.c")
