#include "main.h"
#include "stuff.h"
#include "math.h"

TS_StateTypeDef touches;
struct
{
	uint16_t x;
	uint16_t y;
	uint16_t w;
	uint16_t h;
} button;

uint8_t draw_finished = 0;

int main()
{
	SCB_EnableICache();
	SCB_EnableDCache();
	
	HAL_Init();
	RCC_Init();

	BSP_LCD_Init();
	BSP_LCD_LayerRgb565Init(0, LCD_FB_START_ADDRESS);
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_DisplayOn();
	BSP_LCD_Clear(0);
	BSP_LCD_SetBackColor(0);
	BSP_LCD_SetTextColor(0xFFFFFF);
	
	BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
	
	button.x = 200;
	button.y = 100;
	button.w = 40;
	button.h = 25;

	while(1)
	{
		BSP_TS_GetState(&touches);
		uint8_t string[10];
		
		if (touches.touchDetected)
		{
			draw_finished = 0;
			BSP_LCD_SetTextColor(0xFF000000);
			BSP_LCD_FillEllipse(button.x, button.y, button.w, button.h);
			
			sprintf((char*)string, "%3d %3d", touches.touchX[0], touches.touchY[0]);
			button.x = touches.touchX[0];
			button.y = touches.touchY[0];
			if (touches.touchDetected > 1)
			{
				button.w = (uint16_t)fabs(touches.touchX[0] - touches.touchX[1]) / 2;
				button.h = (uint16_t)fabs(touches.touchY[0] - touches.touchY[1]) / 2;
				if (button.w < 5)
					button.w = 5;
				if (button.h < 5)
					button.h = 5;
				if (button.w > 75)
					button.w = 75;
				if (button.h > 75)
					button.h = 75;
			}
			BSP_LCD_SetTextColor(0x00FF1F1F);
			BSP_LCD_FillEllipse(button.x, button.y, button.w, button.h);
			BSP_LCD_SetTextColor(0xFF7F1F1F);
			BSP_LCD_FillEllipse(button.x, button.y, button.w - 3, button.h - 3);
			BSP_LCD_SetBackColor(0xFF8F8F8F);
			BSP_LCD_SetTextColor(0xFFFFFFFF);
			BSP_LCD_DisplayStringAt(button.x - 7, button.y - 7, "OK", LEFT_MODE);
			BSP_LCD_SetBackColor(0);
		}
		else
		{
			if (!draw_finished)
			{
				sprintf((char*)string, "%3d %3d", 0, 0);
				BSP_LCD_SetTextColor(0x00FFFFFF);
				BSP_LCD_FillEllipse(button.x, button.y, button.w, button.h);
				BSP_LCD_SetTextColor(0xFF8F8F8F);
				BSP_LCD_FillEllipse(button.x, button.y, button.w - 3, button.h - 3);
				BSP_LCD_SetBackColor(0xFF8F8F8F);
				BSP_LCD_SetTextColor(0xFFFFFFFF);
				BSP_LCD_DisplayStringAt(button.x - 7, button.y - 7, "OK", LEFT_MODE);
				BSP_LCD_SetBackColor(0);
				draw_finished = 1;
			}
		}
		BSP_LCD_SetTextColor(0xFFFFFFFF);
		BSP_LCD_DisplayStringAtLine(0, string);
	}
}
