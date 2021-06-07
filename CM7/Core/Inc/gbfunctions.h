/*
 * gbfunctions.h
 *
 *  Created on: Jun 6, 2021
 *      Author: Rami
 */

#ifndef INC_GBFUNCTIONS_H_
#define INC_GBFUNCTIONS_H_

#include <stdio.h>

#define Z_FLAG 7
#define N_FLAG 6
#define H_FLAG 5
#define C_FLAG 4

/* Public function prototypes -----------------------------------------------*/

uint16_t concat_16bit_bigEndian(uint8_t x, uint8_t y);
void setbit(uint8_t *n, uint8_t bit);
void resetbit(uint8_t *n, uint8_t bit);
uint8_t checkbit(uint8_t n, uint8_t bit);

// reusable operations

void v8bitRegisterINC(uint8_t *reg, uint8_t *flagReg);
void v8bitRegisterDEC(uint8_t *reg, uint8_t *flagReg);

#endif /* INC_GBFUNCTIONS_H_ */
