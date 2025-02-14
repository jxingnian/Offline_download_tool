/*
 * @Author: xingnina j_xingnian@163.com
 * @Date: 2025-02-14 09:44:00
 * @LastEditors: xingnina j_xingnian@163.com
 * @LastEditTime: 2025-02-14 09:44:00
 * @FilePath: \Offline_download_tool\components\dap\include\dap_hw.h
 */
#ifndef _DAP_HW_H_
#define _DAP_HW_H_

#include <stdint.h>
#include "driver/gpio.h"
#include "esp_err.h"

/**
 * @brief 初始化DAP硬件
 * 
 * @return esp_err_t 
 */
esp_err_t dap_hw_init(void);

/**
 * @brief 设置SWCLK引脚电平
 * 
 * @param level 电平值（0或1）
 */
void dap_hw_swclk_set(uint8_t level);

/**
 * @brief 设置SWDIO引脚电平
 * 
 * @param level 电平值（0或1）
 */
void dap_hw_swdio_set(uint8_t level);

/**
 * @brief 读取SWDIO引脚电平
 * 
 * @return uint8_t 电平值（0或1）
 */
uint8_t dap_hw_swdio_get(void);

/**
 * @brief 设置nRESET引脚电平
 * 
 * @param level 电平值（0或1）
 */
void dap_hw_reset_set(uint8_t level);

/**
 * @brief 配置SWDIO引脚方向
 * 
 * @param mode GPIO模式（输入/输出）
 */
void dap_hw_swdio_mode(gpio_mode_t mode);

#endif /* _DAP_HW_H_ */
