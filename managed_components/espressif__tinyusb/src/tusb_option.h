/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
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
 * This file is part of the TinyUSB stack.
 */

#ifndef _TUSB_OPTION_H_
#define _TUSB_OPTION_H_

#include "common/tusb_compiler.h"

// 版本号定义，格式为 major.minor.revision，例如 1.0.0
#define TUSB_VERSION_MAJOR     0
#define TUSB_VERSION_MINOR     18
#define TUSB_VERSION_REVISION  0

// 将版本号转换为数字形式，方便比较
#define TUSB_VERSION_NUMBER    (TUSB_VERSION_MAJOR * 10000 + TUSB_VERSION_MINOR * 100 + TUSB_VERSION_REVISION)
// 将版本号转换为字符串形式
#define TUSB_VERSION_STRING    TU_STRING(TUSB_VERSION_MAJOR) "." TU_STRING(TUSB_VERSION_MINOR) "." TU_STRING(TUSB_VERSION_REVISION)

//--------------------------------------------------------------------+
// 支持的微控制器 (MCU)
// CFG_TUSB_MCU 必须定义为以下值之一
//--------------------------------------------------------------------+

#define OPT_MCU_NONE                0

// LPC 系列
#define OPT_MCU_LPC11UXX            1 ///< NXP LPC11Uxx
#define OPT_MCU_LPC13XX             2 ///< NXP LPC13xx
#define OPT_MCU_LPC15XX             3 ///< NXP LPC15xx
#define OPT_MCU_LPC175X_6X          4 ///< NXP LPC175x, LPC176x
#define OPT_MCU_LPC177X_8X          5 ///< NXP LPC177x, LPC178x
#define OPT_MCU_LPC18XX             6 ///< NXP LPC18xx
#define OPT_MCU_LPC40XX             7 ///< NXP LPC40xx
#define OPT_MCU_LPC43XX             8 ///< NXP LPC43xx
#define OPT_MCU_LPC51               9 ///< NXP LPC51
#define OPT_MCU_LPC51UXX            OPT_MCU_LPC51 ///< NXP LPC51
#define OPT_MCU_LPC54              10 ///< NXP LPC54
#define OPT_MCU_LPC55              11 ///< NXP LPC55
// 旧版命名
#define OPT_MCU_LPC54XXX           OPT_MCU_LPC54
#define OPT_MCU_LPC55XX            OPT_MCU_LPC55

// NRF 系列
#define OPT_MCU_NRF5X             100 ///< Nordic nRF5x series

// SAM 系列
#define OPT_MCU_SAMD21            200 ///< MicroChip SAMD21
#define OPT_MCU_SAMD51            201 ///< MicroChip SAMD51
#define OPT_MCU_SAMG              202 ///< MicroChip SAMDG series
#define OPT_MCU_SAME5X            203 ///< MicroChip SAM E5x
#define OPT_MCU_SAMD11            204 ///< MicroChip SAMD11
#define OPT_MCU_SAML22            205 ///< MicroChip SAML22
#define OPT_MCU_SAML21            206 ///< MicroChip SAML21
#define OPT_MCU_SAMX7X            207 ///< MicroChip SAME70, S70, V70, V71 family

// STM32 系列
#define OPT_MCU_STM32F0           300 ///< ST F0
#define OPT_MCU_STM32F1           301 ///< ST F1
#define OPT_MCU_STM32F2           302 ///< ST F2
#define OPT_MCU_STM32F3           303 ///< ST F3
#define OPT_MCU_STM32F4           304 ///< ST F4
#define OPT_MCU_STM32F7           305 ///< ST F7
#define OPT_MCU_STM32H7           306 ///< ST H7
#define OPT_MCU_STM32L1           308 ///< ST L1
#define OPT_MCU_STM32L0           307 ///< ST L0
#define OPT_MCU_STM32L4           309 ///< ST L4
#define OPT_MCU_STM32G0           310 ///< ST G0
#define OPT_MCU_STM32G4           311 ///< ST G4
#define OPT_MCU_STM32WB           312 ///< ST WB
#define OPT_MCU_STM32U5           313 ///< ST U5
#define OPT_MCU_STM32L5           314 ///< ST L5
#define OPT_MCU_STM32H5           315 ///< ST H5
#define OPT_MCU_STM32U0           316 ///< ST U0
#define OPT_MCU_STM32H7RS         317 ///< ST F7RS
#define OPT_MCU_STM32C0           318 ///< ST C0

