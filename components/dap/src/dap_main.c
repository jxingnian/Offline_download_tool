/**
 * @file dap_main.c
 * @brief DAP组件主实现文件
 * @author xingnina (j_xingnian@163.com)
 * @version 1.0
 * @date 2025-02-14
 */

#include <string.h>
#include "esp_log.h"
#include "dap.h"

static const char *TAG = "DAP";

/* DAP版本信息 */
#define DAP_FW_VER     "1.0.0"
#define DAP_VID        0x0D28    // ARM的标准VID
#define DAP_PID        0x0204    // CMSIS-DAP标准PID

/* DAP状态 */
static struct {
    bool initialized;
    bool connected;
} dap_state = {
    .initialized = false,
    .connected = false
};

/**
 * @brief 处理DAP Info命令
 */
static uint8_t process_info(uint8_t *request, uint8_t *response) {
    uint8_t id = request[0];
    uint8_t *resp = &response[1];
    
    switch (id) {
        case 0x01:  // 获取VID
            resp[0] = 4;
            resp[1] = DAP_VID & 0xFF;
            resp[2] = (DAP_VID >> 8) & 0xFF;
            return 3;
            
        case 0x02:  // 获取PID
            resp[0] = 4;
            resp[1] = DAP_PID & 0xFF;
            resp[2] = (DAP_PID >> 8) & 0xFF;
            return 3;
            
        case 0xF0:  // 获取版本信息
            resp[0] = strlen(DAP_FW_VER);
            strcpy((char *)&resp[1], DAP_FW_VER);
            return strlen(DAP_FW_VER) + 1;
            
        default:
            resp[0] = 0;
            return 1;
    }
}

esp_err_t dap_init(void) {
    if (dap_state.initialized) {
        return ESP_OK;
    }
    
    ESP_LOGI(TAG, "Initializing DAP...");
    
    // TODO: 初始化硬件接口
    
    dap_state.initialized = true;
    ESP_LOGI(TAG, "DAP initialized successfully");
    return ESP_OK;
}

esp_err_t dap_process_command(uint8_t *request, uint16_t request_len, 
                            uint8_t *response, uint16_t *response_len) {
    if (!request || !response || !response_len || request_len < 1) {
        return ESP_FAIL;
    }
    
    uint8_t cmd = request[0];
    uint8_t resp_size = 0;
    
    // 设置响应命令ID
    response[0] = cmd;
    
    switch (cmd) {
        case ID_DAP_Info:
            resp_size = process_info(&request[1], response);
            break;
            
        // TODO: 实现其他命令处理
            
        default:
            ESP_LOGW(TAG, "Unknown command: 0x%02x", cmd);
            response[0] = DAP_ERROR;
            resp_size = 1;
            break;
    }
    
    *response_len = resp_size;
    return ESP_OK;
}
