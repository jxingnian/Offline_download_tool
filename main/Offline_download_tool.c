/*
 * @Author: xingnina j_xingnian@163.com
 * @Date: 2025-02-05 13:08:50
 * @LastEditors: xingnina j_xingnian@163.com
 * @LastEditTime: 2025-02-14 15:34:18
 * @FilePath: \Offline_download_tool\main\Offline_download_tool.c
 * @file Offline_download_tool.c
 * @brief ESP32-S3-N16R8 DAPlink离线下载器项目主文件
 * @version 0.1
 * @copyright Copyright (c) 2025
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "dap.h"
#include "tusb.h"
#include "esp_log.h"

static const char *TAG = "MAIN";

/**
 * @brief USB 设备任务
 * @param param 任务参数
 */
static void usb_device_task(void *param)
{
    (void)param;
    ESP_LOGI(TAG, "USB device task started");
    
    while (1) {
        tud_task(); // TinyUSB 设备任务
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

/**
 * @brief 应用程序主入口函数
 */
void app_main(void)
{
    led_init();
    ESP_LOGI(TAG, "Initializing USB device...");

    // init device stack on configured roothub port
    tusb_rhport_init_t dev_init = {
        .role = TUSB_ROLE_DEVICE,
        .speed = TUSB_SPEED_AUTO
    };
    tusb_init(BOARD_TUD_RHPORT, &dev_init);

    // 创建 USB 设备任务
    xTaskCreate(usb_device_task, "usbd", 4096, NULL, 5, NULL);
    
    ESP_LOGI(TAG, "USB device initialized");
}
