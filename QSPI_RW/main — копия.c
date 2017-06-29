#include "main.h"
#include "stuff.h"

uint8_t data[] = "QSPI DIRECT READ/WRITE TEST PROJECT!!!";
uint8_t received[sizeof(data)];

TS_StateTypeDef touches;

int main()
{
	SCB_EnableICache();
	SCB_EnableDCache();
	
	HAL_Init();
	RCC_Init();
	
	//Init green LED
	IOCLK(I);
	OUT(I, P1);	
	
	QSPI_Init();
	QSPI_EraseBlock(QSPI_ADDRESS);
	QSPI_Write(data, QSPI_ADDRESS, sizeof(data));
	QSPI_Read(received, QSPI_ADDRESS, sizeof(data));
	
	BSP_TS_Init(800, 480);
	
	BSP_LCD_Init();
	BSP_LCD_LayerRgb565Init(0, LCD_FB_START_ADDRESS);
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_DisplayOn();
	BSP_LCD_Clear(0);
	BSP_LCD_SetBackColor(0);
	BSP_LCD_SetTextColor(0xFFFFFF);
	BSP_LCD_DisplayStringAtLine(0, received);
	BSP_LCD_FillEllipse(200, 100, 150, 80);
	
	while(1)
	{
		IOSET(PI1);
		HAL_Delay(10);
		//QSPI_Read(received, QSPI_ADDRESS, sizeof(data));
		IORESET(PI1);
		HAL_Delay(10);
		BSP_TS_GetState(&touches);
		uint8_t string[10];
		if (touches.touchDetected)
		{
			sprintf((char*)string, "%3d %3d", touches.touchX[0], touches.touchY[0]);
		}
		else
		{

		}
		
		BSP_LCD_DisplayStringAtLine(0, string);
	}
}
