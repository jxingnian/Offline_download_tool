/**
 * @file dap_hw.c
 * @brief DAP硬件抽象层实现
 * @author xingnina (j_xingnian@163.com)
 * @version 1.0
 * @date 2025-02-14
 */

#include "esp_log.h"
#include "driver/gpio.h"
#include "dap_hw.h"

static const char *TAG = "DAP_HW";

/* DAP引脚定义 */
#define DAP_PIN_SWCLK    GPIO_NUM_4  // TODO: 根据实际硬件配置修改
#define DAP_PIN_SWDIO    GPIO_NUM_5  // TODO: 根据实际硬件配置修改
#define DAP_PIN_nRESET   GPIO_NUM_6  // TODO: 根据实际硬件配置修改

esp_err_t dap_hw_init(void) {
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << DAP_PIN_SWCLK) |
                       (1ULL << DAP_PIN_SWDIO) |
                       (1ULL << DAP_PIN_nRESET),
        .pull_down_en = 0,
        .pull_up_en = 0
    };
    
    esp_err_t ret = gpio_config(&io_conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure GPIO pins");
        return ret;
    }
    
    // 设置默认电平
    gpio_set_level(DAP_PIN_SWCLK, 1);
    gpio_set_level(DAP_PIN_SWDIO, 1);
    gpio_set_level(DAP_PIN_nRESET, 1);
    
    ESP_LOGI(TAG, "DAP hardware initialized");
    return ESP_OK;
}

void dap_hw_swclk_set(uint8_t level) {
    gpio_set_level(DAP_PIN_SWCLK, level);
}

void dap_hw_swdio_set(uint8_t level) {
    gpio_set_level(DAP_PIN_SWDIO, level);
}

uint8_t dap_hw_swdio_get(void) {
    return gpio_get_level(DAP_PIN_SWDIO);
}

void dap_hw_reset_set(uint8_t level) {
    gpio_set_level(DAP_PIN_nRESET, level);
}

void dap_hw_swdio_mode(gpio_mode_t mode) {
    gpio_set_direction(DAP_PIN_SWDIO, mode);
}
