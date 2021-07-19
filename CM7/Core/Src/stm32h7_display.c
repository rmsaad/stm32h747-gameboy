/**
  ******************************************************************************
  * @file           : stm32h7_display.c
  * @brief          : STM32H747-DISCO Display Functions
  *                   This file Handles copying contents of the work buffer(s) to the frame
  *                   buffer and updating the LCD screen.
  * @author         : Rami
  * @date           : Jun 14, 2021
  *
  ******************************************************************************
  *
  ******************************************************************************
  */

#include <stdint.h>
#include "main.h"

// colors
#define DARKEST_GREEN  0XFF0F380FUL
#define DARK_GREEN     0XFF306230UL
#define LIGHT_GREEN    0XFF8BAC0FUL
#define LIGHTEST_GREEN 0XFF9BBC0FUL

static DMA2D_HandleTypeDef  hdma2d;
extern LTDC_HandleTypeDef   hlcd_ltdc;
extern uint8_t  ucLY;
#define LAYER0_ADDRESS      0xD0000000


static uint32_t LCD_X_Size = 800;


static const uint32_t Buffers[] =
{
  LAYER0_ADDRESS,
  LAYER0_ADDRESS + (720*576*4)
};

// L8 CLUT
static uint32_t clut_argb8888[256];
DMA2D_CLUTCfgTypeDef clut_cfg;

/**
 * @brief Sets the color palette for the L8 indirect color mode used for the gameboy
 * @returns Nothing
 */
void stm32h7_displaySetPalette(){

	clut_argb8888[1] = LIGHTEST_GREEN;
	clut_argb8888[2] = LIGHT_GREEN;
	clut_argb8888[3] = DARK_GREEN;
	clut_argb8888[4] = DARKEST_GREEN;

	clut_cfg.pCLUT = clut_argb8888;
	clut_cfg.CLUTColorMode = DMA2D_CCM_ARGB8888;
	clut_cfg.Size = 255;
}


/**
 * @brief Copies Source buffer into the frame buffer, applies CLUT loading
 * @param pSrc Source buffer
 * @param pDst Destination buffer
 * @param x The coordinate x position of the buffer on the display
 * @param y The coordinate y position of the buffer on the display
 * @param xsize The x size of the buffer
 * @param ysize The y size of the buffer
 * @returns Nothing
 */
void CopyBuffer(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize){

    uint32_t destination = (uint32_t)pDst + (y * LCD_X_Size + x) * 4;
    uint32_t source      = (uint32_t)pSrc;

    /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/
    hdma2d.Init.Mode         = DMA2D_M2M_PFC;
    hdma2d.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
    hdma2d.Init.OutputOffset = LCD_X_Size - xsize;
    hdma2d.Init.AlphaInverted = DMA2D_REGULAR_ALPHA;  /* No Output Alpha Inversion*/
    hdma2d.Init.RedBlueSwap   = DMA2D_RB_REGULAR;     /* No Output Red & Blue swap */

    /*##-2- DMA2D Callbacks Configuration ######################################*/
    hdma2d.XferCpltCallback  = NULL;

    /*##-3- Foreground Configuration ###########################################*/
    hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    hdma2d.LayerCfg[1].InputAlpha = 0xFF;
    //hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
    hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_L8;
    hdma2d.LayerCfg[1].InputOffset = 0;
    hdma2d.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR; /* No ForeGround Red/Blue swap */
    hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No ForeGround Alpha inversion */

    hdma2d.Instance          = DMA2D;

    /* DMA2D Initialization */
    if(HAL_DMA2D_Init(&hdma2d) == HAL_OK){
        if(HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK){
            if(HAL_DMA2D_CLUTLoad(&hdma2d, clut_cfg, 1) == HAL_OK){
                HAL_DMA2D_PollForTransfer(&hdma2d, 10);
                if (HAL_DMA2D_Start(&hdma2d, source, destination, xsize, ysize) == HAL_OK){
                    /* Polling For DMA transfer */
                    HAL_DMA2D_PollForTransfer(&hdma2d, 10);
                }
                ++y;
                destination = (uint32_t)pDst + (y * LCD_X_Size + x) * 4;
                if (HAL_DMA2D_Start(&hdma2d, source, destination, xsize, ysize) == HAL_OK){
                    /* Polling For DMA transfer */
                    HAL_DMA2D_PollForTransfer(&hdma2d, 10);
                }

                ++y;
                destination = (uint32_t)pDst + (y * LCD_X_Size + x) * 4;
                if (HAL_DMA2D_Start(&hdma2d, source, destination, xsize, ysize) == HAL_OK){
                    /* Polling For DMA transfer */
                    HAL_DMA2D_PollForTransfer(&hdma2d, 10);
                }
            }
        }
    }
}

/**
 * @brief Copies the work buffer to the frame-buffer while converting from L8 color to RGB8888 through use of DMA2D
 * @param gb_frame Work buffer
 * @param scaleAmount Up-scaling amount
 * @returns Nothing
 */
void displayFrameBuffer(uint8_t* gb_frame, uint8_t scaleAmount, uint8_t ly){
	//CopyBuffer((uint32_t *) gb_frame, (uint32_t *)Buffers[0], 0, (480 - (144*3))/2, 160 * scaleAmount, 144 * scaleAmount);
	CopyBuffer((uint32_t *) gb_frame, (uint32_t *)Buffers[0], 0, (480 - (144*3))/2 + (ly * scaleAmount), 160 * scaleAmount, 1);

	//LTDC_LAYER(&hlcd_ltdc, 0)->CFBAR = ((uint32_t)Buffers[0]);
}

