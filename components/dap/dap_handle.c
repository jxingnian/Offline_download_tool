#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/ringbuf.h"
#include "freertos/semphr.h"
#include "esp_log.h"

#include "dap_handle.h"
#include "DAP.h"

static const char *TAG = "DAP_HANDLE";

// DAP 数据包结构
typedef struct {
    uint16_t length;
    uint8_t buf[DAP_PACKET_SIZE];
} dap_packet_t;

// DAP 处理相关句柄
static RingbufHandle_t dap_request_buf = NULL;   // 请求缓冲区
static RingbufHandle_t dap_response_buf = NULL;  // 响应缓冲区
static SemaphoreHandle_t dap_mutex = NULL;       // DAP 互斥锁
static TaskHandle_t dap_task_handle = NULL;      // DAP 任务句柄

esp_err_t dap_handle_init(void)
{
    // 创建互斥锁
    dap_mutex = xSemaphoreCreateMutex();
    if (dap_mutex == NULL) {
        ESP_LOGE(TAG, "Failed to create mutex");
        return ESP_FAIL;
    }

    // 创建环形缓冲区
    dap_request_buf = xRingbufferCreate(sizeof(dap_packet_t) * DAP_BUFFER_NUM, RINGBUF_TYPE_BYTEBUF);
    dap_response_buf = xRingbufferCreate(sizeof(dap_packet_t) * DAP_BUFFER_NUM, RINGBUF_TYPE_BYTEBUF);
    if (dap_request_buf == NULL || dap_response_buf == NULL) {
        ESP_LOGE(TAG, "Failed to create ring buffer");
        return ESP_FAIL;
    }

    // 创建 DAP 处理任务
    BaseType_t ret = xTaskCreate(dap_handle_task, "DAP_HANDLE", 4096, NULL, 5, &dap_task_handle);
    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create DAP task");
        return ESP_FAIL;
    }

    // 初始化 DAP
    DAP_Setup();
    ESP_LOGI(TAG, "DAP handle initialized");
    return ESP_OK;
}

void dap_handle_deinit(void)
{
    if (dap_task_handle) {
        vTaskDelete(dap_task_handle);
        dap_task_handle = NULL;
    }

    if (dap_request_buf) {
        vRingbufferDelete(dap_request_buf);
        dap_request_buf = NULL;
    }

    if (dap_response_buf) {
        vRingbufferDelete(dap_response_buf);
        dap_response_buf = NULL;
    }

    if (dap_mutex) {
        vSemaphoreDelete(dap_mutex);
        dap_mutex = NULL;
    }
}

esp_err_t dap_handle_request(uint8_t *request, uint16_t req_size, uint8_t *response, uint16_t *resp_size)
{
    if (!request || !response || !resp_size || req_size > DAP_PACKET_SIZE) {
        return ESP_ERR_INVALID_ARG;
    }

    dap_packet_t packet = {
        .length = req_size
    };
    memcpy(packet.buf, request, req_size);

    // 发送请求到环形缓冲区
    if (xRingbufferSend(dap_request_buf, &packet, sizeof(dap_packet_t), pdMS_TO_TICKS(100)) != pdTRUE) {
        ESP_LOGW(TAG, "Failed to send request to ring buffer");
        return ESP_FAIL;
    }

    // 等待响应
    size_t size;
    dap_packet_t *resp = (dap_packet_t *)xRingbufferReceive(dap_response_buf, &size, pdMS_TO_TICKS(100));
    if (!resp) {
        ESP_LOGW(TAG, "No response received");
        return ESP_FAIL;
    }

    // 复制响应数据
    memcpy(response, resp->buf, resp->length);
    *resp_size = resp->length;

    // 释放缓冲区
    vRingbufferReturnItem(dap_response_buf, resp);
    return ESP_OK;
}

void dap_handle_task(void *arg)
{
    size_t size;
    dap_packet_t packet;
    dap_packet_t response;

    while (1) {
        // 等待请求
        dap_packet_t *req = (dap_packet_t *)xRingbufferReceive(dap_request_buf, &size, portMAX_DELAY);
        if (!req) {
            continue;
        }

        // 打印请求命令
        ESP_LOGI(TAG, "DAP Command: 0x%02X, Length: %d", req->buf[0], req->length);
        ESP_LOG_BUFFER_HEX_LEVEL(TAG, req->buf, req->length, ESP_LOG_INFO);

        // 处理 DAP 命令
        if (xSemaphoreTake(dap_mutex, portMAX_DELAY) == pdTRUE) {
            response.length = DAP_ProcessCommand(req->buf, response.buf);
            xSemaphoreGive(dap_mutex);

            // 打印响应
            ESP_LOGI(TAG, "DAP Response: Length: %d", response.length);
            ESP_LOG_BUFFER_HEX_LEVEL(TAG, response.buf, response.length, ESP_LOG_INFO);
        }

        // 发送响应
        if (xRingbufferSend(dap_response_buf, &response, sizeof(dap_packet_t), pdMS_TO_TICKS(100)) != pdTRUE) {
            ESP_LOGW(TAG, "Failed to send response");
        }

        // 释放请求缓冲区
        vRingbufferReturnItem(dap_request_buf, req);
    }
}
