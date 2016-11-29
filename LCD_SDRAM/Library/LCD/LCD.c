#include "lcd.h"
#include "string.h"
#include "stdlib.h"

LCD_LayerPropTypedef layer_prop[GUI_NUM_LAYERS];

void LCD_Init()
{
	uint32_t i;

	LTDC_Init();
	
	DMA2D_Init();
	
	/*Initialize GUI Layer structure */
	layer_prop[0].address = LCD_LAYER0_FRAME_BUFFER;
	layer_prop[1].address = LCD_LAYER1_FRAME_BUFFER; 
	layer_prop[0].xSize = XSIZE_PHYS;
	layer_prop[0].ySize = YSIZE_PHYS; 
	layer_prop[1].xSize = XSIZE_PHYS;
	layer_prop[1].ySize = YSIZE_PHYS; 
	layer_prop[0].BytesPerPixel = 4;
	layer_prop[1].BytesPerPixel = 4;
	
	for (i = 0; i < GUI_NUM_LAYERS; i++) 
	{
		layer_prop[i].pending_buffer = -1;
		layer_prop[i].BytesPerPixel = 8;
	}
	
	LTDC_ConfigLayer(0);
	LTDC_ConfigLayer(1);
	memset((void*)layer_prop[0].address, 0, XSIZE_PHYS*YSIZE_PHYS * layer_prop[0].BytesPerPixel);
	memset((void*)layer_prop[1].address, 0, XSIZE_PHYS*YSIZE_PHYS * layer_prop[1].BytesPerPixel);
}

void LCD_LL_CopyBuffer(int LayerIndex, int IndexSrc, int IndexDst)
{
	uint32_t BufferSize, AddrSrc, AddrDst;

	BufferSize = GetBufferSize(LayerIndex);
	AddrSrc    = (uint32_t)layer_prop[LayerIndex].address + BufferSize * IndexSrc;
	AddrDst    = (uint32_t)layer_prop[LayerIndex].address + BufferSize * IndexDst;
	DMA2D_CopyBuffer(LayerIndex, (void *)AddrSrc, (void *)AddrDst, layer_prop[LayerIndex].xSize, layer_prop[LayerIndex].ySize, 0, 0);
	layer_prop[LayerIndex].buffer_index = IndexDst;
}

void LCD_LL_CopyRect(int LayerIndex, int x0, int y0, int x1, int y1, int xSize, int ySize)
{
	uint32_t AddrSrc, AddrDst;  

	AddrSrc = (uint32_t)layer_prop[LayerIndex].address + (y0 * layer_prop[LayerIndex].xSize + x0) * layer_prop[LayerIndex].BytesPerPixel;
	AddrDst = (uint32_t)layer_prop[LayerIndex].address + (y1 * layer_prop[LayerIndex].xSize + x1) * layer_prop[LayerIndex].BytesPerPixel;
	DMA2D_CopyBuffer(LayerIndex, (void *)AddrSrc, (void *)AddrDst, xSize, ySize, layer_prop[LayerIndex].xSize - xSize, layer_prop[LayerIndex].xSize - xSize);
}

void LCD_LL_FillRect(uint8_t layer, uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint32_t color, float opacity)
{
	DMA2D_FillBuffer((void*)(layer_prop[layer].address + (x0 + y0 * layer_prop[layer].xSize)), width, height, layer_prop[layer].xSize - width, color | (((uint32_t)(opacity * 255.0)) << 24));
}

void LCD_LL_Point(uint8_t layer, uint16_t x, uint16_t y, uint32_t color, float opacity)
{
	*(uint32_t*)(layer_prop[layer].address + x + y * layer_prop[layer].xSize) = color | (((uint32_t)(opacity * 255.0)) << 24);
}

void LCD_LL_Line(uint8_t layer, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color, float opacity)
{
	int16_t A, B, f = 0;
	int8_t sign, signa, signb;
	uint16_t x = x0, y = y0;
	
	A = y1 - y0;
	B = x0 - x1;
	sign = (abs(A) > abs(B)) ? 1 : -1;
	signa = (A < 0) ? -1 : 1;
	signb = (B < 0) ? -1 : 1;

	*(uint32_t*)(layer_prop[layer].address + x0 + y0 * layer_prop[layer].xSize) = color | (((uint32_t)(opacity * 255.0)) << 24);	
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
		if ((x > 0) && (y > 0) && (x < layer_prop[layer].xSize) && (y < layer_prop[layer].ySize))
		{
			*(uint32_t*)(layer_prop[layer].address + x + y * layer_prop[layer].xSize) = color | (((uint32_t)(opacity * 255.0)) << 24);
		}
	} while (x != x1 || y != y1);
}

void LCD_LL_DrawBitmap32bpp(int LayerIndex, int x, int y, uint8_t const * p, int xSize, int ySize, int BytesPerLine)
{
	uint32_t BufferSize, AddrDst;
	int OffLineSrc, OffLineDst;

	BufferSize = GetBufferSize(LayerIndex);
	AddrDst = (uint32_t)layer_prop[LayerIndex].address + BufferSize * layer_prop[LayerIndex].buffer_index + (y * layer_prop[LayerIndex].xSize + x) * layer_prop[LayerIndex].BytesPerPixel;
	OffLineSrc = (BytesPerLine / 4) - xSize;
	OffLineDst = layer_prop[LayerIndex].xSize - xSize;
	DMA2D_CopyBuffer(LayerIndex, (void *)p, (void *)AddrDst, xSize, ySize, OffLineSrc, OffLineDst);
}
