/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <stdint.h>
#include "driver/rmt_encoder.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief LED灯带编码器配置类型
 */
typedef struct {
    uint32_t resolution; /*!< 编码器分辨率，单位Hz */
} led_strip_encoder_config_t;

/**
 * @brief 创建RMT编码器，用于将LED灯带像素编码为RMT符号
 *
 * @param[in] config 编码器配置
 * @param[out] ret_encoder 返回的编码器句柄
 * @return
 *      - ESP_ERR_INVALID_ARG 参数无效
 *      - ESP_ERR_NO_MEM 创建LED灯带编码器时内存不足
 *      - ESP_OK 成功创建编码器
 */
esp_err_t rmt_new_led_strip_encoder(const led_strip_encoder_config_t *config, rmt_encoder_handle_t *ret_encoder);

#ifdef __cplusplus
}
#endif
