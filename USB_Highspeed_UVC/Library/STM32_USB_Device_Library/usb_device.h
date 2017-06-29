#ifndef __usb_device_H
#define __usb_device_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "usbd_def.h"

extern USBD_HandleTypeDef hUsbDeviceHS;

void USB_Init();

#ifdef __cplusplus
}
#endif
#endif /*__usb_device_H */
