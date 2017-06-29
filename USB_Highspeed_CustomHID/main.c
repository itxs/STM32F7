#include "main.h"

int main(void)
{
	HAL_Init();
	RCC_Init();
	GPIO_Init();
	USB_Init();
	HAL_GPIO_WritePin(GPIOK, GPIO_PIN_3, GPIO_PIN_RESET);
	while (1);
}

void Error_Handler(void)
{
	while(1);
}
