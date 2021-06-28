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
	return (uint16_t) (y << 8) + x;
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
	(((tempRes & 0x07FF) < (*regHL & 0x07FF))) ? setbit(flagReg, H_FLAG): resetbit(flagReg, H_FLAG);
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

void vGBFunctionDAA(uint8_t *regA, uint8_t *flagReg){
	uint16_t tempShort = *regA;
	if(checkbit(*flagReg, N_FLAG) != 0){
		if(checkbit(*flagReg, H_FLAG) != 0) tempShort += 0xFA;
		if(checkbit(*flagReg, C_FLAG) != 0) tempShort += 0xA0;
	}else{
		if(checkbit(*flagReg, H_FLAG) || (tempShort & 0xF) > 9) tempShort += 0x06;
		if(checkbit(*flagReg, C_FLAG) || ((tempShort & 0x1F0) > 0x90)){ tempShort += 0x60; setbit(flagReg, C_FLAG);
		}else{ resetbit(flagReg, C_FLAG);}
	}
	*regA = tempShort;
	resetbit(flagReg, H_FLAG);
	(*regA != 0) ? resetbit(flagReg, Z_FLAG): setbit(flagReg, Z_FLAG);
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
	((*regA & 0xF) + (regValue & 0xF) > 0xF) ? setbit(flagReg, H_FLAG) : resetbit(flagReg, H_FLAG);
	resetbit(flagReg, N_FLAG);
	(tempRes > 0xFF) ? setbit(flagReg, C_FLAG): resetbit(flagReg, C_FLAG);
	*regA = tempRes;
	(*regA != 0) ? resetbit(flagReg, Z_FLAG): setbit(flagReg, Z_FLAG);
}

void vGBFunctionADC(uint8_t *regA, uint8_t *flagReg, uint8_t regValue){
	uint32_t tempRes = *regA + regValue + checkbit(*flagReg, C_FLAG);
	((*regA & 0xF) + ((regValue & 0xF) + checkbit(*flagReg, C_FLAG)) > 0xF) ? setbit(flagReg, H_FLAG) : resetbit(flagReg, H_FLAG);
	resetbit(flagReg, N_FLAG);
	(tempRes > 0xFF) ? setbit(flagReg, C_FLAG) : resetbit(flagReg, C_FLAG);
	*regA = tempRes;
	(*regA != 0) ? resetbit(flagReg, Z_FLAG): setbit(flagReg, Z_FLAG);
}

void vGBFunctionSUB(uint8_t *regA, uint8_t *flagReg, uint8_t regValue){
	(regValue > *regA) ? setbit(flagReg, C_FLAG): resetbit(flagReg, C_FLAG);
	((regValue & 0x0F) > (*regA & 0x0F)) ? setbit(flagReg, H_FLAG): resetbit(flagReg, H_FLAG);
	*regA -= regValue;
	(*regA != 0) ? resetbit(flagReg, Z_FLAG): setbit(flagReg, Z_FLAG);
	setbit(flagReg, N_FLAG);
}

void vGBFunctionSBC(uint8_t *regA, uint8_t *flagReg, uint8_t regValue){
	uint32_t tempRes = regValue + checkbit(*flagReg, C_FLAG);
	((*regA & 0xF) - ((regValue & 0xF) + checkbit(*flagReg, C_FLAG)) < 0x0) ? setbit(flagReg, H_FLAG) : resetbit(flagReg, H_FLAG);
	(*regA - regValue - checkbit(*flagReg, C_FLAG) < 0 ) ? setbit(flagReg, C_FLAG): resetbit(flagReg, C_FLAG);
	*regA -= tempRes;
	(*regA != 0) ? resetbit(flagReg, Z_FLAG): setbit(flagReg, Z_FLAG);
	setbit(flagReg, N_FLAG);
}

void vGBFunctionCP(uint8_t regA, uint8_t *flagReg, uint8_t regValue){
	(regValue > regA) ? setbit(flagReg, C_FLAG): resetbit(flagReg, C_FLAG);
	((regValue & 0x0F) > (regA & 0x0F)) ? setbit(flagReg, H_FLAG): resetbit(flagReg, H_FLAG);
	(regA == regValue) ? setbit(flagReg, Z_FLAG): resetbit(flagReg, Z_FLAG);
	setbit(flagReg, N_FLAG);
}

void vGBFunctionRET(uint16_t *regSP, uint16_t *regPC){
	*regPC = concat_16bit_bigEndian(ucGBMemoryRead(*regSP), ucGBMemoryRead(*regSP + 1));
	*regSP += 2;
}

void vGBFunctionPOP(uint16_t *regSP, uint16_t *reg16){
	*reg16 = concat_16bit_bigEndian(ucGBMemoryRead(*regSP), ucGBMemoryRead(*regSP + 1));
	*regSP += 2;
}

uint8_t vGBFunctionJP_NZ_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t addr){
	if(checkbit(*flagReg, Z_FLAG) != 0){
		return 12;
	}else{
		*regPC = addr;
		return 16;
	}
}