// Sony
#define OPT_MCU_CXD56             400 ///< SONY CXD56

// TI
#define OPT_MCU_MSP430x5xx        500 ///< TI MSP430x5xx
#define OPT_MCU_MSP432E4          510 ///< TI MSP432E4xx
#define OPT_MCU_TM4C123           511 ///< TI Tiva-C 123x
#define OPT_MCU_TM4C129           512 ///< TI Tiva-C 129x

// ValentyUSB eptri
#define OPT_MCU_VALENTYUSB_EPTRI  600 ///< Fomu eptri config

// NXP iMX RT
#define OPT_MCU_MIMXRT1XXX        700                 ///< NXP iMX RT1xxx Series
#define OPT_MCU_MIMXRT10XX        OPT_MCU_MIMXRT1XXX  ///< RT10xx
#define OPT_MCU_MIMXRT11XX        OPT_MCU_MIMXRT1XXX  ///< RT11xx

// Nuvoton
#define OPT_MCU_NUC121            800
#define OPT_MCU_NUC126            801
#define OPT_MCU_NUC120            802
#define OPT_MCU_NUC505            803

// Espressif
#define OPT_MCU_ESP32S2           900 ///< Espressif ESP32-S2
#define OPT_MCU_ESP32S3           901 ///< Espressif ESP32-S3
#define OPT_MCU_ESP32             902 ///< Espressif ESP32 (for host max3421e)
#define OPT_MCU_ESP32C3           903 ///< Espressif ESP32-C3
#define OPT_MCU_ESP32C6           904 ///< Espressif ESP32-C6
#define OPT_MCU_ESP32C2           905 ///< Espressif ESP32-C2
#define OPT_MCU_ESP32H2           906 ///< Espressif ESP32-H2
#define OPT_MCU_ESP32P4           907 ///< Espressif ESP32-P4
#define TUSB_MCU_VENDOR_ESPRESSIF (CFG_TUSB_MCU >= 900 && CFG_TUSB_MCU < 1000) // 检查是否为 Espressif MCU
#define TUP_MCU_ESPRESSIF        TUSB_MCU_VENDOR_ESPRESSIF //  向后兼容

// Dialog
#define OPT_MCU_DA1469X          1000 ///< Dialog Semiconductor DA1469x

// Raspberry Pi
#define OPT_MCU_RP2040           1100 ///< Raspberry Pi RP2040

// NXP Kinetis
#define OPT_MCU_KINETIS_KL       1200 ///< NXP KL series
#define OPT_MCU_KINETIS_K32L     1201 ///< NXP K32L series
#define OPT_MCU_KINETIS_K32      1201 ///< K32L 的别名
#define OPT_MCU_KINETIS_K        1202 ///< NXP K series

#define OPT_MCU_MKL25ZXX         1200 ///< KL 的别名 (已过时)
#define OPT_MCU_K32L2BXX         1201 ///< K32 的别名 (已过时)

// Silabs
#define OPT_MCU_EFM32GG          1300 ///< Silabs EFM32GG

// Renesas RX
#define OPT_MCU_RX63X            1400 ///< Renesas RX63N/631
#define OPT_MCU_RX65X            1401 ///< Renesas RX65N/RX651
#define OPT_MCU_RX72N            1402 ///< Renesas RX72N
#define OPT_MCU_RAXXX            1403 ///< Renesas RA generic

// Mind Motion
#define OPT_MCU_MM32F327X        1500 ///< Mind Motion MM32F327

// GigaDevice
#define OPT_MCU_GD32VF103        1600 ///< GigaDevice GD32VF103

// Broadcom
#define OPT_MCU_BCM2711          1700 ///< Broadcom BCM2711
#define OPT_MCU_BCM2835          1701 ///< Broadcom BCM2835
#define OPT_MCU_BCM2837          1702 ///< Broadcom BCM2837

// Infineon
#define OPT_MCU_XMC4000          1800 ///< Infineon XMC4000

// PIC
#define OPT_MCU_PIC32MZ          1900 ///< MicroChip PIC32MZ family
#define OPT_MCU_PIC32MM          1901 ///< MicroChip PIC32MM family
#define OPT_MCU_PIC32MX          1902 ///< MicroChip PIC32MX family
#define OPT_MCU_PIC32MK          1903 ///< MicroChip PIC32MK family
#define OPT_MCU_PIC24            1910 ///< MicroChip PIC24 family
#define OPT_MCU_DSPIC33          1911 ///< MicroChip DSPIC33 family

