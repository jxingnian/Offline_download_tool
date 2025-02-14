/*
 * @Author: xingnina j_xingnian@163.com
 * @Date: 2025-02-13 11:52:47
 * @LastEditors: xingnina j_xingnian@163.com
 * @LastEditTime: 2025-02-14 15:37:18
 * @FilePath: \Offline_download_tool\components\led\led.h
 */
#ifndef _LED_H_
#define _LED_H_

//led引脚定义
#define BLINK_GPIO 35

// 定义LED条的分辨率和GPIO引脚
#define RMT_LED_STRIP_RESOLUTION_HZ 10000000 // 10MHz分辨率，1个tick = 0.1us（LED条需要高分辨率）
#define RMT_LED_STRIP_GPIO_NUM      48

//函数声明
void led_init(void);
static esp_err_t ws2812b_init(void);
static esp_err_t ws2812b_set_pixel(uint32_t index, uint8_t red, uint8_t green, uint8_t blue);
static void led_blink_task(void *pvParameters);

#endif
