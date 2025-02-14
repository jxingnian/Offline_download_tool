/*
 * @Author: xingnina j_xingnian@163.com
 * @Date: 2025-02-14 09:24:40
 * @LastEditors: xingnina j_xingnian@163.com
 * @LastEditTime: 2025-02-14 09:41:19
 * @FilePath: \Offline_download_tool\components\dap\include\dap.h
 */
/**
 * @file dap.h
 * @brief DAP (Debug Access Port) 组件的主头文件
 * @author xingnina (j_xingnian@163.com)
 * @version 1.0
 * @date 2025-02-14
 * 
 * @copyright Copyright (c) 2025
 * 
 * @note 该组件实现了CMSIS-DAP协议，用于调试ARM Cortex-M系列芯片
 */

#ifndef _DAP_H_
#define _DAP_H_

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

/* DAP Command IDs */
#define ID_DAP_Info                     0x00U
#define ID_DAP_HostStatus              0x01U
#define ID_DAP_Connect                 0x02U
#define ID_DAP_Disconnect             0x03U
#define ID_DAP_TransferConfigure      0x04U
#define ID_DAP_Transfer               0x05U
#define ID_DAP_TransferBlock          0x06U
#define ID_DAP_TransferAbort          0x07U
#define ID_DAP_WriteABORT             0x08U
#define ID_DAP_Delay                  0x09U
#define ID_DAP_ResetTarget            0x0AU
#define ID_DAP_SWJ_Pins               0x10U
#define ID_DAP_SWJ_Clock              0x11U
#define ID_DAP_SWJ_Sequence           0x12U
#define ID_DAP_SWD_Configure          0x13U
#define ID_DAP_JTAG_Sequence          0x14U
#define ID_DAP_JTAG_Configure         0x15U
#define ID_DAP_JTAG_IDCODE           0x16U

/* DAP Status Codes */
#define DAP_OK                      0U
#define DAP_ERROR                   0xFFU

/* DAP Interface */
typedef struct {
    uint8_t (*initialize)(void);
    uint8_t (*uninitialize)(void);
    uint8_t (*connect)(void);
    uint8_t (*disconnect)(void);
    uint8_t (*transfer_configure)(uint8_t *data);
    uint8_t (*transfer)(uint8_t *data, uint8_t *response);
    uint8_t (*transfer_block)(uint8_t *data, uint8_t *response);
} dap_ops_t;

/**
 * @brief 初始化DAP组件
 * 
 * @return esp_err_t 
 *         - ESP_OK: 成功
 *         - ESP_FAIL: 失败
 */
esp_err_t dap_init(void);

/**
 * @brief 处理DAP命令
 * 
 * @param request 请求数据
 * @param request_len 请求数据长度
 * @param response 响应数据缓冲区
 * @param response_len 响应数据长度指针
 * @return esp_err_t 
 *         - ESP_OK: 成功
 *         - ESP_FAIL: 失败
 */
esp_err_t dap_process_command(uint8_t *request, uint16_t request_len, 
                            uint8_t *response, uint16_t *response_len);

#endif /* _DAP_H_ */