// BridgeTek
#define OPT_MCU_FT90X            2000 ///< BridgeTek FT90x
#define OPT_MCU_FT93X            2001 ///< BridgeTek FT93x

// Allwinner
#define OPT_MCU_F1C100S          2100 ///< Allwinner F1C100s family

// WCH
#define OPT_MCU_CH32V307         2200 ///< WCH CH32V307
#define OPT_MCU_CH32F20X         2210 ///< WCH CH32F20x
#define OPT_MCU_CH32V20X         2220 ///< WCH CH32V20X
#define OPT_MCU_CH32V103         2230 ///< WCH CH32V103

// NXP LPC MCX
#define OPT_MCU_MCXN9            2300  ///< NXP MCX N9 Series
#define OPT_MCU_MCXA15           2301  ///< NXP MCX A15 Series

// Analog Devices
#define OPT_MCU_MAX32690         2400  ///< ADI MAX32690
#define OPT_MCU_MAX32666         2401  ///< ADI MAX32666/5
#define OPT_MCU_MAX32650         2402  ///< ADI MAX32650/1/2
#define OPT_MCU_MAX78002         2403  ///< ADI MAX78002

// 检查配置的 MCU 是否是列表中的一个
// 使用 || 作为分隔符将 _TU_CHECK_MCU 应用于输入列表
#define _TU_CHECK_MCU(_m)    (CFG_TUSB_MCU == _m)
#define TU_CHECK_MCU(...)    (TU_ARGS_APPLY(_TU_CHECK_MCU, ||, __VA_ARGS__))

//--------------------------------------------------------------------+
// 支持的操作系统
//--------------------------------------------------------------------+

#define OPT_OS_NONE       1  ///< 无 RTOS
#define OPT_OS_FREERTOS   2  ///< FreeRTOS
#define OPT_OS_MYNEWT     3  ///< Mynewt OS
#define OPT_OS_CUSTOM     4  ///< 自定义 OS 由应用程序实现
#define OPT_OS_PICO       5  ///< Raspberry Pi Pico SDK
#define OPT_OS_RTTHREAD   6  ///< RT-Thread
#define OPT_OS_RTX4       7  ///< Keil RTX 4

//--------------------------------------------------------------------+
// 模式和速度
//--------------------------------------------------------------------+
// 低字节表示操作模式
#define OPT_MODE_NONE           0x0000 ///< 禁用
#define OPT_MODE_DEVICE         0x0001 ///< 设备模式
#define OPT_MODE_HOST           0x0002 ///< 主机模式

// 高字节表示最大操作速度（对应于tusb_speed_t）
#define OPT_MODE_DEFAULT_SPEED  0x0000 ///< 默认（最大）速度，由MCU支持
#define OPT_MODE_LOW_SPEED      0x0100 ///< 低速
#define OPT_MODE_FULL_SPEED     0x0200 ///< 全速
#define OPT_MODE_HIGH_SPEED     0x0400 ///< 高速
#define OPT_MODE_SPEED_MASK     0xff00 ///< 速度掩码

//--------------------------------------------------------------------+
// 包含 tusb_config.h
//--------------------------------------------------------------------+

// 允许使用命令行更改配置文件名/位置
#ifdef CFG_TUSB_CONFIG_FILE
  #include CFG_TUSB_CONFIG_FILE
#else
  #include "tusb_config.h"
#endif

#include "common/tusb_mcu.h"

//--------------------------------------------------------------------+
// USBIP 配置
//--------------------------------------------------------------------+

// DWC2从机模式配置（用于设备）
#ifndef CFG_TUD_DWC2_SLAVE_ENABLE
  #ifndef CFG_TUD_DWC2_SLAVE_ENABLE_DEFAULT
  #define CFG_TUD_DWC2_SLAVE_ENABLE_DEFAULT 1
  #endif

  #define CFG_TUD_DWC2_SLAVE_ENABLE CFG_TUD_DWC2_SLAVE_ENABLE_DEFAULT
#endif

