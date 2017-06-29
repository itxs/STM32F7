#include "main.h"

int main()
{
	SCB_EnableICache();
	SCB_EnableDCache();
	
	HAL_Init();
	RCC_Init();
	BSP_QSPI_Init();
	
	GPIO_Init();
	USB_Init();
	HAL_GPIO_WritePin(GPIOK, GPIO_PIN_3, GPIO_PIN_RESET);
	while(1)
	{
		HAL_Delay(100);
		
	}
}

void Error_Handler()
{
	while(1);
}
