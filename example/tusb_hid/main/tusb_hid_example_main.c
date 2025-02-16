/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

// 包含必要的头文件
#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"
#include "driver/gpio.h"

// 定义应用程序使用的按钮，默认使用BOOT信号
#define APP_BUTTON (GPIO_NUM_0)
// 定义日志标签
static const char *TAG = "example";

/************* TinyUSB 描述符 ****************/

// 定义USB描述符的总长度
#define TUSB_DESC_TOTAL_LEN      (TUD_CONFIG_DESC_LEN + CFG_TUD_HID * TUD_HID_DESC_LEN)

/**
 * @brief HID报告描述符
 *
 * 在这个例子中我们实现了键盘和鼠标HID设备，
 * 所以我们必须定义两种报告描述符
 */
const uint8_t hid_report_descriptor[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(HID_ITF_PROTOCOL_KEYBOARD)),
    TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(HID_ITF_PROTOCOL_MOUSE))
};

/**
 * @brief 字符串描述符
 */
const char* hid_string_descriptor[5] = {
    // 字符串描述符指针数组
    (char[]){0x09, 0x04},  // 0: 支持的语言是英语 (0x0409)
    "TinyUSB",             // 1: 制造商
    "TinyUSB Device",      // 2: 产品
    "123456",              // 3: 序列号，应该使用芯片ID
    "Example HID interface",  // 4: HID接口
};

/**
 * @brief 配置描述符
 *
 * 这是一个简单的配置描述符，定义了1个配置和1个HID接口
 */
static const uint8_t hid_configuration_descriptor[] = {
    // 配置号, 接口数, 字符串索引, 总长度, 属性, 功耗(mA)
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, TUSB_DESC_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

    // 接口号, 字符串索引, 引导协议, 报告描述符长度, EP In地址, 大小和轮询间隔
    TUD_HID_DESCRIPTOR(0, 4, false, sizeof(hid_report_descriptor), 0x81, 16, 10),
};

/********* TinyUSB HID 回调函数 ***************/

// 当收到GET HID REPORT DESCRIPTOR请求时调用
// 应用程序返回描述符指针，其内容必须存在足够长的时间以完成传输
uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance)
{
    // 我们只使用一个接口和一个HID报告描述符，所以可以忽略参数'instance'
    return hid_report_descriptor;
}

// 当收到GET_REPORT控制请求时调用
// 应用程序必须填充缓冲区报告的内容并返回其长度
// 返回零将导致堆栈阻塞请求
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
    // 未使用的参数
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

    return 0;
}

// 当收到SET_REPORT控制请求或在OUT端点上收到数据时调用（Report ID = 0, Type = 0）
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    // 此函数为空，因为在此示例中我们不处理incoming reports
}

/********* 应用程序 ***************/

// 定义鼠标移动方向的枚举
typedef enum {
    MOUSE_DIR_RIGHT,
    MOUSE_DIR_DOWN,
    MOUSE_DIR_LEFT,
    MOUSE_DIR_UP,
    MOUSE_DIR_MAX,
} mouse_dir_t;

// 定义鼠标移动的最大距离和每次移动的步长
#define DISTANCE_MAX        125
#define DELTA_SCALAR        5

// 计算鼠标下一步移动的delta值
static void mouse_draw_square_next_delta(int8_t *delta_x_ret, int8_t *delta_y_ret)
{
    static mouse_dir_t cur_dir = MOUSE_DIR_RIGHT;
    static uint32_t distance = 0;

    // 根据当前方向计算下一个delta值
    if (cur_dir == MOUSE_DIR_RIGHT) {
        *delta_x_ret = DELTA_SCALAR;
        *delta_y_ret = 0;
    } else if (cur_dir == MOUSE_DIR_DOWN) {
        *delta_x_ret = 0;
        *delta_y_ret = DELTA_SCALAR;
    } else if (cur_dir == MOUSE_DIR_LEFT) {
        *delta_x_ret = -DELTA_SCALAR;
        *delta_y_ret = 0;
    } else if (cur_dir == MOUSE_DIR_UP) {
        *delta_x_ret = 0;
        *delta_y_ret = -DELTA_SCALAR;
    }

    // 更新当前方向的累计距离
    distance += DELTA_SCALAR;
    // 检查是否需要改变方向
    if (distance >= DISTANCE_MAX) {
        distance = 0;
        cur_dir++;
        if (cur_dir == MOUSE_DIR_MAX) {
            cur_dir = 0;
        }
    }
}

// 发送HID演示数据
static void app_send_hid_demo(void)
{
    // 键盘输出：发送按键'a/A'的按下和释放
    ESP_LOGI(TAG, "Sending Keyboard report");
    uint8_t keycode[6] = {HID_KEY_A};
    tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, 0, keycode);
    vTaskDelay(pdMS_TO_TICKS(50));
    tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, 0, NULL);

    // 鼠标输出：移动鼠标光标画一个正方形轨迹
    ESP_LOGI(TAG, "Sending Mouse report");
    int8_t delta_x;
    int8_t delta_y;
    for (int i = 0; i < (DISTANCE_MAX / DELTA_SCALAR) * 4; i++) {
        // 获取下一个x和y的delta值以画出正方形图案
        mouse_draw_square_next_delta(&delta_x, &delta_y);
        tud_hid_mouse_report(HID_ITF_PROTOCOL_MOUSE, 0x00, delta_x, delta_y, 0, 0);
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

// 主应用程序入口
void app_main(void)
{
    // 初始化用于触发HID报告的按钮
    const gpio_config_t boot_button_config = {
        .pin_bit_mask = BIT64(APP_BUTTON),
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_up_en = true,
        .pull_down_en = false,
    };
    ESP_ERROR_CHECK(gpio_config(&boot_button_config));

    // USB初始化
    ESP_LOGI(TAG, "USB initialization");
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = NULL,
        .string_descriptor = hid_string_descriptor,
        .string_descriptor_count = sizeof(hid_string_descriptor) / sizeof(hid_string_descriptor[0]),
        .external_phy = false,
#if (TUD_OPT_HIGH_SPEED)
        .fs_configuration_descriptor = hid_configuration_descriptor, // 全速和高速的HID配置描述符相同
        .hs_configuration_descriptor = hid_configuration_descriptor,
        .qualifier_descriptor = NULL,
#else
        .configuration_descriptor = hid_configuration_descriptor,
#endif // TUD_OPT_HIGH_SPEED
    };

    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
    ESP_LOGI(TAG, "USB initialization DONE");

    // 主循环
    while (1) {
        if (tud_mounted()) {
            static bool send_hid_data = true;
            if (send_hid_data) {
                app_send_hid_demo();
            }
            send_hid_data = !gpio_get_level(APP_BUTTON);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