// 为设备启用DWC2 DMA
#ifndef CFG_TUD_DWC2_DMA_ENABLE
  #ifndef CFG_TUD_DWC2_DMA_ENABLE_DEFAULT
  #define CFG_TUD_DWC2_DMA_ENABLE_DEFAULT 0
  #endif

  #define CFG_TUD_DWC2_DMA_ENABLE CFG_TUD_DWC2_DMA_ENABLE_DEFAULT
#endif

// 为主机启用DWC2从机模式
#ifndef CFG_TUH_DWC2_SLAVE_ENABLE
  #ifndef CFG_TUH_DWC2_SLAVE_ENABLE_DEFAULT
  #define CFG_TUH_DWC2_SLAVE_ENABLE_DEFAULT 1
  #endif

  #define CFG_TUH_DWC2_SLAVE_ENABLE CFG_TUH_DWC2_SLAVE_ENABLE_DEFAULT
#endif

// 为主机启用DWC2 DMA
#ifndef CFG_TUH_DWC2_DMA_ENABLE
  #ifndef CFG_TUH_DWC2_DMA_ENABLE_DEFAULT
  #define CFG_TUH_DWC2_DMA_ENABLE_DEFAULT 0
  #endif

  #define CFG_TUH_DWC2_DMA_ENABLE   CFG_TUH_DWC2_DMA_ENABLE_DEFAULT
#endif

// 启用PIO-USB软件主机控制器
#ifndef CFG_TUH_RPI_PIO_USB
  #define CFG_TUH_RPI_PIO_USB 0
#endif

#ifndef CFG_TUD_RPI_PIO_USB
  #define CFG_TUD_RPI_PIO_USB 0
#endif

// MAX3421主机控制器选项
#ifndef CFG_TUH_MAX3421
  #define CFG_TUH_MAX3421  0
#endif

//--------------------------------------------------------------------
// 根集线器模式检测
//--------------------------------------------------------------------

//------------- 根集线器作为设备 -------------//

#if defined(CFG_TUSB_RHPORT0_MODE) && ((CFG_TUSB_RHPORT0_MODE) & OPT_MODE_DEVICE)
  #define TUD_RHPORT_MODE     (CFG_TUSB_RHPORT0_MODE)
  #define TUD_OPT_RHPORT      0
#elif defined(CFG_TUSB_RHPORT1_MODE) && ((CFG_TUSB_RHPORT1_MODE) & OPT_MODE_DEVICE)
  #define TUD_RHPORT_MODE     (CFG_TUSB_RHPORT1_MODE)
  #define TUD_OPT_RHPORT      1
#else
  #define TUD_RHPORT_MODE     OPT_MODE_NONE
#endif

#ifndef CFG_TUD_ENABLED
  // 回退使用CFG_TUSB_RHPORTx_MODE
  #define CFG_TUD_ENABLED     (TUD_RHPORT_MODE & OPT_MODE_DEVICE)
#endif

#ifndef CFG_TUD_MAX_SPEED
  // 回退使用CFG_TUSB_RHPORTx_MODE
  #define CFG_TUD_MAX_SPEED   (TUD_RHPORT_MODE & OPT_MODE_SPEED_MASK)
#endif

// 向后兼容
#define TUSB_OPT_DEVICE_ENABLED CFG_TUD_ENABLED

// 高速支持指示器
#define TUD_OPT_HIGH_SPEED    (CFG_TUD_MAX_SPEED ? (CFG_TUD_MAX_SPEED & OPT_MODE_HIGH_SPEED) : TUP_RHPORT_HIGHSPEED)

//------------- 根集线器作为主机 -------------//

#if defined(CFG_TUSB_RHPORT0_MODE) && ((CFG_TUSB_RHPORT0_MODE) & OPT_MODE_HOST)
  #define TUH_RHPORT_MODE  (CFG_TUSB_RHPORT0_MODE)
  #define TUH_OPT_RHPORT   0
#elif defined(CFG_TUSB_RHPORT1_MODE) && ((CFG_TUSB_RHPORT1_MODE) & OPT_MODE_HOST)
  #define TUH_RHPORT_MODE  (CFG_TUSB_RHPORT1_MODE)
  #define TUH_OPT_RHPORT   1
#else
  #define TUH_RHPORT_MODE   OPT_MODE_NONE
#endif

#ifndef CFG_TUH_ENABLED
  // 回退使用CFG_TUSB_RHPORTx_MODE
  #define CFG_TUH_ENABLED     (TUH_RHPORT_MODE & OPT_MODE_HOST)
#endif

