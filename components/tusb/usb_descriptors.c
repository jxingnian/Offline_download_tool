/*
 * @Author: xingnina j_xingnian@163.com
 * @Date: 2025-02-14 08:53:11
 * @LastEditors: xingnina j_xingnian@163.com
 * @LastEditTime: 2025-02-15 17:21:44
 * @FilePath: \Offline_download_tool\components\tusb\usb_descriptors.c
 */
#include "tusb.h"
#include "device/usbd.h"
#include "class/cdc/cdc.h"
#include "usb_descriptors.h"
#include "tinyusb.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

static const char *TAG = "USB";

// 字符串描述符索引
enum {
    STRID_LANGID = 0,
    STRID_MANUFACTURER,
    STRID_PRODUCT,
    STRID_SERIAL,
    STRID_CDC,
    STRID_VENDOR,
    STRID_NUM
};

// CDC端点
#define EPNUM_CDC_NOTIF   0x81  // CDC通知端点
#define EPNUM_CDC_OUT     0x02  // CDC输出端点
#define EPNUM_CDC_IN      0x82  // CDC输入端点

// VENDOR端点 (CMSIS-DAP v2)
#define EPNUM_VENDOR_OUT   0x03  // VENDOR输出端点
#define EPNUM_VENDOR_IN    0x83  // VENDOR输入端点

// 配置描述符总长度
#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN + TUD_VENDOR_DESC_LEN)

// 设备描述符
static const tusb_desc_device_t desc_device = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,           // USB 2.0
    .bDeviceClass      = 0x00,              // 由接口定义
    .bDeviceSubClass   = 0x00,
    .bDeviceProtocol   = 0x00,
    .bMaxPacketSize0   = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor          = 0x0D28,           // ARM VID
    .idProduct         = 0x0204,           // CMSIS-DAP v2 PID
    .bcdDevice         = 0x0100,           // 版本 1.0
    .iManufacturer     = STRID_MANUFACTURER,
    .iProduct          = STRID_PRODUCT,
    .iSerialNumber     = STRID_SERIAL,
    .bNumConfigurations = 0x01
};

// 配置描述符
static const uint8_t desc_configuration[] = {
    // 配置描述符
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 
                         0x80, 100),  // 总线供电

    // CDC描述符（包含两个接口：控制和数据）
    TUD_CDC_DESCRIPTOR(ITF_NUM_CDC_CTRL, STRID_CDC, EPNUM_CDC_NOTIF, 
                      8, EPNUM_CDC_OUT, EPNUM_CDC_IN, 64),

    // VENDOR描述符（CMSIS-DAP v2）
    TUD_VENDOR_DESCRIPTOR(ITF_NUM_VENDOR, STRID_VENDOR, EPNUM_VENDOR_OUT, 
                         EPNUM_VENDOR_IN, 64)
};

// 字符串描述符
static const char* string_descriptor[] = {
    [STRID_LANGID] = (const char[]){0x09, 0x04},  // 支持的语言ID（英语）
    [STRID_MANUFACTURER] = "XING",
    [STRID_PRODUCT] = "CMSIS-DAP v2",
    [STRID_SERIAL] = "123456",
    [STRID_CDC] = "CDC Serial",
    [STRID_VENDOR] = "CMSIS-DAP v2 Interface"
};

// 导出描述符配置
tusb_desc_device_t* get_tusb_desc_device(void)
{
    return (tusb_desc_device_t*)&desc_device;
}

const uint8_t* get_tusb_desc_configuration(void)
{
    return desc_configuration;
}

const char** get_tusb_string_descriptor(void)
{
    return (const char**)string_descriptor;
}

size_t get_tusb_string_descriptor_count(void)
{
    return STRID_NUM;
}

// CDC事件回调
void tud_cdc_line_coding_cb(uint8_t itf, cdc_line_coding_t const* p_line_coding)
{
    (void)itf;
    (void)p_line_coding;
}

void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts)
{
    (void)itf;
    (void)dtr;
    (void)rts;
}

#ifdef CONFIG_TINYUSB_VENDOR_ENABLED
// BOS描述符回调函数
uint8_t const *tud_descriptor_bos_cb(void) __attribute__((weak));
uint8_t const *tud_descriptor_bos_cb(void)
{
    // BOS描述符（包含WebUSB和Microsoft OS 2.0描述符）
    static uint8_t const desc_bos[] = {
        // BOS描述符头部
        0x05, // bLength
        0x0F, // bDescriptorType (Binary Device Object Store)
        U16_TO_U8S_LE(BOS_TOTAL_LEN), // wTotalLength
        0x02, // bNumDeviceCaps

        // WebUSB平台能力描述符
        0x18, // bLength
        0x10, // bDescriptorType (Device Capability)
        0x05, // bDevCapabilityType (Platform)
        0x00, // bReserved
        // WebUSB平台能力UUID
        0x38, 0xB6, 0x08, 0x34, 0xA9, 0x09, 0xA0, 0x47,
        0x8B, 0xFD, 0xA0, 0x76, 0x88, 0x15, 0xB6, 0x65,
        U16_TO_U8S_LE(0x0100), // bcdVersion
        VENDOR_REQUEST_WEBUSB, // bVendorCode
        0x01, // iLandingPage

        // Microsoft OS 2.0平台能力描述符
        0x1C, // bLength
        0x10, // bDescriptorType (Device Capability)
        0x05, // bDevCapabilityType (Platform)
        0x00, // bReserved
        // MS OS 2.0平台能力UUID
        0xDF, 0x60, 0xDD, 0xD8, 0x89, 0x45, 0xC7, 0x4C,
        0x9C, 0xD2, 0x65, 0x9D, 0x9E, 0x64, 0x8A, 0x9F,
        U16_TO_U8S_LE(0x0100), // bcdVersion
        U16_TO_U8S_LE(MS_OS_20_DESC_LEN), // wLength
        VENDOR_REQUEST_MICROSOFT, // bVendorCode
        0x00  // bAltEnumCode
    };
    return desc_bos;
}

