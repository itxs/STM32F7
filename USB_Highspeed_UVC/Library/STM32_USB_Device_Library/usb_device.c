#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_customhid.h"
#include "usbd_custom_hid_if.h"
#include "usbd_usr.h"

#pragma data_alignment=4
USBD_HandleTypeDef hUsbDeviceHS;

extern USBD_Usr_cb_TypeDef USR_cb;
extern USBD_ClassTypeDef USBD_UVC;

void USB_Init()
{
	USBD_Init(&hUsbDeviceHS, &USR_desc, DEVICE_HS);
	hUsbDeviceHS.pClass = &USBD_UVC;
	hUsbDeviceHS.pUserData = &USR_cb;
	USBD_Start(&hUsbDeviceHS);
}
