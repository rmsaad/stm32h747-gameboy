/*
 * stm32h7_display.h
 *
 *  Created on: Jun 14, 2021
 *      Author: Rami
 */

#ifndef INC_STM32H7_DISPLAY_H_
#define INC_STM32H7_DISPLAY_H_

void dummy_code();
void CopyBuffer(uint32_t *pSrc,
                           uint32_t *pDst,
                           uint16_t x,
                           uint16_t y,
                           uint16_t xsize,
                           uint16_t ysize);


#endif /* INC_STM32H7_DISPLAY_H_ */
