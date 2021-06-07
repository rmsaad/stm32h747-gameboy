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

void vGBFunctionRLCA(uint8_t *reg, uint8_t *flagReg){
	uint8_t tempCarry = checkbit(*reg, 7);
	*flagReg = (tempCarry != 0) ? 0x10 : 0x00;
	*reg <<= 1;
	*reg += tempCarry;
}

void vGBFunctionRRCA(uint8_t *reg, uint8_t *flagReg){
	uint8_t tempCarry = checkbit(*reg, 0);
	*flagReg = (tempCarry != 0) ? 0x10 : 0x00;
	*reg >>= 1;
	if (tempCarry != 0) setbit(reg, 7);
}

void vGBFunction16bitADD(uint16_t *regHL, uint16_t reg16, uint8_t *flagReg){ // CHECK THIS SHIT FIRST
	uint32_t tempRes = *regHL + reg16;
	(tempRes & 0xffff0000) ? setbit(flagReg, C_FLAG): resetbit(flagReg, C_FLAG); // 15th bit CFLAG
	*regHL = (uint16_t)(tempRes & 0xffff);
	((*regHL & 0x0f) + (reg16 & 0x0f) > 0x0f) ? setbit(flagReg, H_FLAG): resetbit(flagReg, H_FLAG); // 11th bit HFLAG
	resetbit(flagReg, N_FLAG);
}

void vGBFunctionRLA(){

}

void vGBFunctionRRA(){

}

void vMemoryLocationINC(){

}
