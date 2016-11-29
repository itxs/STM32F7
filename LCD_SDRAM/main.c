#include "main.h"
#include "stdlib.h"
#include "math.h"

__IO uint32_t i = 0;

int main(void)
{
	srand(170);
	HAL_Init();
	RCC_Init();
	GPIO_Init();
	SDRAM_Init();
	LCD_Init();
	LCD_LL_FillRect(0, 0, 0, 480, 272, 0, 1);
	while (1)
	{
		//LCD_LL_FillRect(0, 0, 60, 479, 2, 0xFFFFFF, 1);
		LCD_LL_Line(0, 0, 60, 479, 62, 0xFFFFFF, 1);
		i++;
	}
}

void Error_Handler(void)
{
	while(1);
}
