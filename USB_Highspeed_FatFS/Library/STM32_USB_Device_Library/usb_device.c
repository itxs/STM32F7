#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_msc.h"
#include "usbd_msc_storage.h"

USBD_HandleTypeDef hUsbDeviceHS;
		        
void USB_Init()
{
	/* Init Device Library */
	USBD_Init(&hUsbDeviceHS, &MSC_Desc, 0);

	/* Add Supported Class */
	USBD_RegisterClass(&hUsbDeviceHS, USBD_MSC_CLASS);

	/* Add Storage callbacks for MSC Class */
	USBD_MSC_RegisterStorage(&hUsbDeviceHS, &USBD_MSC_fops);

	/* Start Device Process */
	USBD_Start(&hUsbDeviceHS);
}

/* Коллбек получения данных по USB от хоста */
void USB_OutEventCallback(uint8_t* pData)
{
	
}
