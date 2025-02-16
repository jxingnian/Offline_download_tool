/*
 * @Author: xingnina j_xingnian@163.com
 * @Date: 2025-02-05 13:08:50
 * @LastEditors: xingnina j_xingnian@163.com
 * @LastEditTime: 2025-02-15 16:45:26
 * @FilePath: \Offline_download_tool\main\Offline_download_tool.c
 * @file Offline_download_tool.c
 * @brief ESP32-S3-N16R8 DAPlink离线下载器项目主文件
 * @version 0.1
 * @copyright Copyright (c) 2025
 */

#include <stdio.h>
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tusb.h"
// #include "tusb_cdc_acm.h"
// #include "tusb_console.h"
#include "sdkconfig.h"
#include "led.h"
#include "dap_handle.h"
#include "usb_descriptors.h"

static const char *TAG = "MAIN";

// 当收到SET_REPORT控制请求或在OUT端点上收到数据时调用
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, const uint8_t *buffer, uint16_t bufsize)
{
    if (report_type == HID_REPORT_TYPE_OUTPUT) {
        uint16_t resp_len;
        static uint8_t response[64];
        ESP_LOGI(TAG, "USB HID received %d bytes", bufsize);

        // 处理DAP命令
        if (dap_handle_request((uint8_t*)buffer, bufsize, response, &resp_len) == ESP_OK) {
            // 等待HID就绪
            while (!tud_hid_ready()) {
                // 等待就绪
            }
            // 发送响应
            tud_hid_report(0, response, resp_len);
        }
    }
}

static void usb_device_task(void *param)
{
    (void)param;
    ESP_LOGI(TAG, "USB device task started");
    while (1) {
        tud_task(); // RTOS forever loop
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

/**
 * @brief 应用程序主入口函数
 */
void app_main(void)
{
    ESP_LOGI(TAG, "DAP Link starting...");

    // 初始化LED
    led_init();
    ESP_LOGI(TAG, "LED initialized");

    // 初始化DAP处理模块
    ESP_ERROR_CHECK(dap_handle_init());
    ESP_LOGI(TAG, "DAP initialized");

    // 初始化USB HID
    tud_hid_init();
    ESP_LOGI(TAG, "USB HID initialized");
    
    // 创建USB设备任务
    xTaskCreate(usb_device_task, "USB DEVICE", 4096, NULL, 5, NULL);
    ESP_LOGI(TAG, "USB device task created");
}
