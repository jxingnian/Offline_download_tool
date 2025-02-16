#pragma once

#include <stdint.h>
#include "esp_err.h"

// DAP 数据包大小
#define DAP_PACKET_SIZE 64

// DAP 缓冲区数量
#define DAP_BUFFER_NUM 20

// DAP 处理状态
typedef enum {
    DAP_OK = 0,
    DAP_ERROR,
    DAP_BUSY
} dap_status_t;

// 初始化 DAP 处理模块
esp_err_t dap_handle_init(void);

// 反初始化 DAP 处理模块
void dap_handle_deinit(void);

// 处理 DAP 命令
esp_err_t dap_handle_request(uint8_t *request, uint16_t req_size, uint8_t *response, uint16_t *resp_size);

// DAP 处理任务
void dap_handle_task(void *arg);
