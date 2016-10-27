#include "main.h"


int main(void)
{
	HAL_Init();
	RCC_Init();
	GPIO_Init();
	USB_Init();
	
	while (1);
}

void Error_Handler(void)
{
	while(1);
}
