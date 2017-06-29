#include "stm32f7xx_hal.h"
#include "stm32f7xx.h"
#include "stm32f7xx_it.h"

extern PCD_HandleTypeDef hpcd_USB_OTG;

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
	while (1);
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
	while (1);
}

/**
* @brief This function handles Pre-fetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
	while (1);
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
	while (1);
}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
	while (1);
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

/**
* @brief This function handles USB On The Go FS global interrupt.
*/
void OTG_FS_IRQHandler(void)
{
	HAL_PCD_IRQHandler(&hpcd_USB_OTG);
}

void OTG_HS_IRQHandler(void)
{
	HAL_PCD_IRQHandler(&hpcd_USB_OTG);
}
