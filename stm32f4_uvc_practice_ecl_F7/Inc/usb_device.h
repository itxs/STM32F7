/*
 * usb_device.h
 *
 */

#ifndef INC_USB_DEVICE_H_
#define INC_USB_DEVICE_H_
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "usbd_def.h"
#include "usbd_desc.h"
#include "usbd_uvc.h"

extern USBD_HandleTypeDef hUsbDevice;

/* USB_Device init function */
void USB_DEVICE_Init(void);

#ifdef __cplusplus
}
#endif
#endif /* INC_USB_DEVICE_H_ */
