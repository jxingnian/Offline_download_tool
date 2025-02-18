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
#include "sdkconfig.h"
#include "led.h"
#include "dap_handle.h"
#include "usb_descriptors.h"
#include "tinyusb.h"
#include "class/vendor/vendor_device.h"
#include <inttypes.h>

static const char *TAG = "MAIN";

// BULK 传输缓冲区大小
#define BULK_BUFFER_SIZE 64
static uint32_t cmd_count = 0;

// BULK 传输回调函数
void tud_vendor_rx_cb(uint8_t itf, uint8_t const* buffer, uint16_t bufsize)
{
    cmd_count++;
    ESP_LOGI(TAG, "[%lu] 收到 BULK 数据 - 接口: %d, 大小: %" PRIu16 " 字节", cmd_count, itf, bufsize);
    
    if (bufsize > 0) {
        ESP_LOGI(TAG, "[%lu] 命令ID: 0x%02X", cmd_count, buffer[0]);
        ESP_LOG_BUFFER_HEX_LEVEL(TAG, buffer, bufsize, ESP_LOG_DEBUG);
    }
    
    uint8_t response[BULK_BUFFER_SIZE];
    uint16_t resp_len = 0;

    // 处理DAP命令
    esp_err_t err = dap_handle_request((uint8_t*)buffer, bufsize, response, &resp_len);
    if (err == ESP_OK && resp_len > 0) {
        ESP_LOGI(TAG, "[%lu] 准备发送响应，大小: %" PRIu16 " 字节", cmd_count, resp_len);
        ESP_LOG_BUFFER_HEX_LEVEL(TAG, response, resp_len, ESP_LOG_DEBUG);
        
        // 尝试发送响应，如果失败则重试
        int retry = 3;
        while (retry--) {
            if (tud_vendor_write(response, resp_len)) {
                ESP_LOGI(TAG, "[%lu] 响应发送成功", cmd_count);
                break;
            } else {
                ESP_LOGW(TAG, "[%lu] 发送响应失败，剩余重试次数: %d", cmd_count, retry);
                vTaskDelay(pdMS_TO_TICKS(1));
            }
        }
    } else {
        ESP_LOGE(TAG, "[%lu] 处理 DAP 命令失败: %d", cmd_count, err);
    }
}

// BULK 发送完成回调
void tud_vendor_tx_cb(uint8_t itf, uint32_t sent_bytes)
{
    ESP_LOGD(TAG, "[%lu] BULK 发送完成 - 接口: %d, 发送字节数: %lu", cmd_count, itf, sent_bytes);
}

static void usb_device_task(void *param)
{
    (void)param;
    ESP_LOGI(TAG, "USB 设备任务启动");
    
    // 等待 USB 设备枚举完成
    uint32_t wait_count = 0;
    while (!tud_mounted()) {
        wait_count++;
        if (wait_count % 10 == 0) {  // 每1秒打印一次
            ESP_LOGI(TAG, "等待 USB 设备枚举... (%lu)", wait_count/10);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    ESP_LOGI(TAG, "USB 设备枚举完成");

    while (1) {
        tud_task();
        // 检查 USB 状态
        if (!tud_mounted()) {
            ESP_LOGW(TAG, "USB 设备断开连接");
            while (!tud_mounted()) {
                vTaskDelay(pdMS_TO_TICKS(100));
            }
            ESP_LOGI(TAG, "USB 设备重新连接");
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

/**
 * @brief 应用程序主入口函数
 */
void app_main(void)
{
    ESP_LOGI(TAG, "DAP Link 启动...");

    // 初始化LED
    led_init();
    ESP_LOGI(TAG, "LED 初始化完成");

    // 初始化DAP处理模块
    ESP_ERROR_CHECK(dap_handle_init());
    ESP_LOGI(TAG, "DAP 初始化完成");

    // 初始化 TinyUSB
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = get_tusb_desc_device(),
        .string_descriptor = get_tusb_string_descriptor(),
        .string_descriptor_count = get_tusb_string_descriptor_count(),
        .external_phy = false,
        .configuration_descriptor = get_tusb_desc_configuration(),
    };

    ESP_LOGI(TAG, "正在安装 USB 设备驱动...");
    vTaskDelay(pdMS_TO_TICKS(200));  // 等待 USB PHY 稳定
    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
    ESP_LOGI(TAG, "USB 驱动安装完成");
    
    // 创建USB设备任务，提高优先级
    xTaskCreate(usb_device_task, "USB DEVICE", 4096, NULL, configMAX_PRIORITIES - 1, NULL);
    ESP_LOGI(TAG, "USB 设备任务创建完成");

    // 等待 USB 设备初始化完成
    vTaskDelay(pdMS_TO_TICKS(100));
    ESP_LOGI(TAG, "USB 初始化完成");
}
