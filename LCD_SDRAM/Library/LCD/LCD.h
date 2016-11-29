#ifndef __LCD_H
#define __LCD_H

#include "ltdc.h"
#include "dma2d.h"

void LCD_Init();
void LCD_LL_CopyBuffer(int LayerIndex, int IndexSrc, int IndexDst);
void LCD_LL_FillRect(uint8_t layer, uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint32_t color, float opacity);
void LCD_LL_Line(uint8_t layer, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color, float opacity);
void LCD_LL_Point(uint8_t layer, uint16_t x, uint16_t y, uint32_t color, float opacity);
void LCD_LL_CopyRect(int LayerIndex, int x0, int y0, int x1, int y1, int xSize, int ySize);
void LCD_LL_DrawBitmap32bpp(int LayerIndex, int x, int y, uint8_t const * p, int xSize, int ySize, int BytesPerLine);

#endif
