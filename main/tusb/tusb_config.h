/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Jerzy Kasenbreg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// 包含必要的头文件
#include "sdkconfig.h"

//--------------------------------------------------------------------+
// 板级特定配置
//--------------------------------------------------------------------+

// 设备使用的RHPort号可以由board.mk定义，默认为端口0
#ifdef CONFIG_TINYUSB_RHPORT_HS
    // 如果定义了高速端口，使用高速模式
#   define CFG_TUSB_RHPORT1_MODE    OPT_MODE_DEVICE | OPT_MODE_HIGH_SPEED
#   define CONFIG_USB_HS            1
#else
    // 否则使用全速模式
#   define CFG_TUSB_RHPORT0_MODE    OPT_MODE_DEVICE | OPT_MODE_FULL_SPEED
#   define CONFIG_USB_HS            0
#endif

//--------------------------------------------------------------------
// 通用配置
//--------------------------------------------------------------------

// 通过编译器标志定义，以提高灵活性
#ifndef CFG_TUSB_MCU
#error CFG_TUSB_MCU必须被定义
#endif

// 定义操作系统，默认为FreeRTOS
#ifndef CFG_TUSB_OS
#define CFG_TUSB_OS           OPT_OS_FREERTOS
#endif

// 定义ESP平台
#ifndef ESP_PLATFORM
#define ESP_PLATFORM 1
#endif

// 设置调试级别
#ifndef CFG_TUSB_DEBUG
#define CFG_TUSB_DEBUG        0
#endif

// 为特定的ESP32系列MCU定义FreeRTOS包含路径
#if TU_CHECK_MCU(OPT_MCU_ESP32S2, OPT_MCU_ESP32S3, OPT_MCU_ESP32P4)
#define CFG_TUSB_OS_INC_PATH    freertos/
#endif

// 启用设备栈
#define CFG_TUD_ENABLED       1

/* 在某些MCU上，USB DMA只能访问特定的SRAM区域，并且有对齐限制。
 * TinyUSB使用以下宏来声明传输内存，以便将其放入特定的区域。
 * 例如：
 * - CFG_TUSB_MEM SECTION : __attribute__ (( section(".usb_ram") ))
 * - CFG_TUSB_MEM_ALIGN   : __attribute__ ((aligned(4)))
 */
#ifndef CFG_TUSB_MEM_SECTION
#define CFG_TUSB_MEM_SECTION
#endif

#ifndef CFG_TUSB_MEM_ALIGN
#define CFG_TUSB_MEM_ALIGN        __attribute__ ((aligned(4)))
#endif

//--------------------------------------------------------------------
// 设备配置
//--------------------------------------------------------------------

// 定义端点0的大小，默认为64字节
#ifndef CFG_TUD_ENDPOINT0_SIZE
#define CFG_TUD_ENDPOINT0_SIZE    64
#endif

//--------------------------------------------------------------------
// DAPLink 设备类配置
//--------------------------------------------------------------------

// 启用 CDC 设备类
#define CFG_TUD_CDC               1
#define CFG_TUD_CDC_RX_BUFSIZE   64
#define CFG_TUD_CDC_TX_BUFSIZE   64

// 启用 HID 设备类（用于 CMSIS-DAP）
#define CFG_TUD_HID              1
#define CFG_TUD_HID_EP_BUFSIZE   64

// 启用 MSC 设备类（用于拖拽烧录）
#define CFG_TUD_MSC              1
#define CFG_TUD_MSC_EP_BUFSIZE   512

// 设备接口数量
#define CFG_TUD_CDC_EP_BUFSIZE   64

#ifdef __cplusplus
}
#endif

#endif /* _TUSB_CONFIG_H_ */