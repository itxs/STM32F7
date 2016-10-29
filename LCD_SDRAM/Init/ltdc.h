#ifndef __LTDC_H
#define __LTDC_H

#include "stm32f7xx_hal.h"

/* LCD Timings */
#define  HSYNC            ((uint16_t)41)   /* Horizontal synchronization */
#define  HBP              ((uint16_t)13)   /* Horizontal back porch      */
#define  HFP              ((uint16_t)32)   /* Horizontal front porch     */
#define  VSYNC            ((uint16_t)10)   /* Vertical synchronization   */
#define  VBP              ((uint16_t)2)    /* Vertical back porch        */
#define  VFP              ((uint16_t)2)    /* Vertical front porch       */

#define LCD_SWAP_XY  1 
#define LCD_MIRROR_Y 1

#define LCD_SWAP_RB  1

#define XSIZE_PHYS 480
#define YSIZE_PHYS 272

#define NUM_BUFFERS  3 /* Number of multiple buffers to be used */
#define NUM_VSCREENS 1 /* Number of virtual screens to be used */

#define BK_COLOR GUI_DARKBLUE

#define GUI_NUM_LAYERS 2

#define COLOR_CONVERSION_0 GUICC_M8888I

#define LCD_LAYER0_FRAME_BUFFER  ((uint32_t)0xC0200000)
#define LCD_LAYER1_FRAME_BUFFER  ((uint32_t)0xC0400000)

typedef struct
{
  uint32_t	address;          
  int32_t	pending_buffer;   
  uint32_t	buffer_index;     
  uint32_t		xSize;            
  uint32_t		ySize;            
  uint32_t		BytesPerPixel;
} LCD_LayerPropTypedef;

void LTDC_Init();
void LTDC_ConfigLayer(uint32_t LayerIndex);
inline uint32_t LTDC_GetPixelformat(uint32_t LayerIndex);
uint32_t GetBufferSize(uint32_t LayerIndex);

#endif