#ifndef CFG_TUH_MAX_SPEED
  // 回退使用CFG_TUSB_RHPORTx_MODE
  #define CFG_TUH_MAX_SPEED   (TUH_RHPORT_MODE & OPT_MODE_SPEED_MASK)
#endif

// 向后兼容
#define TUSB_OPT_HOST_ENABLED   CFG_TUH_ENABLED

// 高速支持指示器
#define TUH_OPT_HIGH_SPEED    (CFG_TUH_MAX_SPEED ? (CFG_TUH_MAX_SPEED & OPT_MODE_HIGH_SPEED) : TUP_RHPORT_HIGHSPEED)

//--------------------------------------------------------------------+
// 通用选项（默认）
//--------------------------------------------------------------------+

// TUP_MCU_STRICT_ALIGN will overwrite TUP_ARCH_STRICT_ALIGN.
// In case TUP_MCU_STRICT_ALIGN = 1 and TUP_ARCH_STRICT_ALIGN =0, we will not reply on compiler
// to generate unaligned access code.
// LPC_IP3511 Highspeed cannot access unaligned memory on USB_RAM
#if TUD_OPT_HIGH_SPEED && TU_CHECK_MCU(OPT_MCU_LPC54XXX, OPT_MCU_LPC55XX)
  #define TUP_MCU_STRICT_ALIGN   1
#else
  #define TUP_MCU_STRICT_ALIGN   0
#endif


//--------------------------------------------------------------------+
// Common Options (Default)
//--------------------------------------------------------------------+

// Debug enable to print out error message
#ifndef CFG_TUSB_DEBUG
  #define CFG_TUSB_DEBUG 0
#endif

// USBH日志级别
#ifndef CFG_TUH_LOG_LEVEL
  #define CFG_TUH_LOG_LEVEL   2
#endif

// USBD日志级别
#ifndef CFG_TUD_LOG_LEVEL
  #define CFG_TUD_LOG_LEVEL   2
#endif

// 用于放置USB传输缓冲区的内存段
#ifndef CFG_TUSB_MEM_SECTION
  #define CFG_TUSB_MEM_SECTION
#endif

// USB传输缓冲区的对齐要求
#ifndef CFG_TUSB_MEM_ALIGN
  #define CFG_TUSB_MEM_ALIGN      TU_ATTR_ALIGNED(4)
#endif

// D-Cache行大小
#ifndef CFG_TUSB_MEM_DCACHE_LINE_SIZE
  #ifndef CFG_TUSB_MEM_DCACHE_LINE_SIZE_DEFAULT
  #define CFG_TUSB_MEM_DCACHE_LINE_SIZE_DEFAULT 32
  #endif

  #define CFG_TUSB_MEM_DCACHE_LINE_SIZE CFG_TUSB_MEM_DCACHE_LINE_SIZE_DEFAULT
#endif

// 操作系统选择
#ifndef CFG_TUSB_OS
  #define CFG_TUSB_OS             OPT_OS_NONE
#endif

// 操作系统头文件路径
#ifndef CFG_TUSB_OS_INC_PATH
  #ifndef CFG_TUSB_OS_INC_PATH_DEFAULT
  #define CFG_TUSB_OS_INC_PATH_DEFAULT
  #endif

  #define CFG_TUSB_OS_INC_PATH  CFG_TUSB_OS_INC_PATH_DEFAULT
#endif

//--------------------------------------------------------------------
// 设备选项（默认）
//--------------------------------------------------------------------

// 设备控制器内存段属性
#ifndef CFG_TUD_MEM_SECTION
  #define CFG_TUD_MEM_SECTION     CFG_TUSB_MEM_SECTION
#endif

// 设备控制器内存对齐属性
#ifndef CFG_TUD_MEM_ALIGN
  #define CFG_TUD_MEM_ALIGN       CFG_TUSB_MEM_ALIGN
#endif

// 设备D-Cache使能
#ifndef CFG_TUD_MEM_DCACHE_ENABLE
  #ifndef CFG_TUD_MEM_DCACHE_ENABLE_DEFAULT
  #define CFG_TUD_MEM_DCACHE_ENABLE_DEFAULT  0
  #endif

  #define CFG_TUD_MEM_DCACHE_ENABLE   CFG_TUD_MEM_DCACHE_ENABLE_DEFAULT
#endif

