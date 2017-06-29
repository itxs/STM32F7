/*
 * usb_device.c
 *
 *	Created on: 2 èþë. 2016 ã.
 *		Author: coel
 */

#include "usb_device.h"
#include "usbd_core.h"

/* USB Device Core handle declaration */
USBD_HandleTypeDef hUsbDevice;

/* init function */
void USB_DEVICE_Init(void)
{
	/* Init Device Library,Add Supported Class and Start the library*/
	
	//For change to Highspeed mode on STM32F746G-Disco just replace
	//DEVICE_HS to DEVICE_FS and switch usb cable to fullspeed connector
	USBD_Init(&hUsbDevice, &FS_Desc, DEVICE_HS);
	USBD_RegisterClass(&hUsbDevice, &USBD_UVC);
	USBD_Start(&hUsbDevice);
}

