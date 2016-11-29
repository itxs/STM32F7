#ifndef __DMA2D_H
#define __DMA2D_H

#include "stm32f7xx_hal.h"

#define DMA2D_MODE_M2M				(0 << 16)
#define DMA2D_MODE_M2M_PFC			(1 << 16)
#define DMA2D_MODE_M2M_BLEND		(2 << 16)
#define DMA2D_MODE_R2M				(3 << 16)

#define DMA2D_IT_CONF_ERROR			DMA2D_CR_CEIE
#define DMA2D_IT_CLUT_TC			DMA2D_CR_CTCIE
#define DMA2D_IT_CLUT_ACCESS_ERROR	DMA2D_CR_CAEIE
#define DMA2D_IT_TWATERMARK			DMA2D_CR_TWIE
#define DMA2D_ABORT					DMA2D_CR_ABORT
#define DMA2D_SUSPEND				DMA2D_CR_SUSP
#define DMA2D_START					DMA2D_CR_START


void DMA2D_Init();
void DMA2D_CopyBuffer(uint32_t LayerIndex, void * pSrc, void * pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLineSrc, uint32_t OffLineDst);
void DMA2D_FillBuffer(void * pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex);

#endif
