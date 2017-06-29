#ifndef __COLOR_MODELS
#define __COLOR_MODELS

#include "stdint.h"

typedef struct
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
} RGB_Pix;
	
typedef struct
{
	uint8_t Y;
	uint8_t U;
	uint8_t V;
} YUV_Pix;

RGB_Pix YUV_to_RGB(YUV_Pix* pix);
YUV_Pix RGB_to_YUV(RGB_Pix* pix);

#endif
