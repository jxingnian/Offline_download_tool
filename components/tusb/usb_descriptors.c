/*
 * @Author: xingnina j_xingnian@163.com
 * @Date: 2025-02-14 08:53:11
 * @LastEditors: xingnina j_xingnian@163.com
 * @LastEditTime: 2025-02-15 17:21:44
 * @FilePath: \Offline_download_tool\components\tusb\usb_descriptors.c
 */
#include "tusb.h"
#include "usb_descriptors.h"
#include "class/hid/hid_device.h"
#include "tinyusb.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
// #include "freertos/ringbuf.h"
// #include "dap_handle.h"

static const char *TAG = "USB";

/************* TinyUSB 描述符 ****************/

// 定义USB描述符的总长度
#define TUSB_DESC_TOTAL_LEN      (TUD_CONFIG_DESC_LEN + CFG_TUD_HID * TUD_HID_DESC_LEN)

/**
 * @brief HID报告描述符
 *
 * 实现CMSIS-DAP HID设备
 * 参考CMSIS-DAP v2.0规范
 */
const uint8_t hid_report_descriptor[] = {
    0x06, 0x00, 0xFF,  // Usage Page (Vendor Defined 0xFF00)
    0x09, 0x01,        // Usage (0x01)
    0xA1, 0x01,        // Collection (Application)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x75, 0x08,        //   Report Size (8)
    0x95, 0x40,        //   Report Count (64)
    0x09, 0x01,        //   Usage (0x01)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear)
    0x95, 0x40,        //   Report Count (64)
    0x09, 0x01,        //   Usage (0x01)
    0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear)
    0xC0               // End Collection
};

/**
 * @brief 字符串描述符
 */
const char* hid_string_descriptor[5] = {
    // 字符串描述符指针数组
    (char[]){0x09, 0x04},  // 0: 支持的语言是英语 (0x0409)
    "XING",                // 1: 制造商
    "CMSIS-DAP v2",       // 2: 产品
    "123456",             // 3: 序列号，应该使用芯片ID
    "CMSIS-DAP v2"        // 4: HID接口
};

/**
 * @brief 设备描述符
 * 
 * 自定义设备描述符，使其更符合CMSIS-DAP标准
 */
static const tusb_desc_device_t device_descriptor = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,                  // USB 2.0
    .bDeviceClass = 0x00,              // 每个接口指定类
    .bDeviceSubClass = 0x00,           // 每个接口指定子类
    .bDeviceProtocol = 0x00,           // 每个接口指定协议
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = 0xC251,               // CMSIS-DAP v2 VID
    .idProduct = 0xF001,              // CMSIS-DAP v2 PID
    .bcdDevice = 0x0100,              // 设备版本 1.0
    .iManufacturer = 0x01,            // 制造商字符串索引
    .iProduct = 0x02,                 // 产品字符串索引
    .iSerialNumber = 0x03,            // 序列号字符串索引
    .bNumConfigurations = 0x01        // 配置数量
};

/**
 * @brief 配置描述符
 * 
 * CMSIS-DAP需要64字节的端点大小和1ms的轮询间隔
 */
static const uint8_t hid_configuration_descriptor[] = {
    // 配置号, 接口数, 字符串索引, 总长度, 属性, 功耗(mA)
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, TUSB_DESC_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

    // 接口号, 字符串索引, 引导协议, 报告描述符长度, EP In地址, 大小和轮询间隔
    TUD_HID_DESCRIPTOR(0, 4, HID_ITF_PROTOCOL_NONE, sizeof(hid_report_descriptor), 0x81, 64, 1),
};

/********* TinyUSB HID 回调函数 ***************/

uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance)
{
    return hid_report_descriptor;
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
    return 0;
}

void tud_hid_init(void) {
    ESP_LOGI(TAG, "USB initialization");
    
    // USB 配置
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = &device_descriptor,
        .string_descriptor = hid_string_descriptor,
        .string_descriptor_count = sizeof(hid_string_descriptor) / sizeof(hid_string_descriptor[0]),
        .external_phy = false,
        .configuration_descriptor = hid_configuration_descriptor,
    };

    // 初始化 USB 设备
    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
    ESP_LOGI(TAG, "USB initialization DONE");
}
