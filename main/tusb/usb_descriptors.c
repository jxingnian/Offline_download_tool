/*
 * MIT 许可证 (MIT)
 *
 * 版权所有 (c) 2019 Ha Thach (tinyusb.org)
 * 版权所有 (c) 2020 Jerzy Kasenbreg
 *
 * 特此免费授予任何获得本软件副本和相关文档文件（下称"软件"）的人不受限制地处置该软件的权利，
 * 包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，
 * 以及再授权被配发了本软件的人如上的权利，须在下列条件下：
 *
 * 上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。
 *
 * 本软件是"如此"提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。
 * 在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，
 * 还是产生于、源于或有关于本软件以及本软件的使用或其它处置。
 *
 */

#include <stdint.h>
#include "tusb.h"
#include "usb_descriptors.h"

//--------------------------------------------------------------------+
// 设备描述符
//--------------------------------------------------------------------+
tusb_desc_device_t const desc_device = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,
    .bDeviceClass       = TUSB_CLASS_MISC,
    .bDeviceSubClass    = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol    = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor           = 0x0D28,    // ARM 的 VID
    .idProduct          = 0x0204,    // DAPLink 的 PID
    .bcdDevice          = 0x0100,
    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,
    .bNumConfigurations = 0x01
};

//--------------------------------------------------------------------+
// 配置描述符
//--------------------------------------------------------------------+

#define CONFIG_TOTAL_LEN    (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN + TUD_HID_DESC_LEN + TUD_MSC_DESC_LEN)

#define EPNUM_CDC_NOTIF   0x81
#define EPNUM_CDC_OUT    0x02
#define EPNUM_CDC_IN     0x82
#define EPNUM_HID        0x83
#define EPNUM_MSC_OUT    0x04
#define EPNUM_MSC_IN     0x84

uint8_t const desc_configuration[] = {
    // 配置描述符
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),

    // CDC 描述符
    TUD_CDC_DESCRIPTOR(ITF_NUM_CDC, 4, EPNUM_CDC_NOTIF, 8, EPNUM_CDC_OUT, EPNUM_CDC_IN, 64),

    // HID 描述符（CMSIS-DAP）
    TUD_HID_DESCRIPTOR(ITF_NUM_HID, 5, HID_ITF_PROTOCOL_NONE, 64, EPNUM_HID, CFG_TUD_HID_EP_BUFSIZE, 1),

    // MSC 描述符
    TUD_MSC_DESCRIPTOR(ITF_NUM_MSC, 6, EPNUM_MSC_OUT, EPNUM_MSC_IN, 64)
};

//--------------------------------------------------------------------+
// 字符串描述符
//--------------------------------------------------------------------+

// 字符串描述符数组
char const *string_desc_arr [] = {
    (const char[]) { 0x09, 0x04 },  // 0: 支持的语言是英语 (0x0409)
    "ARM",                          // 1: 制造商
    "DAPLink CMSIS-DAP",           // 2: 产品
    "123456",                      // 3: 序列号
    "CDC Serial Port",             // 4: CDC 接口
    "CMSIS-DAP HID",              // 5: HID 接口
    "Mass Storage"                 // 6: MSC 接口
};

static uint16_t _desc_str[32];

// 当收到获取字符串描述符请求时调用
// 应用程序返回描述符的指针，其内容必须存在足够长的时间以完成传输
uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
    (void) langid;
    uint8_t chr_count;

    if (index == 0) {
        memcpy(&_desc_str[1], string_desc_arr[0], 2);
        chr_count = 1;
    } else {
        // 检查索引是否超出范围
        if (index >= sizeof(string_desc_arr) / sizeof(string_desc_arr[0])) {
            return NULL;
        }

        const char *str = string_desc_arr[index];

        // 限制最大字符数
        chr_count = (uint8_t) strlen(str);
        if (chr_count > 31) {
            chr_count = 31;
        }

        // 将ASCII字符转换为UTF-16
        for (uint8_t i = 0; i < chr_count; i++) {
            _desc_str[1 + i] = str[i];
        }
    }

    // 第一个字节是长度（包括头部），第二个字节是字符串类型
    _desc_str[0] = (uint16_t)((TUSB_DESC_STRING << 8) | (2 * chr_count + 2));

    return _desc_str;
}

// 当收到获取设备描述符请求时调用
// 应用程序返回描述符的指针
uint8_t const *tud_descriptor_device_cb(void)
{
    return (uint8_t const *)&desc_device;
}

// 当收到获取配置描述符请求时调用
// 应用程序返回描述符的指针
// 描述符内容必须存在足够长的时间以完成传输
uint8_t const *tud_descriptor_configuration_cb(uint8_t index)
{
    (void)index; // 用于多配置情况
    return desc_configuration;
}