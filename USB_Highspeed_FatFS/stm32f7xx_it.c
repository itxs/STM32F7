#include "stm32f7xx_hal.h"
#include "stm32f7xx_it.h"

extern PCD_HandleTypeDef hpcd_USB_OTG_HS;
extern QSPI_HandleTypeDef QSPIHandle;

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

/**
* @brief This function handles USB On The Go HS global interrupt.
*/
void OTG_HS_IRQHandler(void)
{
	HAL_PCD_IRQHandler(&hpcd_USB_OTG_HS);
}

/**
  * @brief  This function handles QUADSPI interrupt request.
  * @param  None
  * @retval None
  */
void QUADSPI_IRQHandler(void)
{
  HAL_QSPI_IRQHandler(&QSPIHandle);
}

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
	HAL_RCC_NMI_IRQHandler();
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
	while (1)
	{
	}
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
	while (1)
	{
	}
}

/**
* @brief This function handles Pre-fetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
	while (1)
	{
	}
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
	while (1)
	{
	}
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
}