// 设备D-Cache行大小
#ifndef CFG_TUD_MEM_DCACHE_LINE_SIZE
  #define CFG_TUD_MEM_DCACHE_LINE_SIZE CFG_TUSB_MEM_DCACHE_LINE_SIZE
#endif

// 端点0大小
#ifndef CFG_TUD_ENDPOINT0_SIZE
  #define CFG_TUD_ENDPOINT0_SIZE  64
#endif

// 最大接口数
#ifndef CFG_TUD_INTERFACE_MAX
  #define CFG_TUD_INTERFACE_MAX   16
#endif

// 最大端点数
#ifndef CFG_TUD_ENDPPOINT_MAX
  #define CFG_TUD_ENDPPOINT_MAX   TUP_DCD_ENDPOINT_MAX
#endif

#if CFG_TUD_ENDPPOINT_MAX > TUP_DCD_ENDPOINT_MAX
  #error "CFG_TUD_ENDPPOINT_MAX必须小于或等于TUP_DCD_ENDPOINT_MAX"
#endif

// USB 2.0合规性测试模式支持
#ifndef CFG_TUD_TEST_MODE
  #define CFG_TUD_TEST_MODE       0
#endif

//------------- 设备类驱动程序 -------------//
// 蓝牙
#ifndef CFG_TUD_BTH
  #define CFG_TUD_BTH             0
#endif

#if CFG_TUD_BTH && !defined(CFG_TUD_BTH_ISO_ALT_COUNT)
#error CFG_TUD_BTH_ISO_ALT_COUNT必须定义，以告知蓝牙驱动程序要使用的ISO端点数量
#endif

// CDC
#ifndef CFG_TUD_CDC
  #define CFG_TUD_CDC             0
#endif

// MSC
#ifndef CFG_TUD_MSC
  #define CFG_TUD_MSC             0
#endif

// HID
#ifndef CFG_TUD_HID
  #define CFG_TUD_HID             0
#endif

// 音频
#ifndef CFG_TUD_AUDIO
  #define CFG_TUD_AUDIO           0
#endif

// 视频
#ifndef CFG_TUD_VIDEO
  #define CFG_TUD_VIDEO           0
#endif

// MIDI
#ifndef CFG_TUD_MIDI
  #define CFG_TUD_MIDI            0
#endif

// 厂商特定
#ifndef CFG_TUD_VENDOR
  #define CFG_TUD_VENDOR          0
#endif

// USBTMC
#ifndef CFG_TUD_USBTMC
  #define CFG_TUD_USBTMC          0
#endif

// DFU运行时
#ifndef CFG_TUD_DFU_RUNTIME
  #define CFG_TUD_DFU_RUNTIME     0
#endif

// DFU
#ifndef CFG_TUD_DFU
  #define CFG_TUD_DFU             0
#endif

// ECM/RNDIS
#ifndef CFG_TUD_ECM_RNDIS
  #ifdef CFG_TUD_NET
    #warning "CFG_TUD_NET已重命名为CFG_TUD_ECM_RNDIS"
    #define CFG_TUD_ECM_RNDIS   CFG_TUD_NET
  #else
    #define CFG_TUD_ECM_RNDIS   0
  #endif
#endif

// NCM
#ifndef CFG_TUD_NCM
  #define CFG_TUD_NCM         0
#endif

//--------------------------------------------------------------------
// 主机选项（默认）
//--------------------------------------------------------------------
#if CFG_TUH_ENABLED
  #ifndef CFG_TUH_DEVICE_MAX
    #define CFG_TUH_DEVICE_MAX 1
  #endif

  #ifndef CFG_TUH_ENUMERATION_BUFSIZE
    #define CFG_TUH_ENUMERATION_BUFSIZE 256
  #endif
#endif // CFG_TUH_ENABLED

// 主机控制器内存段属性
#ifndef CFG_TUH_MEM_SECTION
  #define CFG_TUH_MEM_SECTION   CFG_TUSB_MEM_SECTION
#endif

// 主机控制器内存对齐属性
#ifndef CFG_TUH_MEM_ALIGN
  #define CFG_TUH_MEM_ALIGN     CFG_TUSB_MEM_ALIGN
#endif

// 主机D-Cache使能
#ifndef CFG_TUH_MEM_DCACHE_ENABLE
  #ifndef CFG_TUH_MEM_DCACHE_ENABLE_DEFAULT
  #define CFG_TUH_MEM_DCACHE_ENABLE_DEFAULT  0
  #endif

  #define CFG_TUH_MEM_DCACHE_ENABLE   CFG_TUH_MEM_DCACHE_ENABLE_DEFAULT
