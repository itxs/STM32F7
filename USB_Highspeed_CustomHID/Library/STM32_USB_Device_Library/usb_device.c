#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_customhid.h"
#include "usbd_custom_hid_if.h"

USBD_HandleTypeDef hUsbDeviceHS;
		        
void USB_Init()
{
	USBD_Init(&hUsbDeviceHS, &HS_Desc, DEVICE_HS);
	USBD_RegisterClass(&hUsbDeviceHS, &USBD_CUSTOM_HID);
	USBD_CUSTOM_HID_RegisterInterface(&hUsbDeviceHS, &USBD_CustomHID_fops_HS);
	USBD_Start(&hUsbDeviceHS);
}

/* Коллбек получения данных по USB от хоста */
void USB_OutEventCallback(uint8_t* pData)
{
	
}

/* Функция записи данных в буфер отправки по USB в хост */
int8_t USBD_CUSTOM_HID_SendReport_HS(uint8_t *pData, uint8_t size)
{
	return USBD_CUSTOM_HID_SendReport(&hUsbDeviceHS, pData, size);  
}