uint8_t vGBFunctionCALL_NZ_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t *regSP){
	if(checkbit(*flagReg, Z_FLAG) != 0){
		return 12;
	}else{
		vGBFunctionPUSH(regSP, regPC);
		*regPC = concat_16bit_bigEndian(ucGBMemoryRead(*regPC - 2), ucGBMemoryRead(*regPC - 1));
		return 24;
	}
}

void vGBFunctionPUSH(uint16_t *regSP, uint16_t *reg16){
	vGBMemoryWrite(*regSP - 1, (uint8_t) ((*reg16 & 0xFF00) >> 8));
	vGBMemoryWrite(*regSP - 2, (uint8_t) (*reg16 & 0x00FF));
	*regSP -= 2;
}

uint8_t vGBFunctionJP_Z_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t addr){
	if(checkbit(*flagReg, Z_FLAG) != 0){
		*regPC = addr;
		return 16;
	}else{
		return 12;
	}
}

uint8_t vGBFunctionCALL_Z_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t *regSP){
	if(checkbit(*flagReg, Z_FLAG) != 0){
		vGBFunctionPUSH(regSP, regPC);
		*regPC = concat_16bit_bigEndian(ucGBMemoryRead(*regPC - 2), ucGBMemoryRead(*regPC - 1));
		return 24;
	}else{
		return 12;
	}
}

uint8_t vGBFunctionJP_NC_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t addr){
	if(checkbit(*flagReg, C_FLAG) != 0){
		return 12;
	}else{
		*regPC = addr;
		return 16;
	}
}

uint8_t vGBFunctionCALL_NC_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t *regSP){
	if(checkbit(*flagReg, C_FLAG) != 0){
		return 12;
	}else{
		vGBFunctionPUSH(regSP, regPC);
		*regPC = concat_16bit_bigEndian(ucGBMemoryRead(*regPC - 2), ucGBMemoryRead(*regPC - 1));
		return 24;
	}
}

uint8_t vGBFunctionJP_C_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t addr){
	if(checkbit(*flagReg, C_FLAG) != 0){
		*regPC = addr;
		return 16;
	}else{
		return 12;
	}
}

uint8_t vGBFunctionCALL_C_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t *regSP){
	if(checkbit(*flagReg, C_FLAG) != 0){
		vGBFunctionPUSH(regSP, regPC);
		*regPC = concat_16bit_bigEndian(ucGBMemoryRead(*regPC - 2), ucGBMemoryRead(*regPC - 1));
		return 24;
	}else{
		return 12;
	}
}

void vGBFunctionADD_SP_r8(uint16_t *regSP, uint8_t *flagReg, uint8_t r8value){
	//uint32_t tempRes = *regSP + (int8_t) r8value;
	uint32_t tempRes = *regSP +r8value;
	((tempRes & 0xFF) < (*regSP & 0xFF)) ? setbit(flagReg, C_FLAG): resetbit(flagReg, C_FLAG);
	((tempRes & 0xF) < (*regSP & 0xF)) ? setbit(flagReg, H_FLAG): resetbit(flagReg, H_FLAG);
	//(tempRes & 0xffff0000) ? setbit(flagReg, C_FLAG): resetbit(flagReg, C_FLAG);
	//(((*regSP & 0x0F) + ((int8_t)r8value & 0x0F)) > 0x0F) ? setbit(flagReg, H_FLAG): resetbit(flagReg, H_FLAG);
	resetbit(flagReg, Z_FLAG);
	resetbit(flagReg, N_FLAG);
	//*regSP = tempRes & 0xFFFF;
	*regSP += (int8_t) r8value;
}

void vGBFunctionLD_HL_SP_r8(uint16_t *regHL, uint16_t *regSP, uint8_t *flagReg, uint8_t r8value){
	//uint32_t tempRes = *regSP + (int8_t) r8value;
	uint32_t tempRes = *regSP +r8value;
	((tempRes & 0xFF) < (*regSP & 0xFF)) ? setbit(flagReg, C_FLAG): resetbit(flagReg, C_FLAG);
	((tempRes & 0xF) < (*regSP & 0xF)) ? setbit(flagReg, H_FLAG): resetbit(flagReg, H_FLAG);
	//(tempRes & 0xffff0000) ? setbit(flagReg, C_FLAG): resetbit(flagReg, C_FLAG);
	//(((*regSP & 0x0F) + ((int8_t)r8value & 0x0F)) > 0x0F) ? setbit(flagReg, H_FLAG): resetbit(flagReg, H_FLAG);
	resetbit(flagReg, Z_FLAG);
	resetbit(flagReg, N_FLAG);
	//*regHL = tempRes & 0xFFFF;
	 *regHL = *regSP + (int8_t) r8value;
}

/*prefix functions*/

