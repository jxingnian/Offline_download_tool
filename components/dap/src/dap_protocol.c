/**
 * @file dap_protocol.c
 * @brief DAP协议层实现
 * @author xingnina (j_xingnian@163.com)
 * @version 1.0
 * @date 2025-02-14
 */

#include "esp_log.h"
#include "dap.h"
#include "dap_hw.h"
#include "driver/gpio.h"
#include "esp_rom_sys.h"
#include <string.h>

static const char *TAG = "DAP_PROTO";

/* SWD传输配置 */
static struct {
    uint8_t idle_cycles;
    uint16_t wait_retry;
    uint16_t match_retry;
} transfer_config = {
    .idle_cycles = 0,
    .wait_retry = 100,
    .match_retry = 100
};

/* SWD请求头格式 */
#define SWD_REQ_START    1U      // Start bit
#define SWD_REQ_APnDP    (1U<<1) // AP not DP
#define SWD_REQ_RnW     (1U<<2)  // Read not Write
#define SWD_REQ_A32     (7U<<3)  // A[3:2] register address
#define SWD_REQ_PARITY  (1U<<6)  // Parity bit
#define SWD_REQ_STOP    (1U<<7)  // Stop bit
#define SWD_REQ_PARK    (1U<<8)  // Park bit

/* SWD应答值 */
#define SWD_ACK_OK      1U  // OK response
#define SWD_ACK_WAIT    2U  // WAIT response
#define SWD_ACK_FAULT   4U  // FAULT response

/* 时钟延时 */
#define PIN_DELAY() esp_rom_delay_us(1)

/* SWD时钟周期 */
static inline void swd_clock_cycle(void) {
    dap_hw_swclk_set(0);
    PIN_DELAY();
    dap_hw_swclk_set(1);
    PIN_DELAY();
}

/* 写一位数据 */
static inline void swd_write_bit(uint8_t bit) {
    dap_hw_swdio_set(bit);
    dap_hw_swclk_set(0);
    PIN_DELAY();
    dap_hw_swclk_set(1);
    PIN_DELAY();
}

/* 读一位数据 */
static inline uint8_t swd_read_bit(void) {
    uint8_t bit;
    dap_hw_swclk_set(0);
    PIN_DELAY();
    bit = dap_hw_swdio_get();
    dap_hw_swclk_set(1);
    PIN_DELAY();
    return bit;
}

/**
 * @brief 生成SWD序列
 */
void swd_sequence(uint32_t count, const uint8_t *data) {
    uint32_t val;
    uint32_t n;

    val = 0;
    n = 0;
    while (count--) {
        if (n == 0) {
            val = *data++;
            n = 8;
        }
        dap_hw_swdio_set(val & 1);
        swd_clock_cycle();
        val >>= 1;
        n--;
    }
}

/**
 * @brief 执行SWD传输
 */
uint8_t dap_transfer(uint8_t *request, uint8_t *response) {
    uint32_t req = request[0];
    uint32_t *data = (uint32_t *)&request[1];
    uint32_t val;
    uint32_t parity;
    uint32_t ack;
    uint32_t bit;
    
    // 配置SWDIO为输出
    dap_hw_swdio_mode(GPIO_MODE_OUTPUT);
    
    // 发送请求
    val = req & 0xFF;
    parity = 0;
    for (bit = 0; bit < 8; bit++) {
        swd_write_bit(val & 1);
        if (val & 1) {
            parity ^= 1;
        }
        val >>= 1;
    }
    
    // 发送奇偶校验位
    swd_write_bit(parity);
    
    // 发送停止位和停泊位
    swd_write_bit(1);
    swd_write_bit(1);
    
    // 切换SWDIO为输入
    dap_hw_swdio_mode(GPIO_MODE_INPUT);
    
    // 读取应答
    ack = 0;
    for (bit = 0; bit < 3; bit++) {
        ack |= swd_read_bit() << bit;
    }
    
    response[0] = ack;
    
    if (ack == SWD_ACK_OK) {
        // 数据传输
        if (req & SWD_REQ_RnW) {
            // 读数据
            val = 0;
            parity = 0;
            for (bit = 0; bit < 32; bit++) {
                val >>= 1;
                if (swd_read_bit()) {
                    val |= 0x80000000;
                    parity ^= 1;
                }
            }
            
            // 检查奇偶校验
            if (parity != swd_read_bit()) {
                response[0] = DAP_ERROR;
                return DAP_ERROR;
            }
            
            *data = val;
            return DAP_OK;
        } else {
            // 写数据
            val = *data;
            parity = 0;
            
            // 配置SWDIO为输出
            dap_hw_swdio_mode(GPIO_MODE_OUTPUT);
            
            for (bit = 0; bit < 32; bit++) {
                swd_write_bit(val & 1);
                if (val & 1) {
                    parity ^= 1;
                }
                val >>= 1;
            }
            
            // 发送奇偶校验位
            swd_write_bit(parity);
            return DAP_OK;
        }
    }
    
    return ack;
}

/**
 * @brief 配置传输参数
 */
uint8_t dap_transfer_configure(uint8_t *data) {
    transfer_config.idle_cycles = data[0];
    transfer_config.wait_retry = (data[1] << 0) | (data[2] << 8);
    transfer_config.match_retry = (data[3] << 0) | (data[4] << 8);
    
    ESP_LOGI(TAG, "Transfer config - Idle: %d, Wait: %d, Match: %d",
             transfer_config.idle_cycles,
             transfer_config.wait_retry,
             transfer_config.match_retry);
             
    return DAP_OK;
}

/**
 * @brief 执行SWD块传输
 */
uint8_t dap_transfer_block(uint8_t *data, uint8_t *response) {
    uint32_t count = (data[1] << 0) | (data[2] << 8);
    uint32_t request = data[0];
    uint32_t *data_ptr = (uint32_t *)&data[3];
    uint32_t *resp_ptr = (uint32_t *)&response[3];
    uint8_t resp;
    
    response[0] = request;
    response[1] = count & 0xFF;
    response[2] = (count >> 8) & 0xFF;
    
    while (count--) {
        resp = dap_transfer((uint8_t *)&request, (uint8_t *)resp_ptr);
        if (resp != DAP_OK) {
            response[0] = resp;
            return resp;
        }
        data_ptr++;
        resp_ptr++;
    }
    
    return DAP_OK;
}
