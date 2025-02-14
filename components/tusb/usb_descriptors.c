/*
 * @Author: xingnina j_xingnian@163.com
 * @Date: 2025-02-14 08:53:11
 * @LastEditors: xingnina j_xingnian@163.com
 * @LastEditTime: 2025-02-14 17:53:44
 * @FilePath: \Offline_download_tool\components\tusb\usb_descriptors.c
 */
#include "tusb.h"
#include "usb_descriptors.h"
#include "../dap/dap.h"

/* 接口组合必须有一个唯一的产品ID，因为PC会在首次插入后保存设备驱动。
 * 相同的VID/PID但不同接口（例如先MSC，后CDC）可能会导致PC上的系统错误。
 */
#define _PID_MAP(itf, n)  ( (CFG_TUD_##itf) << (n) )
#define USB_PID           (0x4000 | _PID_MAP(HID, 0) )

#define USB_VID   0xCafe  // 供应商ID
#define USB_BCD   0x0200  // USB规范版本号

//--------------------------------------------------------------------+
// 设备描述符
//--------------------------------------------------------------------+
tusb_desc_device_t const desc_device =
{
    .bLength            = sizeof(tusb_desc_device_t),  // 描述符长度
    .bDescriptorType    = TUSB_DESC_DEVICE,            // 描述符类型（设备）
    .bcdUSB             = USB_BCD,                     // USB规范版本
    .bDeviceClass       = 0x00,                        // 设备类别（由接口定义）
    .bDeviceSubClass    = 0x00,                        // 设备子类
    .bDeviceProtocol    = 0x00,                        // 设备协议
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,      // 端点0最大包大小

    .idVendor           = USB_VID,                     // 供应商ID
    .idProduct          = USB_PID,                     // 产品ID
    .bcdDevice          = 0x0100,                      // 设备版本号

    .iManufacturer      = 0x01,                        // 制造商字符串索引
    .iProduct           = 0x02,                        // 产品字符串索引
    .iSerialNumber      = 0x03,                        // 序列号字符串索引

    .bNumConfigurations = 0x01                         // 配置数量
};

// 当收到GET DEVICE DESCRIPTOR请求时调用
uint8_t const * tud_descriptor_device_cb(void)
{
    return (uint8_t const *) &desc_device;
}

//--------------------------------------------------------------------+
// HID报告描述符
//--------------------------------------------------------------------+
uint8_t const desc_hid_report[] =
{
    // CMSIS-DAP v2 使用通用 HID 接口
    0x06, 0x00, 0xFF,  // Usage Page (Vendor Defined 0xFF00)
    0x09, 0x01,        // Usage (0x01)
    0xA1, 0x01,        // Collection (Application)
    0x09, 0x01,        //   Usage (0x01)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x75, 0x08,        //   Report Size (8)
    0x95, 0x40,        //   Report Count (64)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x01,        //   Usage (0x01)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x75, 0x08,        //   Report Size (8)
    0x95, 0x40,        //   Report Count (64)
    0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0               // End Collection
};

// 当收到GET HID REPORT DESCRIPTOR请求时调用
uint8_t const * tud_hid_descriptor_report_cb(uint8_t instance)
{
    (void) instance;
    return desc_hid_report;
}

//--------------------------------------------------------------------+
// 配置描述符
//--------------------------------------------------------------------+
#define CONFIG_TOTAL_LEN    (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)
#define EPNUM_HID   0x81

uint8_t const desc_configuration[] =
{
    // 配置号, 接口数, 字符串索引, 总长度, 属性, 功耗(mA)
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),

    // 接口号, 字符串索引, 协议, 报告描述符长度, 输入端点地址, 大小和轮询间隔
    TUD_HID_DESCRIPTOR(ITF_NUM_HID, 0, HID_ITF_PROTOCOL_NONE, sizeof(desc_hid_report), EPNUM_HID,
                      CFG_TUD_HID_EP_BUFSIZE, 5)
};

// 当收到GET CONFIGURATION DESCRIPTOR请求时调用
uint8_t const * tud_descriptor_configuration_cb(uint8_t index)
{
    (void) index;
    return desc_configuration;
}

//--------------------------------------------------------------------+
// HID报告回调
//--------------------------------------------------------------------+

// 当收到GET_REPORT控制请求时调用
// 应用程序必须填充缓冲区报告的内容并返回其长度
// 返回零将导致堆栈STALL请求
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

    return 0;
}

// 当收到SET_REPORT控制请求或在OUT端点上接收到数据时调用（Report ID = 0, Type = 0）
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    (void) instance;
    (void) report_id;
    (void) report_type;

    // 处理 CMSIS-DAP 命令
    if (bufsize > 0)
    {
        uint8_t response[CFG_TUD_HID_EP_BUFSIZE];
        uint16_t response_len;

        // 处理 DAP 命令
        if (dap_process_command((uint8_t*)buffer, bufsize, response, &response_len) == ESP_OK)
        {
            // 发送响应
            if (tud_hid_ready())
            {
                tud_hid_report(0, response, response_len);
            }
        }
    }
}

//--------------------------------------------------------------------+
// 字符串描述符
//--------------------------------------------------------------------+
// 字符串描述符数组
char const* string_desc_arr [] =
{
    (const char[]) { 0x09, 0x04 }, // 0: 支持的语言是英语 (0x0409)
    "TinyUSB",                     // 1: 制造商
    "TinyUSB Device",              // 2: 产品
    "123456",                      // 3: 序列号
};

static uint16_t _desc_str[32];

// 当收到GET STRING DESCRIPTOR请求时调用
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
    (void) langid;
    uint8_t chr_count;

    if ( index == 0)
    {
        memcpy(&_desc_str[1], string_desc_arr[0], 2);
        chr_count = 1;
    }
    else
    {
        // 将ASCII字符串转换为UTF-16
        if ( !(index < sizeof(string_desc_arr)/sizeof(string_desc_arr[0])) ) return NULL;

        const char* str = string_desc_arr[index];

        // 限制最大字符数
        chr_count = strlen(str);
        if ( chr_count > 31 ) chr_count = 31;

        for(uint8_t i=0; i<chr_count; i++)
        {
            _desc_str[1+i] = str[i];
        }
    }

    // 第一个字节是长度（包括头），第二个字节是字符串类型
    _desc_str[0] = (TUSB_DESC_STRING << 8 ) | (2*chr_count + 2);

    return _desc_str;
}
