idf_component_register(SRCS "Offline_download_tool.c"
                    INCLUDE_DIRS "."
                    REQUIRES "led")

# espressif__tinyusb 应匹配当前依赖的 tinyusb 名称
idf_component_get_property(tusb_lib espressif__tinyusb COMPONENT_LIB)

target_include_directories(${tusb_lib} PUBLIC "${COMPONENT_DIR}/tusb")
target_sources(${tusb_lib} PUBLIC "${COMPONENT_DIR}/tusb/usb_descriptors.c")