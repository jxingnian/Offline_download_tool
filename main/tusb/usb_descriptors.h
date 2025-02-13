#ifndef _USB_DESCRIPTORS_H_
#define _USB_DESCRIPTORS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "tusb.h"

// 定义接口编号
enum {
    ITF_NUM_CDC = 0,
    ITF_NUM_CDC_DATA,
    ITF_NUM_HID,
    ITF_NUM_MSC,
    ITF_NUM_TOTAL
};

// 函数声明
uint8_t const *tud_descriptor_device_cb(void);
uint8_t const *tud_descriptor_configuration_cb(uint8_t index);
uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid);

#ifdef __cplusplus
}
#endif

#endif /* _USB_DESCRIPTORS_H_ */