#endif

#ifndef CFG_TUH_MEM_DCACHE_LINE_SIZE
  #define CFG_TUH_MEM_DCACHE_LINE_SIZE CFG_TUSB_MEM_DCACHE_LINE_SIZE
#endif
//------------- 类 -------------//

#ifndef CFG_TUH_HUB
  #define CFG_TUH_HUB    0  // 集线器支持
#endif

#ifndef CFG_TUH_CDC
  #define CFG_TUH_CDC    0  // CDC类支持
#endif

// FTDI不是CDC类的一部分，仅重用CDC驱动API
#ifndef CFG_TUH_CDC_FTDI
  #define CFG_TUH_CDC_FTDI 0
#endif

// 可以使用FTDI CDC驱动的产品ID列表。0x0403是FTDI的VID
#ifndef CFG_TUH_CDC_FTDI_VID_PID_LIST
  #define CFG_TUH_CDC_FTDI_VID_PID_LIST \
    {0x0403, 0x6001}, {0x0403, 0x6006}, {0x0403, 0x6010}, {0x0403, 0x6011}, \
    {0x0403, 0x6014}, {0x0403, 0x6015}, {0x0403, 0x8372}, {0x0403, 0xFBFA}, \
    {0x0403, 0xCD18}
#endif

// CP210X不是CDC类的一部分，仅重用CDC驱动API
#ifndef CFG_TUH_CDC_CP210X
  #define CFG_TUH_CDC_CP210X 0
#endif

// 可以使用CP210X CDC驱动的产品ID列表。0x10C4是Silicon Labs的VID
#ifndef CFG_TUH_CDC_CP210X_VID_PID_LIST
  #define CFG_TUH_CDC_CP210X_VID_PID_LIST \
    {0x10C4, 0xEA60}, {0x10C4, 0xEA70}
#endif

#ifndef CFG_TUH_CDC_CH34X
  // CH34X不是CDC类的一部分，仅重用CDC驱动API
  #define CFG_TUH_CDC_CH34X 0
#endif

// 可以使用CH34X CDC驱动的产品ID列表
#ifndef CFG_TUH_CDC_CH34X_VID_PID_LIST
  #define CFG_TUH_CDC_CH34X_VID_PID_LIST \
    { 0x1a86, 0x5523 }, /* ch341芯片 */ \
    { 0x1a86, 0x7522 }, /* ch340k芯片 */ \
    { 0x1a86, 0x7523 }, /* ch340芯片 */ \
    { 0x1a86, 0xe523 }, /* ch330芯片 */ \
    { 0x4348, 0x5523 }, /* ch340自定义芯片 */ \
    { 0x2184, 0x0057 }, /* 从Linux内核驱动/drivers/usb/serial/ch341.c获取 */ \
    { 0x9986, 0x7523 }  /* 从Linux内核驱动/drivers/usb/serial/ch341.c获取 */
#endif

#ifndef CFG_TUH_HID
  #define CFG_TUH_HID    0  // HID类支持
#endif

#ifndef CFG_TUH_MIDI
  #define CFG_TUH_MIDI   0  // MIDI类支持
#endif

#ifndef CFG_TUH_MSC
  #define CFG_TUH_MSC    0  // 大容量存储类支持
#endif

#ifndef CFG_TUH_VENDOR
  #define CFG_TUH_VENDOR 0  // 厂商特定类支持
#endif

#ifndef CFG_TUH_API_EDPT_XFER
  #define CFG_TUH_API_EDPT_XFER 0  // 端点传输API支持
#endif

//--------------------------------------------------------------------+
// TypeC选项（默认）
//--------------------------------------------------------------------+

#ifndef CFG_TUC_ENABLED
#define CFG_TUC_ENABLED 0  // TypeC支持

#define tuc_int_handler(_p)
#endif

//------------------------------------------------------------------
// 配置验证
//------------------------------------------------------------------
#if CFG_TUD_ENDPOINT0_SIZE > 64
  #error 控制端点最大包大小不能大于64
#endif

// 为避免使用-pedantic选项（严格ISO C）时出现GCC编译器警告
typedef int make_iso_compilers_happy;

#endif /* _TUSB_OPTION_H_ */

/** @} */
