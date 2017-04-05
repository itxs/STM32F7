#include "dma2d.h"

DMA2D_HandleTypeDef hDMA2D;

void DMA2D_Init()
{
	__HAL_RCC_DMA2D_CLK_ENABLE(); 
}

void DMA2D_CopyBuffer(uint32_t LayerIndex, void * pSrc, void * pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLineSrc, uint32_t OffLineDst)
{
	DMA2D->CR      = DMA2D_MODE_M2M | DMA2D_CR_TCIE;

	/* Set up pointers */
	DMA2D->FGMAR   = (uint32_t)pSrc;
	DMA2D->OMAR    = (uint32_t)pDst;
	DMA2D->FGOR    = OffLineSrc;
	DMA2D->OOR     = OffLineDst;

	/* Set up pixel format */
	DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888;

	/*  Set up size */
	DMA2D->NLR     = (uint32_t)(xSize << 16) | (uint16_t)ySize; 

	DMA2D->CR     |= DMA2D_CR_START;

	/* Wait until transfer is done */
	while (DMA2D->CR & DMA2D_CR_START);
}

void DMA2D_FillBuffer(void * pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex)
{
	/* Set up mode */
	DMA2D->CR      = DMA2D_MODE_R2M | DMA2D_IT_TC;
	DMA2D->OCOLR   = ColorIndex;

	/* Set up pointers */
	DMA2D->OMAR    = (uint32_t)pDst;

	/* Set up offsets */
	DMA2D->OOR     = OffLine;

	/* Set up pixel format */
	DMA2D->OPFCCR  = LTDC_PIXEL_FORMAT_ARGB8888;

	/*  Set up size */
	DMA2D->NLR     = (uint32_t)(xSize << 16) | (uint16_t)ySize;

	DMA2D->CR |= DMA2D_CR_START; 
	while (DMA2D->CR & DMA2D_CR_START);
}