// 获取Microsoft OS 2.0描述符
uint8_t *get_ms_descriptor(void)
{
    static uint8_t desc_ms_os_20[] = {
        // 设置头部：长度、类型、Windows版本、总长度
        U16_TO_U8S_LE(0x000A), U16_TO_U8S_LE(MS_OS_20_SET_HEADER_DESCRIPTOR), U32_TO_U8S_LE(0x06030000), U16_TO_U8S_LE(MS_OS_20_DESC_LEN),
        // 配置子集头部：长度、类型、配置索引、保留、配置总长度
        U16_TO_U8S_LE(0x0008), U16_TO_U8S_LE(MS_OS_20_SUBSET_HEADER_CONFIGURATION), 0, 0, U16_TO_U8S_LE(MS_OS_20_DESC_LEN - 0x0A),
        // 功能子集头部：长度、类型、第一个接口、保留、子集长度
        U16_TO_U8S_LE(0x0008), U16_TO_U8S_LE(MS_OS_20_SUBSET_HEADER_FUNCTION), ITF_NUM_VENDOR, 0, U16_TO_U8S_LE(MS_OS_20_DESC_LEN - 0x0A - 0x08),
        // MS OS 2.0兼容ID描述符：长度、类型、兼容ID、子兼容ID
        U16_TO_U8S_LE(0x0014), U16_TO_U8S_LE(MS_OS_20_FEATURE_COMPATBLE_ID), 'W', 'I', 'N', 'U', 'S', 'B', 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        // MS OS 2.0注册表属性描述符：长度、类型
        U16_TO_U8S_LE(MS_OS_20_DESC_LEN - 0x0A - 0x08 - 0x08 - 0x14), U16_TO_U8S_LE(MS_OS_20_FEATURE_REG_PROPERTY),
        U16_TO_U8S_LE(0x0007), U16_TO_U8S_LE(0x002A),
        'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00, 'c', 0x00, 'e', 0x00, 'I', 0x00, 'n', 0x00, 't', 0x00, 'e', 0x00,
        'r', 0x00, 'f', 0x00, 'a', 0x00, 'c', 0x00, 'e', 0x00, 'G', 0x00, 'U', 0x00, 'I', 0x00, 'D', 0x00, 's', 0x00, 0x00, 0x00,
        U16_TO_U8S_LE(0x0050),
        '{', 0x00, 'C', 0x00, 'D', 0x00, 'B', 0x00, '3', 0x00, 'B', 0x00, '5', 0x00, 'A', 0x00, 'D', 0x00, '-', 0x00,
        '2', 0x00, '9', 0x00, '3', 0x00, 'B', 0x00, '-', 0x00, '4', 0x00, '6', 0x00, '6', 0x00, '3', 0x00, '-', 0x00,
        'A', 0x00, 'A', 0x00, '3', 0x00, '6', 0x00, '-', 0x00, '1', 0x00, 'A', 0x00, 'A', 0x00, 'E', 0x00, '4', 0x00,
        '6', 0x00, '4', 0x00, '6', 0x00, '3', 0x00, '7', 0x00, '7', 0x00, '6', 0x00, '}', 0x00, 0x00, 0x00, 0x00, 0x00
    };

    return desc_ms_os_20;
}

// VENDOR类控制请求回调
bool tud_vendor_control_xfer_cb(uint8_t rhport, uint8_t stage, tusb_control_request_t const * request)
{
    // 处理Microsoft OS 2.0描述符请求
    if (request->bmRequestType_bit.type == TUSB_REQ_TYPE_VENDOR)
    {
        switch (request->bRequest)
        {
            case VENDOR_REQUEST_MICROSOFT:
                if (request->wIndex == 7)
                {
                    // Get Microsoft OS 2.0 Descriptor
                    uint8_t const * desc = get_ms_descriptor();
                    return tud_control_xfer(rhport, request, (void*)desc, MS_OS_20_DESC_LEN);
                }
                break;

            case VENDOR_REQUEST_WEBUSB:
                // Get WebUSB URL Descriptor
                return false;  // 不支持WebUSB
        }
    }

    // 不支持的请求
    return false;
}

#endif
