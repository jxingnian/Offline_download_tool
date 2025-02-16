/*
 * @Author: xingnina j_xingnian@163.com
 * @Date: 2025-02-13 11:52:42
 * @LastEditors: xingnina j_xingnian@163.com
 * @LastEditTime: 2025-02-13 13:29:33
 * @FilePath: \Offline_download_tool\components\led\led.c
 * @file led.c
 * @brief LED控制和WS2812B LED灯带驱动实现
 * @details 本文件包含了普通LED的闪烁控制和WS2812B LED灯带的初始化及像素设置功能
 * @version 1.0
 */

#include "driver/gpio.h"
#include "led.h"
#include "esp_log.h"
#include "driver/rmt_tx.h"
#include "driver/rmt_encoder.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/rmt_tx.h"
#include "led_strip_encoder.h"

// RMT发送通道和编码器句柄
static rmt_channel_handle_t led_chan = NULL;
static rmt_encoder_handle_t led_encoder = NULL;

static const char *TAG = "led";

static esp_err_t ws2812b_init(void);
static esp_err_t ws2812b_set_pixel(uint32_t index, uint8_t red, uint8_t green, uint8_t blue);
static void led_blink_task(void *pvParameters);

/**
 * @brief 初始化LED控制
 */
void led_init(void)
{
    // 初始化普通LED的GPIO
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    
    // 初始化WS2812B LED灯带
    ESP_ERROR_CHECK(ws2812b_init());
    
    // 设置第一个LED的颜色
    ws2812b_set_pixel(0, 0, 255, 255);
    
    // 创建LED闪烁任务
    xTaskCreate(led_blink_task, "LED Blink Task", 2048, NULL, 5, NULL);
}

/**
 * @brief 初始化WS2812B LED灯带
 * @return esp_err_t 初始化结果
 */
static esp_err_t ws2812b_init(void)
{
    ESP_LOGI(TAG, "创建RMT TX通道");
    rmt_tx_channel_config_t tx_chan_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT, // 选择源时钟
        .gpio_num = RMT_LED_STRIP_GPIO_NUM,
        .mem_block_symbols = 64, // 增加块大小可以使LED闪烁减少
        .resolution_hz = RMT_LED_STRIP_RESOLUTION_HZ,
        .trans_queue_depth = 4, // 设置可以在后台挂起的事务数量
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &led_chan));

    ESP_LOGI(TAG, "安装LED条编码器");
    led_strip_encoder_config_t encoder_config = {
        .resolution = RMT_LED_STRIP_RESOLUTION_HZ,
    };
    ESP_ERROR_CHECK(rmt_new_led_strip_encoder(&encoder_config, &led_encoder));

    ESP_LOGI(TAG, "启用RMT TX通道");
    ESP_ERROR_CHECK(rmt_enable(led_chan));

    return ESP_OK;
}

/**
 * @brief 设置WS2812B LED灯带指定像素的颜色
 * @param index LED像素索引
 * @param red 红色分量（0-255）
 * @param green 绿色分量（0-255）
 * @param blue 蓝色分量（0-255）
 * @return esp_err_t 操作结果
 */
static esp_err_t ws2812b_set_pixel(uint32_t index, uint8_t red, uint8_t green, uint8_t blue)
{
    if (led_chan == NULL || led_encoder == NULL) {
        ESP_LOGE(TAG, "LED通道或编码器未初始化");
        return ESP_ERR_INVALID_STATE;
    }

    // WS2812B的颜色顺序是GRB
    // 注意：WS2812B的颜色顺序是GRB，但是在内存中是BGR
    uint32_t pixel_data = (blue << 16) | (red << 8) | green;
    
    // 配置RMT传输
    rmt_transmit_config_t tx_config = {
        .loop_count = 0, // 不循环发送
    };

    ESP_LOGI(TAG, "发送LED数据: R=%d, G=%d, B=%d", red, green, blue);
    // 发送LED数据
    return rmt_transmit(led_chan, led_encoder, &pixel_data, sizeof(pixel_data), &tx_config);
}

/**
 * @brief LED闪烁任务
 * @param pvParameters 任务参数
 */
static void led_blink_task(void *pvParameters)
{
    int state = 0;
    while (1) {
        // 切换LED状态
        gpio_set_level(BLINK_GPIO, state);
        vTaskDelay(pdMS_TO_TICKS(500)); // 500ms延时
        state = !state;
    }
}
