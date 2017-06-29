
/* Includes ------------------------------------------------------------------*/
#include "main.h"
//Send static NV12 image to PC by UVC
//By ILIASAM

#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_video_core.h"

__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

__IO uint8_t UserButtonPressed = 0;

void RCC_Init()
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	__HAL_RCC_PWR_CLK_ENABLE();

	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 400;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 5;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	HAL_PWREx_EnableOverDrive();

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
							  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
	
	HAL_RCC_EnableCSS();

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

int main(void)
{
	RCC_Init();

	USBD_Init(&USB_OTG_dev, USB_OTG_HS_CORE_ID, &USR_desc, &VIDEO_cb, &USR_cb);

	while(1);
}



void delay_ms(uint32_t ms)
{
	HAL_Delay(ms);
}


