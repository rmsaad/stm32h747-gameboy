/*
 * gbfunctions.c
 *
 *  Created on: Jun 6, 2021
 *      Author: Rami
 */

#include <stdio.h>
#include "gbfunctions.h"
#include "gbmemory.h"

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
	((*reg & 0x0F) == 0x0F) ? setbit(flagReg, H_FLAG): resetbit(flagReg, H_FLAG);
	*reg = *reg + 1;
	resetbit(flagReg, N_FLAG);
	(*reg != 0) ? resetbit(flagReg, Z_FLAG): setbit(flagReg, Z_FLAG);
}

void v8bitRegisterDEC(uint8_t *reg, uint8_t *flagReg){
	((*reg & 0x0F) != 0) ? resetbit(flagReg, H_FLAG) : setbit(flagReg, H_FLAG);
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

void vGBFunction16bitADD(uint16_t *regHL, uint16_t reg16, uint8_t *flagReg){
	uint32_t tempRes = *regHL + reg16;
	(tempRes & 0xFFFF0000) ? setbit(flagReg, C_FLAG): resetbit(flagReg, C_FLAG);
	(((tempRes & 0x0FFF) < (*regHL & 0x0FFF))) ? setbit(flagReg, H_FLAG): resetbit(flagReg, H_FLAG);
	*regHL = (uint16_t)(tempRes & 0xffff);
	resetbit(flagReg, N_FLAG);
}

void vGBFunctionRLA(uint8_t *regA, uint8_t *flagReg){
	uint8_t prevCarry =  checkbit(*flagReg, C_FLAG);
	*flagReg = ((*regA & 0x80) != 0) ? 0x10 : 0x00;
	*regA <<= 1;
	*regA += prevCarry;
}

void vGBFunctionRRA(uint8_t *regA, uint8_t *flagReg){
	uint8_t prevCarry =  checkbit(*flagReg, C_FLAG);
	*flagReg = ((*regA & 0x01) != 0) ? 0x10 : 0x00;
	*regA >>= 1;
	*regA += (prevCarry << 7);
}

uint8_t vGBFunctionJR_NZ(uint16_t *regPC, uint8_t *flagReg, uint8_t r8value){
	if(checkbit(*flagReg, Z_FLAG) != 0){
		return 8;
	}else{
		*regPC += (int8_t) r8value;
		return 12;
	}
}

void vGBFunctionDAA(uint8_t *regA, uint8_t *flagReg){ // figure this out later
	uint16_t tempShort = *regA;
	if(checkbit(*flagReg, N_FLAG) != 0){
		if(checkbit(*flagReg, H_FLAG) != 0) tempShort = ( tempShort - 0x06) & 0xFF;
		if(checkbit(*flagReg, C_FLAG) != 0) tempShort -= -0x60;
	}else{
		if(checkbit(*flagReg, H_FLAG) || (tempShort & 0xF) > 9) tempShort += 0x06;
		if(checkbit(*flagReg, C_FLAG) || (tempShort > 0x9F)) tempShort += 0x60;
	}
	*regA = tempShort;
	resetbit(flagReg, H_FLAG);
	(*regA != 0) ? resetbit(flagReg, Z_FLAG): setbit(flagReg, Z_FLAG);
	if(tempShort >= 0x100) setbit(flagReg, C_FLAG);
}

uint8_t vGBFunctionJR_Z(uint16_t *regPC, uint8_t *flagReg, uint8_t r8value){
	if(checkbit(*flagReg, Z_FLAG) != 0){
		*regPC += (int8_t) r8value;
		return 12;
	}else{
		return 8;
	}
}

uint8_t vGBFunctionJR_NC(uint16_t *regPC, uint8_t *flagReg, uint8_t r8value){
	if(checkbit(*flagReg, C_FLAG) != 0){
		return 8;
	}else{
		*regPC += (int8_t) r8value;
		return 12;
	}
}

void vMemoryLocationINC(uint16_t loc, uint8_t *flagReg){
	((ucGBMemoryRead(loc) & 0x0F) == 0x0F) ? setbit(flagReg, H_FLAG): resetbit(flagReg, H_FLAG);
	vGBMemoryWrite(loc, ucGBMemoryRead(loc) + 1);
	resetbit(flagReg, N_FLAG);
	(ucGBMemoryRead(loc) != 0) ? resetbit(flagReg, Z_FLAG): setbit(flagReg, Z_FLAG);
}

void vMemoryLocationDEC(uint16_t loc, uint8_t *flagReg){
	((ucGBMemoryRead(loc) & 0x0F) != 0) ? resetbit(flagReg, H_FLAG) : setbit(flagReg, H_FLAG);
	vGBMemoryWrite(loc, ucGBMemoryRead(loc) - 1);
	setbit(flagReg, N_FLAG);
	(ucGBMemoryRead(loc) != 0) ? resetbit(flagReg, Z_FLAG): setbit(flagReg, Z_FLAG);
}

uint8_t vGBFunctionJR_C(uint16_t *regPC, uint8_t *flagReg, uint8_t r8value){
	if(checkbit(*flagReg, C_FLAG) != 0){
		*regPC += (int8_t) r8value;
		return 12;
	}else{
		return 8;
	}
}

void vGBFunctionADD(uint8_t *regA, uint8_t *flagReg, uint8_t regValue){
	uint32_t tempRes = *regA + regValue;
	if((*regA & 0xF) + (regValue & 0xF) > 0xF)	setbit(flagReg, H_FLAG);
	resetbit(flagReg, N_FLAG);
	if(tempRes > 0xFF)	setbit(flagReg, C_FLAG);
	*regA = tempRes;
	if(*regA == 0) setbit(flagReg, Z_FLAG);
}

void vGBFunctionADC(uint8_t *regA, uint8_t *flagReg, uint8_t regValue){
	uint32_t tempRes = *regA + regValue + checkbit(*flagReg, C_FLAG);
	if((*regA & 0xF) + (regValue & 0xF) > 0xF)	setbit(flagReg, H_FLAG);
	resetbit(flagReg, N_FLAG);
	if(tempRes > 0xFF)	setbit(flagReg, C_FLAG);
	*regA = tempRes;
	if(*regA == 0) setbit(flagReg, Z_FLAG);
}

void vGBFunctionSUB(uint8_t *regA, uint8_t *flagReg, uint8_t regValue){
	(regValue > *regA) ? setbit(flagReg, C_FLAG): resetbit(flagReg, C_FLAG);
	((regValue & 0x0F) > (*regA & 0x0F)) ? setbit(flagReg, H_FLAG): resetbit(flagReg, H_FLAG);
	*regA -= regValue;
	(*regA != 0) ? resetbit(flagReg, Z_FLAG): setbit(flagReg, Z_FLAG);
	setbit(flagReg, N_FLAG);
}

void vGBFunctionSBC(uint8_t *regA, uint8_t *flagReg, uint8_t regValue){
	regValue +=  checkbit(*flagReg, C_FLAG);
	(regValue > *regA) ? setbit(flagReg, C_FLAG): resetbit(flagReg, C_FLAG);
	((regValue & 0x0F) > (*regA & 0x0F)) ? setbit(flagReg, H_FLAG): resetbit(flagReg, H_FLAG);
	*regA -= regValue;
	(*regA != 0) ? resetbit(flagReg, Z_FLAG): setbit(flagReg, Z_FLAG);
	setbit(flagReg, N_FLAG);
}

void vGBFunctionCP(uint8_t regA, uint8_t *flagReg, uint8_t regValue){
	(regValue > regA) ? setbit(flagReg, C_FLAG): resetbit(flagReg, C_FLAG);
	((regValue & 0x0F) > (regA & 0x0F)) ? setbit(flagReg, H_FLAG): resetbit(flagReg, H_FLAG);
	(regA == regValue) ? resetbit(flagReg, Z_FLAG): setbit(flagReg, Z_FLAG);
	setbit(flagReg, N_FLAG);
}










