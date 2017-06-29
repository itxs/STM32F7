#include "stm32f7xx_hal.h"
#include "stdlib.h"

extern uint8_t gBuffer[];

extern PCD_HandleTypeDef hpcd_USB_OTG;
void SystemClock_Config(void);
void USB_DEVICE_Init(void);

void UVC_Line(uint8_t* buffer, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color)
{
	int16_t A, B, f = 0;
	int8_t sign, signa, signb;
	uint16_t x = x0, y = y0;
	
	A = y1 - y0;
	B = x0 - x1;
	sign = (abs(A) > abs(B)) ? 1 : -1;
	signa = (A < 0) ? -1 : 1;
	signb = (B < 0) ? -1 : 1;

	buffer[x0 * 3 + y0 * 160 * 3] = (uint8_t)color;
	buffer[x0 * 3 + y0 * 160 * 3 + 1] = (uint8_t)(color >> 8);
	buffer[x0 * 3 + y0 * 160 * 3 + 2] = (uint8_t)(color >> 16);
	
	do {
		(sign == 1) ? (f += B * signb) : (f += A * signa);
		if (f > 0)
		{
			if (sign == 1)
			{
				f -= A * signa;
				x -= signb;
			}
			else
			{
				f -= B * signb;
				y += signa;
			}
		}
		(sign == 1) ? (y += signa) : (x -= signb);
		if ((x > 0) && (y > 0) && (x < 160) && (y < 120))
		{
			buffer[x * 3 + y * 160 * 3] = (uint8_t)color;
			buffer[x * 3 + y * 160 * 3 + 1] = (uint8_t)(color >> 8);
			buffer[x * 3 + y * 160 * 3 + 2] = (uint8_t)(color >> 16);
		}
	} while (x != x1 || y != y1);
}

int main(void)
{
	SCB_EnableICache();
	SCB_EnableDCache();
	
	srand(719386);
	
	HAL_Init();
	SystemClock_Config();

	//LCD backlight turn off
	GPIO_InitTypeDef gpio;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOK, &gpio);
	HAL_GPIO_WritePin(GPIOK, GPIO_PIN_3, GPIO_PIN_RESET);
	
	USB_DEVICE_Init();
	
	UVC_Line(gBuffer,  rand() % 160, rand() % 160, rand() % 120, rand() % 120, ((rand() % 255) << 16) | ((rand() % 255) << 8) | (rand() % 255));
	
	while (1)
	{
		UVC_Line(gBuffer,  rand() % 160, rand() % 160, rand() % 120, rand() % 120, ((rand() % 255) << 16) | ((rand() % 255) << 8) | (rand() % 255));
		HAL_Delay(10);
	}
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
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
	RCC_OscInitStruct.PLL.PLLN = 432;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 9;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
				|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
	
	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();
	__GPIOH_CLK_ENABLE();
	__GPIOK_CLK_ENABLE();
	__SYSCFG_CLK_ENABLE();
}

#ifdef USE_FULL_ASSERT

/**
	 * @brief Reports the name of the source file and the source line number
	 * where the assert_param error has occurred.
	 * @param file: pointer to the source file name
	 * @param line: assert_param error line source number
	 * @retval None
	 */
void assert_failed(uint8_t* file, uint32_t line)
{
}

#endif

/**
	* @}
	*/ 

/**
	* @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
