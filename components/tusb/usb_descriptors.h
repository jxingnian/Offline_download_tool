/*
 * @Author: xingnina j_xingnian@163.com
 * @Date: 2025-02-14 08:53:11
 * @LastEditors: xingnina j_xingnian@163.com
 * @LastEditTime: 2025-02-14 16:51:35
 * @FilePath: \Offline_download_tool\components\tusb\usb_descriptors.h
 */


#ifndef USB_DESCRIPTORS_H_
#define USB_DESCRIPTORS_H_

#include "tinyusb.h"

// 定义接口编号枚举
enum
{
    ITF_NUM_CDC_CTRL = 0,     // CDC控制接口编号
    ITF_NUM_CDC_DATA,         // CDC数据接口编号
    ITF_NUM_VENDOR,           // 厂商特定接口编号
    ITF_NUM_TOTAL            // 接口总数
};

// 定义厂商请求类型枚举
enum
{
    VENDOR_REQUEST_WEBUSB = 1,    // WebUSB请求
    VENDOR_REQUEST_MICROSOFT = 2  // Microsoft OS描述符请求
};

// 获取设备描述符
tusb_desc_device_t* get_tusb_desc_device(void);

// 获取配置描述符
const uint8_t* get_tusb_desc_configuration(void);

// 获取字符串描述符
const char** get_tusb_string_descriptor(void);

// 获取字符串描述符数量
size_t get_tusb_string_descriptor_count(void);

#endif /* USB_DESCRIPTORS_H_ */