uint8_t ucGBFunctionRLC(uint8_t regValue, uint8_t *flagReg){
	uint8_t tempCarry = checkbit(regValue, 7);
	regValue <<= 1;
	regValue += tempCarry;
	(regValue == 0) ? setbit(flagReg, Z_FLAG): resetbit(flagReg, Z_FLAG);
	resetbit(flagReg, N_FLAG);
	resetbit(flagReg, H_FLAG);
	(tempCarry != 0) ? setbit(flagReg, C_FLAG): resetbit(flagReg, C_FLAG);
	return regValue;
}

uint8_t ucGBFunctionRRC(uint8_t regValue, uint8_t *flagReg){
	uint8_t tempCarry = checkbit(regValue, 0);
	regValue >>= 1;
	if(tempCarry) setbit(&regValue, 7);
	(regValue == 0) ? setbit(flagReg, Z_FLAG): resetbit(flagReg, Z_FLAG);
	resetbit(flagReg, N_FLAG);
	resetbit(flagReg, H_FLAG);
	(tempCarry != 0) ? setbit(flagReg, C_FLAG): resetbit(flagReg, C_FLAG);
	return regValue;
}

uint8_t ucGBFunctionRL(uint8_t regValue, uint8_t *flagReg){
	uint8_t prevCarry =  checkbit(*flagReg, C_FLAG);
	((regValue & 0x80) != 0) ? setbit(flagReg, C_FLAG): resetbit(flagReg, C_FLAG);
	regValue <<= 1;
	regValue += prevCarry;
	resetbit(flagReg, N_FLAG);
	resetbit(flagReg, H_FLAG);
	(regValue == 0) ? setbit(flagReg, Z_FLAG): resetbit(flagReg, Z_FLAG);
	return regValue;
}

uint8_t ucGBFunctionRR(uint8_t regValue, uint8_t *flagReg){
	uint8_t prevCarry =  checkbit(*flagReg, C_FLAG);
	((regValue & 0x01) != 0) ? setbit(flagReg, C_FLAG): resetbit(flagReg, C_FLAG);
	regValue >>= 1;
	regValue += (prevCarry << 7);
	resetbit(flagReg, N_FLAG);
	resetbit(flagReg, H_FLAG);
	(regValue == 0) ? setbit(flagReg, Z_FLAG): resetbit(flagReg, Z_FLAG);
	return regValue;
}

uint8_t ucGBFunctionSLA(uint8_t regValue, uint8_t *flagReg){
	((regValue & 0x80) != 0) ? setbit(flagReg, C_FLAG): resetbit(flagReg, C_FLAG);
	regValue <<= 1;
	resetbit(flagReg, N_FLAG);
	resetbit(flagReg, H_FLAG);
	(regValue == 0) ? setbit(flagReg, Z_FLAG): resetbit(flagReg, Z_FLAG);
	return regValue;
}

uint8_t ucGBFunctionSRA(uint8_t regValue, uint8_t *flagReg){
	uint8_t tempMSB = regValue & 0x80;
	((regValue & 0x01) != 0) ? setbit(flagReg, C_FLAG): resetbit(flagReg, C_FLAG);
	regValue >>= 1;
	regValue += tempMSB;
	resetbit(flagReg, N_FLAG);
	resetbit(flagReg, H_FLAG);
	(regValue == 0) ? setbit(flagReg, Z_FLAG): resetbit(flagReg, Z_FLAG);
	return regValue;
}

uint8_t ucGBFunctionSWAP(uint8_t regValue, uint8_t *flagReg){
	regValue = ((regValue & 0x0F) << 4) | ((regValue & 0xF0) >> 4);
	(regValue == 0) ? setbit(flagReg, Z_FLAG): resetbit(flagReg, Z_FLAG);
	resetbit(flagReg, N_FLAG);
	resetbit(flagReg, H_FLAG);
	resetbit(flagReg, C_FLAG);
	return regValue;
}

uint8_t ucGBFunctionSRL(uint8_t regValue, uint8_t *flagReg){
	((regValue & 0x01) != 0) ? setbit(flagReg, C_FLAG): resetbit(flagReg, C_FLAG);
	regValue >>= 1;
	resetbit(flagReg, N_FLAG);
	resetbit(flagReg, H_FLAG);
	(regValue == 0) ? setbit(flagReg, Z_FLAG): resetbit(flagReg, Z_FLAG);
	return regValue;
}

void vGBFunctionBIT(uint8_t regValue, uint8_t bit, uint8_t *flagReg){
	(checkbit(regValue, bit) != 0) ?  resetbit(flagReg, Z_FLAG): setbit(flagReg, Z_FLAG);
	resetbit(flagReg, N_FLAG);
	setbit(flagReg, H_FLAG);
}

uint8_t ucGBFunctionRESHL(uint16_t regHLaddr, uint8_t bit){
	uint8_t tempRes = ucGBMemoryRead(regHLaddr);
	resetbit(&tempRes, bit);
	return tempRes;
}

uint8_t ucGBFunctionSETHL(uint16_t regHLaddr, uint8_t bit){
	uint8_t tempRes = ucGBMemoryRead(regHLaddr);
	setbit(&tempRes, bit);
	return tempRes;
}
