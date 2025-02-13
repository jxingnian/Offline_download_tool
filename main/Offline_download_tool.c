/*
 * @Author: xingnina j_xingnian@163.com
 * @Date: 2025-02-05 13:08:50
 * @LastEditors: xingnina j_xingnian@163.com
 * @LastEditTime: 2025-02-13 11:12:36
 * @FilePath: \Offline_download_tool\main\Offline_download_tool.c
 * @file Offline_download_tool.c
 * @brief ESP32-S3-N16R8 DAPlink离线下载器项目主文件
 * @IDF: 项目使用ESP-IDF 5.3版本SDK进行开发。
 * @note 目标芯片：ESP32-S3-N16R8
 * @note SDK版本：IDF 5.3
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"

/**
 * @brief 应用程序主入口函数
 *
 * 此函数执行以下操作：
 * 1. 获取并打印芯片硬件信息
 * 2. 显示芯片的CPU核心数
 * 3. 显示芯片支持的特性（WiFi、蓝牙等）
 * 4. 打印芯片硅版本
 * 5. 获取并显示Flash大小
 * 6. 显示最小可用堆大小
 * 7. 倒计时后重启系统
 *
 * @note 使用FreeRTOS任务延迟函数进行倒计时
 * @note 使用esp_restart()进行系统重启
 */
void app_main(void)
{
    // 定义芯片信息结构体
    esp_chip_info_t chip_info;
    uint32_t flash_size;

    // 获取芯片信息
    esp_chip_info(&chip_info);

    // 打印芯片基本信息：目标、CPU核心数、支持的特性
    printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
           (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
           (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

    // 计算并打印芯片版本
    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);

    // 获取Flash大小
    if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        printf("Get flash size failed");
        return;
    }

    // 打印Flash大小和类型（嵌入式或外部）
    printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    // 打印最小可用堆大小
    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());

    // 10秒倒计时重启
    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        // 使用FreeRTOS任务延迟函数延迟1秒
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    // 打印重启信息
    printf("Restarting now.\n");
    
    // 确保输出缓冲区被刷新
    fflush(stdout);

    // 重启系统
    esp_restart();
}
