#include "lcd.h"

LCD_LayerPropTypedef layer_prop[GUI_NUM_LAYERS];

void LCD_Init()
{
	uint32_t i;

	LTDC_Init();
	
	DMA2D_Init();
	
	/*Initialize GUI Layer structure */
	layer_prop[0].address = LCD_LAYER0_FRAME_BUFFER;
	layer_prop[1].address = LCD_LAYER1_FRAME_BUFFER; 
	
	for (i = 0; i < GUI_NUM_LAYERS; i++) 
	{
		layer_prop[i].pending_buffer = -1;
		layer_prop[i].BytesPerPixel = 8;
	}
	
	LTDC_ConfigLayer(0);
	LTDC_ConfigLayer(1);
}

void LCD_LL_CopyBuffer(int LayerIndex, int IndexSrc, int IndexDst)
{
	uint32_t BufferSize, AddrSrc, AddrDst;

	BufferSize = GetBufferSize(LayerIndex);
	AddrSrc    = layer_prop[LayerIndex].address + BufferSize * IndexSrc;
	AddrDst    = layer_prop[LayerIndex].address + BufferSize * IndexDst;
	DMA2D_CopyBuffer(LayerIndex, (void *)AddrSrc, (void *)AddrDst, layer_prop[LayerIndex].xSize, layer_prop[LayerIndex].ySize, 0, 0);
	layer_prop[LayerIndex].buffer_index = IndexDst;
}

void LCD_LL_CopyRect(int LayerIndex, int x0, int y0, int x1, int y1, int xSize, int ySize)
{
	uint32_t AddrSrc, AddrDst;  

	AddrSrc = layer_prop[LayerIndex].address + (y0 * layer_prop[LayerIndex].xSize + x0) * layer_prop[LayerIndex].BytesPerPixel;
	AddrDst = layer_prop[LayerIndex].address + (y1 * layer_prop[LayerIndex].xSize + x1) * layer_prop[LayerIndex].BytesPerPixel;
	DMA2D_CopyBuffer(LayerIndex, (void *)AddrSrc, (void *)AddrDst, xSize, ySize, layer_prop[LayerIndex].xSize - xSize, layer_prop[LayerIndex].xSize - xSize);
}

void LCD_LL_FillRect(int LayerIndex, int x0, int y0, int x1, int y1, uint32_t PixelIndex)
{
	uint32_t BufferSize, AddrDst;
	int xSize, ySize;
	
	xSize = x1 - x0 + 1;
	ySize = y1 - y0 + 1;
	BufferSize = GetBufferSize(LayerIndex);
	AddrDst = layer_prop[LayerIndex].address + BufferSize * layer_prop[LayerIndex].buffer_index + (y0 * layer_prop[LayerIndex].xSize + x0) * layer_prop[LayerIndex].BytesPerPixel;
	DMA2D_FillBuffer(LayerIndex, (void *)AddrDst, xSize, ySize, layer_prop[LayerIndex].xSize - xSize, PixelIndex);
}

void LCD_LL_DrawBitmap32bpp(int LayerIndex, int x, int y, uint8_t const * p, int xSize, int ySize, int BytesPerLine)
{
	uint32_t BufferSize, AddrDst;
	int OffLineSrc, OffLineDst;

	BufferSize = GetBufferSize(LayerIndex);
	AddrDst = layer_prop[LayerIndex].address + BufferSize * layer_prop[LayerIndex].buffer_index + (y * layer_prop[LayerIndex].xSize + x) * layer_prop[LayerIndex].BytesPerPixel;
	OffLineSrc = (BytesPerLine / 4) - xSize;
	OffLineDst = layer_prop[LayerIndex].xSize - xSize;
	DMA2D_CopyBuffer(LayerIndex, (void *)p, (void *)AddrDst, xSize, ySize, OffLineSrc, OffLineDst);
}
