#ifndef __LCD_H
#define __LCD_H

#include "ltdc.h"
#include "dma2d.h"

typedef struct
{
	uint16_t x;
	uint16_t y;
	struct Point_t* NextPoint;
} Point_t;

typedef struct
{
	uint16_t x0;
	uint16_t y0;
	uint16_t x1;
	uint16_t y1;
} Line_t;

typedef struct
{
	uint16_t x0;
	uint16_t y0;
	uint16_t x1;
	uint16_t y1;
} Rectangle_t;

typedef struct
{
	uint16_t x0;
	uint16_t y0;
	uint16_t r;
	uint16_t flat;
} Ellipse_t;

typedef struct
{
	uint16_t x0;
	uint16_t y0;
	uint16_t w;
	uint16_t h;
	uint32_t* buffer;
} Image_t;

typedef struct
{
	struct GraphicPrimitive* Children;
	uint8_t		layer;
	uint8_t		id;
	uint8_t		visible;
	union
	{
		Point_t		Point;
		Line_t		Line;
		Rectangle_t Rect;
		Ellipse_t	Ellipse;
		Image_t		Image;
	};
	uint32_t	color;
	uint8_t		opacity;
	void		(*Draw)();
	void		(*Rotate)(int32_t);
	void		(*Delete)();
} GraphicPrimitive; //40 bytes

void LCD_Init();
void LCD_LL_CopyBuffer(int LayerIndex, int IndexSrc, int IndexDst);
void LCD_LL_FillRect(uint8_t layer, uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint32_t color, uint8_t opacity);
void LCD_LL_Line(uint8_t layer, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color, uint8_t opacity);
void LCD_LL_Ellipse(uint8_t layer, uint16_t x, uint16_t y, uint16_t r, uint16_t x_flattening, uint32_t color, uint8_t opacity);
void LCD_LL_Point(uint8_t layer, uint16_t x, uint16_t y, uint32_t color, uint8_t opacity);
void LCD_LL_CopyRect(int LayerIndex, int x0, int y0, int x1, int y1, int xSize, int ySize);
void LCD_LL_DrawBitmap32bpp(int LayerIndex, int x, int y, uint8_t const * p, int xSize, int ySize, int BytesPerLine);

#endif
