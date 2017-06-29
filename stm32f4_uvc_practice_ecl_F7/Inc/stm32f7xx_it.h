/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __stm32f7xx_IT_H
#define __stm32f7xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void DebugMon_Handler(void);
void SysTick_Handler(void);
void OTG_FS_IRQHandler(void);
void OTG_HS_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __stm32f7xx_IT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
