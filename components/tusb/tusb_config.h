/*
 * @Author: xingnina j_xingnian@163.com
 * @Date: 2025-02-14 08:53:11
 * @LastEditors: xingnina j_xingnian@163.com
 * @LastEditTime: 2025-02-14 15:30:58
 * @FilePath: \Offline_download_tool\main\tusb\tusb_config.h
 */
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

//--------------------------------------------------------------------+
// 板级特定配置
//--------------------------------------------------------------------+

// RHPort number used for device can be defined by board.mk, default to port 0
#ifndef BOARD_TUD_RHPORT
#define BOARD_TUD_RHPORT     0
#endif

// Enable Device stack
#define CFG_TUD_ENABLED       1

// RHPort max operational speed can defined by board.mk
#ifndef BOARD_TUD_MAX_SPEED
#define BOARD_TUD_MAX_SPEED   OPT_MODE_DEFAULT_SPEED
#endif

//--------------------------------------------------------------------+
// 设备配置
//--------------------------------------------------------------------+

// 定义端点0的大小，默认为64字节
#ifndef CFG_TUD_ENDPOINT0_SIZE
#define CFG_TUD_ENDPOINT0_SIZE    64
#endif

//--------------------------------------------------------------------+
// HID 设备类配置
//--------------------------------------------------------------------+

// 启用 HID 设备类（用于 CMSIS-DAP）
#define CFG_TUD_HID              1
#define CFG_TUD_HID_EP_BUFSIZE   64

// HID 端点配置
#define CFG_TUD_HID_EP_INTERVAL  1

// CMSIS-DAP v2 需要更大的缓冲区
#ifndef CFG_TUD_HID_EP_BUFSIZE
#define CFG_TUD_HID_EP_BUFSIZE   64  // DAP v2 推荐使用 64 字节
#endif

// 启用 HID 通用输入输出功能
#define CFG_TUD_HID_GENERIC      1

#ifdef __cplusplus
}
#endif

#endif /* _TUSB_CONFIG_H_ */