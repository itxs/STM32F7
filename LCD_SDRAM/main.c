#include "main.h"

int main(void)
{
	HAL_Init();
	RCC_Init();
	GPIO_Init();
	SDRAM_Init();
	LCD_Init();
	
	while (1)
	{
		HAL_Delay(100);
		LCD_LL_FillRect(0, 1, 1, 6, 6, 0xAAAAAAAA);
	}
}

void Error_Handler(void)
{
	while(1);
}
