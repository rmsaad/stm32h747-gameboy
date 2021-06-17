/*
 * stm32h7_display.c
 *
 *  Created on: Jun 14, 2021
 *      Author: Rami
 */

#include <stdint.h>
#include "main.h"
#include "image_320x240_argb8888.h"
#include "life_augmented_argb8888.h"

static DMA2D_HandleTypeDef           hdma2d;
extern LTDC_HandleTypeDef            hlcd_ltdc;

#define LAYER0_ADDRESS        0xD0000000

static __IO int32_t  front_buffer   = 0;
static __IO int32_t  pend_buffer   = -1;
static uint32_t ImageIndex    = 0;
static uint32_t LCD_X_Size = 800;
static uint32_t LCD_Y_Size = 480;

static const uint32_t * Images[] =
{
  image_320x240_argb8888,
  life_augmented_argb8888,
};

static const uint32_t Buffers[] =
{
  LAYER0_ADDRESS,
  LAYER0_ADDRESS + (720*576*4)
};


/**
  * @brief  Line Event callback.
  * @param  hltdc: pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval None
  */
void HAL_LTDC_LineEventCallback(LTDC_HandleTypeDef *hltdc)
{
  if(pend_buffer >= 0)
  {
    LTDC_LAYER(hltdc, 0)->CFBAR = ((uint32_t)Buffers[pend_buffer]);
    __HAL_LTDC_RELOAD_CONFIG(hltdc);

    front_buffer = pend_buffer;
    pend_buffer = -1;
  }

  HAL_LTDC_ProgramLineEvent(hltdc, 0);
}

void CopyBuffer(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize)
{

  uint32_t destination = (uint32_t)pDst + (y * LCD_X_Size + x) * 4;
  uint32_t source      = (uint32_t)pSrc;

  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/
  hdma2d.Init.Mode         = DMA2D_M2M;
  hdma2d.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  hdma2d.Init.OutputOffset = LCD_X_Size - xsize;
  hdma2d.Init.AlphaInverted = DMA2D_REGULAR_ALPHA;  /* No Output Alpha Inversion*/
  hdma2d.Init.RedBlueSwap   = DMA2D_RB_REGULAR;     /* No Output Red & Blue swap */

  /*##-2- DMA2D Callbacks Configuration ######################################*/
  hdma2d.XferCpltCallback  = NULL;

  /*##-3- Foreground Configuration ###########################################*/
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  hdma2d.LayerCfg[1].InputOffset = 0;
  hdma2d.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR; /* No ForeGround Red/Blue swap */
  hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No ForeGround Alpha inversion */

  hdma2d.Instance          = DMA2D;

  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hdma2d) == HAL_OK)
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK)
    {
      if (HAL_DMA2D_Start(&hdma2d, source, destination, xsize, ysize) == HAL_OK)
      {
        /* Polling For DMA transfer */
        HAL_DMA2D_PollForTransfer(&hdma2d, 100);
      }
    }
  }
}

void dummy2_code(uint32_t* gb_frame){
	CopyBuffer((uint32_t *) gb_frame, (uint32_t *)Buffers[0], 0, 0, 160 * 2, 144 * 2);
	LTDC_LAYER(&hlcd_ltdc, 0)->CFBAR = ((uint32_t)Buffers[0]);
	 //__HAL_LTDC_RELOAD_CONFIG(&hlcd_ltdc);
}

void dummy_code(){
	HAL_LTDC_ProgramLineEvent(&hlcd_ltdc, 0);
	  CopyBuffer((uint32_t *)Buffers[0], (uint32_t *)Buffers[1], 0, 0, LCD_X_Size, LCD_Y_Size);

	  while(1){
		  if(pend_buffer < 0)
		      {
		        /* Prepare back buffer */
		        CopyBuffer((uint32_t *)Images[ImageIndex++], (uint32_t *)Buffers[1- front_buffer], (LCD_X_Size - 320)/2, 160, 320, 240);
		        pend_buffer = 1- front_buffer;

		        if(ImageIndex >= 2)
		        {
		          ImageIndex = 0;
		        }

		        /* Wait some time before switching to next stage */

		      }
	  }
}
