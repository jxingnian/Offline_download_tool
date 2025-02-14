/*
 * @Author: xingnina j_xingnian@163.com
 * @Date: 2025-02-05 13:08:50
 * @LastEditors: xingnina j_xingnian@163.com
 * @LastEditTime: 2025-02-14 10:33:24
 * @FilePath: \Offline_download_tool\main\Offline_download_tool.c
 * @file Offline_download_tool.c
 * @brief ESP32-S3-N16R8 DAPlink离线下载器项目主文件
 * @IDF: 项目使用ESP-IDF 5.3版本SDK进行开发。
 * @note 目标芯片：ESP32-S3-N16R8
 * @note SDK版本：IDF 5.3
 */

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "dap.h"
#include "tusb.h"

// /**
//  * @brief 初始化USB
//  */
// static void usb_init(void) {
//     // 初始化TinyUSB
//     tusb_init();
    
//     // 初始化DAP USB接口
//     ESP_ERROR_CHECK(dap_usb_init());
    
//     // 创建USB任务
//     xTaskCreate(dap_usb_task, "usb", 4096, NULL, 5, NULL);
// }

/**
 * @brief 应用程序主入口函数
 */
void app_main(void) {
    // LED初始化
    led_init();
    
    // 初始化DAP
    ESP_ERROR_CHECK(dap_init());
    
    // // 初始化USB
    // usb_init();
}
