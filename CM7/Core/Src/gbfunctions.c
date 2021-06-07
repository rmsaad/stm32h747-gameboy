/*
 * gbfunctions.c
 *
 *  Created on: Jun 6, 2021
 *      Author: Rami
 */

#include <stdio.h>
#include "gbfunctions.h"

uint16_t concat_16bit_bigEndian(uint8_t x, uint8_t y) {
	uint16_t pow = 16;
    while(x >= pow)
        pow *= 16;
    return y * pow + x;
}

void setbit(uint8_t *n, uint8_t bit){
	(*n) |= (0x1 << bit);
}

void resetbit(uint8_t *n, uint8_t bit){
	(*n) &= ~(0x1 << bit);
}

uint8_t checkbit(uint8_t n, uint8_t bit){
	return ((n >> bit) & 0x1);
}

void v8bitRegisterINC(uint8_t *reg, uint8_t *flagReg){
	((*reg & 0x0f) == 0x0f) ? setbit(flagReg, H_FLAG): resetbit(flagReg, H_FLAG);
	*reg = *reg + 1;
	resetbit(flagReg, N_FLAG);
	(*reg != 0) ? resetbit(flagReg, Z_FLAG): setbit(flagReg, Z_FLAG);
}

void v8bitRegisterDEC(uint8_t *reg, uint8_t *flagReg){
	((*reg & 0x0f) != 0) ? resetbit(flagReg, H_FLAG) : setbit(flagReg, H_FLAG);
	*reg = *reg - 1;
	setbit(flagReg, N_FLAG);
	(*reg != 0) ? resetbit(flagReg, Z_FLAG): setbit(flagReg, Z_FLAG);
}

void vMemoryLocationINC(){

}
