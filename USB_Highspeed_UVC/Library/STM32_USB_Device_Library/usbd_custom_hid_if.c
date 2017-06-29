#include "usbd_custom_hid_if.h"
#include "hid_definitions.h"

void USB_OutEventCallback(uint8_t* pData);
extern USBD_HandleTypeDef hUsbDeviceHS;  

__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_HS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
  	USAGE_PAGE(USAGE_PAGE_GENERIC_DESKTOP)
	USAGE(USAGE_SYSCTL)
	COLLECTION(APPLICATION)			//Коллекция репортов
		REPORT_ID(1)
			USAGE(1)
			LOGICAL_MIN(0)			//Минимально возможное передаваемое значение
			LOGICAL_MAX(255)		//Максимально возможное передаваемое значение
			REPORT_SIZE(8)			//Количество бит данных в единице репорта (байте)
			REPORT_COUNT(64)		//Количество репортов этого вида
			INPUT(DATA | VARIABLE | ABSOLUTE)
		REPORT_ID(2)
			USAGE(2)
			LOGICAL_MIN(0)			//Минимально возможное передаваемое значение
			LOGICAL_MAX(255)		//Максимально возможное передаваемое значение
			REPORT_SIZE(8)			//Количество бит данных в единице репорта (байте)
			REPORT_COUNT(64)		//Количество репортов этого вида
			OUTPUT(DATA | VARIABLE | ABSOLUTE)
	END_COLLECTION
}; 

static int8_t CUSTOM_HID_Init_HS(void);
static int8_t CUSTOM_HID_DeInit_HS(void);
static int8_t CUSTOM_HID_OutEvent_HS(uint8_t* pData);

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_HS = 
{
  CUSTOM_HID_ReportDesc_HS,
  CUSTOM_HID_Init_HS,
  CUSTOM_HID_DeInit_HS,
  CUSTOM_HID_OutEvent_HS,
};

/**
  * @brief  CUSTOM_HID_Init_HS
  *         Initializes the CUSTOM HID media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_Init_HS(void)
{  
	return 0;
}

/**
  * @brief  CUSTOM_HID_DeInit_HS
  *         DeInitializes the CUSTOM HID media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_DeInit_HS(void)
{
	return 0;
}

/**
  * @brief  CUSTOM_HID_OutEvent_HS
  *         Manage the CUSTOM HID class events       
  * @param  event_idx: event index
  * @param  state: event state
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_OutEvent_HS  (uint8_t* pData)
{ 
	USB_OutEventCallback(pData);
	return 0;
}
