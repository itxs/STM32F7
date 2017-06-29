/**
	******************************************************************************
	* File Name			: stm32f7xx_hal_msp.c
	* Description		: This file provides code for the MSP Initialization 
	*						and de-Initialization codes.
	******************************************************************************
	*
	* COPYRIGHT(c) 2016 STMicroelectronics
	*
	* Redistribution and use in source and binary forms, with or without modification,
	* are permitted provided that the following conditions are met:
	*	 1. Redistributions of source code must retain the above copyright notice,
	*		this list of conditions and the following disclaimer.
	*	 2. Redistributions in binary form must reproduce the above copyright notice,
	*		this list of conditions and the following disclaimer in the documentation
	*		and/or other materials provided with the distribution.
	*	 3. Neither the name of STMicroelectronics nor the names of its contributors
	*		may be used to endorse or promote products derived from this software
	*		without specific prior written permission.
	*
	* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	*
	******************************************************************************
	*/
/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

extern DMA_HandleTypeDef hdma_dcmi;

/**
	* Initializes the Global MSP.
	*/
void HAL_MspInit(void)
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	/* System interrupt init*/
	/* MemoryManagement_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	/* BusFault_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	/* UsageFault_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
	/* DebugMonitor_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void HAL_PCD_MspInit(PCD_HandleTypeDef* hpcd)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	if(hpcd->Instance==USB_OTG_FS)
	{
	/**USB_OTG_FS GPIO Configuration	
	PA11	 ------> USB_OTG_FS_DM
	PA12	 ------> USB_OTG_FS_DP 
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	__HAL_RCC_USB_OTG_FS_CLK_ENABLE();
	HAL_NVIC_SetPriority(OTG_FS_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
	}
	else if(hpcd->Instance==USB_OTG_HS)
	{
	/**USB_OTG_HS GPIO Configuration	
	PB5	 ------> USB_OTG_HS_ULPI_D7
	PH4	 ------> USB_OTG_HS_ULPI_NXT
	PB13	 ------> USB_OTG_HS_ULPI_D6
	PB12	 ------> USB_OTG_HS_ULPI_D5
	PC0	 ------> USB_OTG_HS_ULPI_STP
	PC2	 ------> USB_OTG_HS_ULPI_DIR
	PA5	 ------> USB_OTG_HS_ULPI_CK
	PB10	 ------> USB_OTG_HS_ULPI_D3
	PA3	 ------> USB_OTG_HS_ULPI_D0
	PB1	 ------> USB_OTG_HS_ULPI_D2
	PB0	 ------> USB_OTG_HS_ULPI_D1
	PB11	 ------> USB_OTG_HS_ULPI_D4 
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_13|GPIO_PIN_12|GPIO_PIN_10 
							|GPIO_PIN_1|GPIO_PIN_0|GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	__HAL_RCC_USB_OTG_HS_CLK_ENABLE();
	__HAL_RCC_USB_OTG_HS_ULPI_CLK_ENABLE();

	HAL_NVIC_SetPriority(OTG_HS_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
	}
}

void HAL_PCD_MspDeInit(PCD_HandleTypeDef* hpcd)
{

	if(hpcd->Instance==USB_OTG_FS)
	{
	/* USER CODE BEGIN USB_OTG_FS_MspDeInit 0 */

	/* USER CODE END USB_OTG_FS_MspDeInit 0 */
	/* Peripheral clock disable */
	__HAL_RCC_USB_OTG_FS_CLK_DISABLE();
	
	/**USB_OTG_FS GPIO Configuration	
	PA11	 ------> USB_OTG_FS_DM
	PA12	 ------> USB_OTG_FS_DP 
	*/
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

	/* Peripheral interrupt DeInit*/
	HAL_NVIC_DisableIRQ(OTG_FS_IRQn);

	}
	else if(hpcd->Instance==USB_OTG_HS)
	{
	__HAL_RCC_USB_OTG_HS_CLK_DISABLE();
	__HAL_RCC_USB_OTG_HS_ULPI_CLK_DISABLE();
	
	/**USB_OTG_HS GPIO Configuration	
	PB5	 ------> USB_OTG_HS_ULPI_D7
	PH4	 ------> USB_OTG_HS_ULPI_NXT
	PB13	 ------> USB_OTG_HS_ULPI_D6
	PB12	 ------> USB_OTG_HS_ULPI_D5
	PC0	 ------> USB_OTG_HS_ULPI_STP
	PC2	 ------> USB_OTG_HS_ULPI_DIR
	PA5	 ------> USB_OTG_HS_ULPI_CK
	PB10	 ------> USB_OTG_HS_ULPI_D3
	PA3	 ------> USB_OTG_HS_ULPI_D0
	PB1	 ------> USB_OTG_HS_ULPI_D2
	PB0	 ------> USB_OTG_HS_ULPI_D1
	PB11	 ------> USB_OTG_HS_ULPI_D4 
	*/
	HAL_GPIO_DeInit(GPIOB, GPIO_PIN_5|GPIO_PIN_13|GPIO_PIN_12|GPIO_PIN_10 
							|GPIO_PIN_1|GPIO_PIN_0|GPIO_PIN_11);
	HAL_GPIO_DeInit(GPIOH, GPIO_PIN_4);
	HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0|GPIO_PIN_2);
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_3);
	HAL_NVIC_DisableIRQ(OTG_HS_IRQn);
	}
}

/**
	* @}
	*/

/**
	* @}
	*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
