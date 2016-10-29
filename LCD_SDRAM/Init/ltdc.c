#include "ltdc.h"

LTDC_HandleTypeDef hltdc;  
extern LCD_LayerPropTypedef layer_prop[GUI_NUM_LAYERS];

void HAL_LTDC_MspInit(LTDC_HandleTypeDef *hltdc)
{  
  GPIO_InitTypeDef gpio_init_structure;
  static RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  
  /* Enable the LTDC clocks */
  __HAL_RCC_LTDC_CLK_ENABLE();
  
  
  /* LCD clock configuration */
  /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
  /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 429 Mhz */
  /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 429/5 = 85 Mhz */
  /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_2 = 85/4 = 21 Mhz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_4;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);   
  
  
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();
  
  /*** LTDC Pins configuration ***/
  /* GPIOE configuration */
  gpio_init_structure.Pin       = GPIO_PIN_4;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FAST;
  gpio_init_structure.Alternate = GPIO_AF14_LTDC;  
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);

  /* GPIOG configuration */
  gpio_init_structure.Pin       = GPIO_PIN_12;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Alternate = GPIO_AF9_LTDC;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);

  /* GPIOI LTDC alternate configuration */
  gpio_init_structure.Pin       = GPIO_PIN_9 | GPIO_PIN_10 | \
                                  GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOI, &gpio_init_structure);

  /* GPIOJ configuration */  
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                                  GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | \
                                  GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | \
                                  GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOJ, &gpio_init_structure);  

  /* GPIOK configuration */  
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | \
                                  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOK, &gpio_init_structure);
  
  /* LCD_DISP GPIO configuration */
  gpio_init_structure.Pin       = GPIO_PIN_12;     /* LCD_DISP pin has to be manually controlled */
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOI, &gpio_init_structure);

  /* LCD_BL_CTRL GPIO configuration */
  gpio_init_structure.Pin       = GPIO_PIN_3;  /* LCD_BL_CTRL pin has to be manually controlled */
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOK, &gpio_init_structure);

  /* Set LTDC Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(LTDC_IRQn, 0xE, 0);
  
  /* Enable LTDC Interrupt */
  HAL_NVIC_EnableIRQ(LTDC_IRQn);
}

void LTDC_Init()
{
	HAL_LTDC_DeInit(&hltdc);

	/* Set LCD Timings */
	hltdc.Init.HorizontalSync = (HSYNC - 1);
	hltdc.Init.VerticalSync = (VSYNC - 1);
	hltdc.Init.AccumulatedHBP = (HSYNC + HBP - 1);
	hltdc.Init.AccumulatedVBP = (VSYNC + VBP - 1);
	hltdc.Init.AccumulatedActiveH = (YSIZE_PHYS + VSYNC + VBP - 1);
	hltdc.Init.AccumulatedActiveW = (XSIZE_PHYS + HSYNC + HBP - 1);
	hltdc.Init.TotalHeigh = (YSIZE_PHYS + VSYNC + VBP + VFP - 1);
	hltdc.Init.TotalWidth = (XSIZE_PHYS + HSYNC + HBP + HFP - 1);

	/* background */
	hltdc.Init.Backcolor.Blue = 0;
	hltdc.Init.Backcolor.Green = 0;
	hltdc.Init.Backcolor.Red = 0;  

	/* Polarity */
	hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
	hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL; 
	hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;  
	hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
	hltdc.Instance = LTDC;

	HAL_LTDC_Init(&hltdc);
	HAL_LTDC_ProgramLineEvent(&hltdc, 0);

	/* Enable dithering */
	HAL_LTDC_EnableDither(&hltdc);
	
	/* Assert display enable LCD_DISP pin */
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_12, GPIO_PIN_SET);

	/* Assert backlight LCD_BL_CTRL pin */
	HAL_GPIO_WritePin(GPIOK, GPIO_PIN_3, GPIO_PIN_SET);
}

void LTDC_ConfigLayer(uint32_t LayerIndex)
{
	LTDC_LayerCfgTypeDef   layer_cfg;

	if (LayerIndex < GUI_NUM_LAYERS) 
	{ 
		/* Layer configuration */
		layer_cfg.WindowX0 = 0;
		layer_cfg.WindowX1 = XSIZE_PHYS;
		layer_cfg.WindowY0 = 0;
		layer_cfg.WindowY1 = YSIZE_PHYS; 
		layer_cfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
		layer_cfg.FBStartAdress = layer_prop[LayerIndex].address;
		layer_cfg.Alpha = 255;
		layer_cfg.Alpha0 = 0;
		layer_cfg.Backcolor.Blue = 0;
		layer_cfg.Backcolor.Green = 0;
		layer_cfg.Backcolor.Red = 0;
		layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
		layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
		layer_cfg.ImageWidth = XSIZE_PHYS;
		layer_cfg.ImageHeight = YSIZE_PHYS;
		HAL_LTDC_ConfigLayer(&hltdc, &layer_cfg, LayerIndex);  

		//HAL_LTDC_EnableCLUT(&hltdc, LayerIndex);
	}
}

uint32_t GetBufferSize(uint32_t LayerIndex) 
{
  uint32_t BufferSize;

  BufferSize = layer_prop[LayerIndex].xSize * layer_prop[LayerIndex].ySize * layer_prop[LayerIndex].BytesPerPixel;
  return BufferSize;
}

void HAL_LTDC_LineEvenCallback(LTDC_HandleTypeDef *hltdc)
{
	uint32_t Addr;
	uint32_t layer;

	for (layer = 0; layer < GUI_NUM_LAYERS; layer++)
	{
		if (layer_prop[layer].pending_buffer >= 0) 
		{
			/* Calculate address of buffer to be used  as visible frame buffer */
			Addr = layer_prop[layer].address + \
			layer_prop[layer].xSize * layer_prop[layer].ySize * layer_prop[layer].pending_buffer * layer_prop[layer].BytesPerPixel;

			__HAL_LTDC_LAYER(hltdc, layer)->CFBAR = Addr;
			__HAL_LTDC_RELOAD_CONFIG(hltdc);

			/* Notify STemWin that buffer is used */
			//GUI_MULTIBUF_ConfirmEx(layer, layer_prop[layer].pending_buffer);

			/* Clear pending buffer flag of layer */
			layer_prop[layer].pending_buffer = -1;
		}
	}

	HAL_LTDC_ProgramLineEvent(hltdc, 0);
}

void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef *hltdc)
{
  __HAL_RCC_LTDC_FORCE_RESET();
  __HAL_RCC_LTDC_RELEASE_RESET();
}
