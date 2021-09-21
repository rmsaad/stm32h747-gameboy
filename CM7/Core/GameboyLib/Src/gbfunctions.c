/**
  ******************************************************************************
  * @file           : gbfunctions.c
  * @brief          : Gameboy CPU extended functions
  *                   This file contains implementation for the more complicated CPU instructions
  * @author         : Rami
  * @date           : Jun 6, 2021
  *
  ******************************************************************************
  *
  ******************************************************************************
  */

#include <stdio.h>
#include <stdint.h>

#include "gbfunctions.h"
#include "gbmemory.h"

/**
 * @brief Concatenates two bytes of data in Big Endian format
 * @param x Lower address
 * @param y Higher address
 * @returns Newly concatenated short value
 */
uint16_t usConcat16BitBigEndian(uint8_t x, uint8_t y) {
	return (uint16_t) (y << 8) + x;
}

/**
 * @brief Sets a bit in a register/memory location
 * @param n Register/memory location
 * @param bit The bit to be set (0 - 7)
 * @returns Nothing
 */
void vSetBit(uint8_t *n, uint8_t bit){
	(*n) |= (0x1 << bit);
}

/**
 * @brief Resets a bit in a register/memory location
 * @param n Register/memory location
 * @param bit The bit to be reset (0 - 7)
 * @returns Nothing
 */
void vResetBit(uint8_t *n, uint8_t bit){
	(*n) &= ~(0x1 << bit);
}

/**
 * @brief Checks a bit of a register/memory location
 * @param n Value of register/memory location
 * @param bit The bit to be checked (0 - 7)
 * @returns Value of checked bit
 */
uint8_t ucCheckBit(uint8_t n, uint8_t bit){
	return ((n >> bit) & 0x1);
}

/**
 * @brief Increment register/ memory location
 * @param reg Pointer to register/memory location
 * @param flagReg Pointer to the flag register
 * @returns Nothing
 */
void v8bitRegisterINC(uint8_t *reg, uint8_t *flagReg){
	((*reg & 0x0F) == 0x0F) ? vSetBit(flagReg, H_FLAG): vResetBit(flagReg, H_FLAG);
	*reg = *reg + 1;
	vResetBit(flagReg, N_FLAG);
	(*reg != 0) ? vResetBit(flagReg, Z_FLAG): vSetBit(flagReg, Z_FLAG);
}

/**
 * @brief Decrement register/ memory location
 * @param reg Pointer to register/memory location
 * @param flagReg Pointer to the flag register
 * @returns Nothing
 */
void v8bitRegisterDEC(uint8_t *reg, uint8_t *flagReg){
	((*reg & 0x0F) != 0) ? vResetBit(flagReg, H_FLAG) : vSetBit(flagReg, H_FLAG);
	*reg = *reg - 1;
	vSetBit(flagReg, N_FLAG);
	(*reg != 0) ? vResetBit(flagReg, Z_FLAG): vSetBit(flagReg, Z_FLAG);
}

/**
 * @brief Rotate register A left, bit 7 becomes carry
 * @param reg Pointer to Register A to be rotated
 * @param flagReg Pointer to the flag register
 * @returns Nothing
 */
void vGBFunctionRLCA(uint8_t *reg, uint8_t *flagReg){
	uint8_t tempCarry = ucCheckBit(*reg, 7);
	*flagReg = (tempCarry != 0) ? 0x10 : 0x00;
	*reg <<= 1;
	*reg += tempCarry;
}

/**
 * @brief Rotate register A right, bit 0 becomes carry
 * @param reg Pointer to Register A to be rotated
 * @param flagReg Pointer to the flag register
 * @returns Nothing
 */
void vGBFunctionRRCA(uint8_t *reg, uint8_t *flagReg){
	uint8_t tempCarry = ucCheckBit(*reg, 0);
	*flagReg = (tempCarry != 0) ? 0x10 : 0x00;
	*reg >>= 1;
	if (tempCarry != 0) vSetBit(reg, 7);
}

/**
 * @brief 16 bit addition to the HL register
 * @param regHL Pointer to the HL register
 * @param reg16 value of a 16 bit register
 * @param flagReg Pointer to the flag register
 * @returns Nothing
 */
void vGBFunction16bitADD(uint16_t *regHL, uint16_t reg16, uint8_t *flagReg){
	uint32_t tempRes = *regHL + reg16;
	(tempRes & 0xFFFF0000) ? vSetBit(flagReg, C_FLAG): vResetBit(flagReg, C_FLAG);
	(((tempRes & 0x07FF) < (*regHL & 0x07FF))) ? vSetBit(flagReg, H_FLAG): vResetBit(flagReg, H_FLAG);
	*regHL = (uint16_t)(tempRes & 0xffff);
	vResetBit(flagReg, N_FLAG);
}

/**
 * @brief Rotate register A left through carry
 * @param regA Pointer to Register A to be rotated
 * @param flagReg Pointer to the flag register
 * @returns Nothing
 */
void vGBFunctionRLA(uint8_t *regA, uint8_t *flagReg){
	uint8_t prevCarry =  ucCheckBit(*flagReg, C_FLAG);
	*flagReg = ((*regA & 0x80) != 0) ? 0x10 : 0x00;
	*regA <<= 1;
	*regA += prevCarry;
}

/**
 * @brief Rotate register A right through carry
 * @param regA Pointer to Register A to be rotated
 * @param flagReg Pointer to the flag register
 * @returns Nothing
 */
void vGBFunctionRRA(uint8_t *regA, uint8_t *flagReg){
	uint8_t prevCarry =  ucCheckBit(*flagReg, C_FLAG);
	*flagReg = ((*regA & 0x01) != 0) ? 0x10 : 0x00;
	*regA >>= 1;
	*regA += (prevCarry << 7);
}

/**
 * @brief Jump to Program Counter address plus immediate signed byte value if Z flag is reset
 * @param regPC Pointer to Program Counter register
 * @param flagReg Pointer to the flag register
 * @param r8value Immediate signed byte value
 * @returns Duration in T-states for the current instruction
 */
uint8_t vGBFunctionJR_NZ(uint16_t *regPC, uint8_t *flagReg, uint8_t r8value){
	if(ucCheckBit(*flagReg, Z_FLAG) != 0){
		return 8;
	}else{
		*regPC += (int8_t) r8value;
		return 12;
	}
}

/**
 * @brief Adjusts register A such that the correct representation of Binary Coded Decimal is obtained
 * @param regA Pointer to register A
 * @param flagReg Pointer to the flag register
 * @returns Nothing
 */
void vGBFunctionDAA(uint8_t *regA, uint8_t *flagReg){
	uint16_t tempShort = *regA;
	if(ucCheckBit(*flagReg, N_FLAG) != 0){
		if(ucCheckBit(*flagReg, H_FLAG) != 0) tempShort += 0xFA;
		if(ucCheckBit(*flagReg, C_FLAG) != 0) tempShort += 0xA0;
	}else{
		if(ucCheckBit(*flagReg, H_FLAG) || (tempShort & 0xF) > 9) tempShort += 0x06;
		if(ucCheckBit(*flagReg, C_FLAG) || ((tempShort & 0x1F0) > 0x90)){ tempShort += 0x60; vSetBit(flagReg, C_FLAG);
		}else{ vResetBit(flagReg, C_FLAG);}
	}
	*regA = tempShort;
	vResetBit(flagReg, H_FLAG);
	(*regA != 0) ? vResetBit(flagReg, Z_FLAG): vSetBit(flagReg, Z_FLAG);
}

/**
 * @brief Jump to Program Counter address plus immediate signed byte value if Z flag is set
 * @param regPC Pointer to Program Counter register
 * @param flagReg Pointer to the flag register
 * @param r8value Immediate signed byte value
 * @returns Duration in T-states for the current instruction
 */
uint8_t vGBFunctionJR_Z(uint16_t *regPC, uint8_t *flagReg, uint8_t r8value){
	if(ucCheckBit(*flagReg, Z_FLAG) != 0){
		*regPC += (int8_t) r8value;
		return 12;
	}else{
		return 8;
	}
}

/**
 * @brief Jump to Program Counter address plus immediate signed byte value if C flag is reset
 * @param regPC Pointer to Program Counter register
 * @param flagReg Pointer to the flag register
 * @param r8value Immediate signed byte value
 * @returns Duration in T-states for the current instruction
 */
uint8_t vGBFunctionJR_NC(uint16_t *regPC, uint8_t *flagReg, uint8_t r8value){
	if(ucCheckBit(*flagReg, C_FLAG) != 0){
		return 8;
	}else{
		*regPC += (int8_t) r8value;
		return 12;
	}
}

/**
 * @brief Increment memory location
 * @param loc memory address
 * @param flagReg Pointer to the flag register
 * @returns Nothing
 */
void vMemoryLocationINC(uint16_t loc, uint8_t *flagReg){
	((ucGBMemoryRead(loc) & 0x0F) == 0x0F) ? vSetBit(flagReg, H_FLAG): vResetBit(flagReg, H_FLAG);
	vGBMemoryWrite(loc, ucGBMemoryRead(loc) + 1);
	vResetBit(flagReg, N_FLAG);
	(ucGBMemoryRead(loc) != 0) ? vResetBit(flagReg, Z_FLAG): vSetBit(flagReg, Z_FLAG);
}

/**
 * @brief Decrement memory location
 * @param loc memory address
 * @param flagReg Pointer to the flag register
 * @returns Nothing
 */
void vMemoryLocationDEC(uint16_t loc, uint8_t *flagReg){
	((ucGBMemoryRead(loc) & 0x0F) != 0) ? vResetBit(flagReg, H_FLAG) : vSetBit(flagReg, H_FLAG);
	vGBMemoryWrite(loc, ucGBMemoryRead(loc) - 1);
	vSetBit(flagReg, N_FLAG);
	(ucGBMemoryRead(loc) != 0) ? vResetBit(flagReg, Z_FLAG): vSetBit(flagReg, Z_FLAG);
}

/**
 * @brief Jump to Program Counter address plus immediate signed byte value if C flag is set
 * @param regPC Pointer to Program Counter register
 * @param flagReg Pointer to the flag register
 * @param r8value Immediate signed byte value
 * @returns Duration in T-states for the current instruction
 */
uint8_t vGBFunctionJR_C(uint16_t *regPC, uint8_t *flagReg, uint8_t r8value){
	if(ucCheckBit(*flagReg, C_FLAG) != 0){
		*regPC += (int8_t) r8value;
		return 12;
	}else{
		return 8;
	}
}

/**
 * @brief Adds the value of a register/ memory location to the A register
 * @param regA Pointer to the A register
 * @param flagReg Pointer to the flag register
 * @param regValue Register/memory location data to be added to A register
 * @returns Nothing
 */
void vGBFunctionADD(uint8_t *regA, uint8_t *flagReg, uint8_t regValue){
	uint32_t tempRes = *regA + regValue;
	((*regA & 0xF) + (regValue & 0xF) > 0xF) ? vSetBit(flagReg, H_FLAG) : vResetBit(flagReg, H_FLAG);
	vResetBit(flagReg, N_FLAG);
	(tempRes > 0xFF) ? vSetBit(flagReg, C_FLAG): vResetBit(flagReg, C_FLAG);
	*regA = tempRes;
	(*regA != 0) ? vResetBit(flagReg, Z_FLAG): vSetBit(flagReg, Z_FLAG);
}

/**
 * @brief Adds the value of a register/ memory location + carry to the A register
 * @param regA Pointer to the A register
 * @param flagReg Pointer to the flag register
 * @param regValue Register/memory location data to be added to A register
 * @returns Nothing
 */
void vGBFunctionADC(uint8_t *regA, uint8_t *flagReg, uint8_t regValue){
	uint32_t tempRes = *regA + regValue + ucCheckBit(*flagReg, C_FLAG);
	((*regA & 0xF) + ((regValue & 0xF) + ucCheckBit(*flagReg, C_FLAG)) > 0xF) ? vSetBit(flagReg, H_FLAG) : vResetBit(flagReg, H_FLAG);
	vResetBit(flagReg, N_FLAG);
	(tempRes > 0xFF) ? vSetBit(flagReg, C_FLAG) : vResetBit(flagReg, C_FLAG);
	*regA = tempRes;
	(*regA != 0) ? vResetBit(flagReg, Z_FLAG): vSetBit(flagReg, Z_FLAG);
}

/**
 * @brief Subtracts the value of a register/ memory location from the A register
 * @param regA Pointer to the A register
 * @param flagReg Pointer to the flag register
 * @param regValue Register/memory location data to be subtracted to A register
 * @returns Nothing
 */
void vGBFunctionSUB(uint8_t *regA, uint8_t *flagReg, uint8_t regValue){
	(regValue > *regA) ? vSetBit(flagReg, C_FLAG): vResetBit(flagReg, C_FLAG);
	((regValue & 0x0F) > (*regA & 0x0F)) ? vSetBit(flagReg, H_FLAG): vResetBit(flagReg, H_FLAG);
	*regA -= regValue;
	(*regA != 0) ? vResetBit(flagReg, Z_FLAG): vSetBit(flagReg, Z_FLAG);
	vSetBit(flagReg, N_FLAG);
}

/**
 * @brief Subtracts the value of a register/ memory location + carry from the A register
 * @param regA Pointer to the A register
 * @param flagReg Pointer to the flag register
 * @param regValue Register/memory location data to be subtracted to A register
 * @returns Nothing
 */
void vGBFunctionSBC(uint8_t *regA, uint8_t *flagReg, uint8_t regValue){
	uint32_t tempRes = regValue + ucCheckBit(*flagReg, C_FLAG);
	((*regA & 0xF) - ((regValue & 0xF) + ucCheckBit(*flagReg, C_FLAG)) < 0x0) ? vSetBit(flagReg, H_FLAG) : vResetBit(flagReg, H_FLAG);
	(*regA - regValue - ucCheckBit(*flagReg, C_FLAG) < 0 ) ? vSetBit(flagReg, C_FLAG): vResetBit(flagReg, C_FLAG);
	*regA -= tempRes;
	(*regA != 0) ? vResetBit(flagReg, Z_FLAG): vSetBit(flagReg, Z_FLAG);
	vSetBit(flagReg, N_FLAG);
}

/**
 * @brief Compare register a with another register/memory location
 * @param regA Value of register A
 * @param flagReg Pointer to the flag register
 * @param regValue Value of a register/memory location
 * @returns Nothing
 */
void vGBFunctionCP(uint8_t regA, uint8_t *flagReg, uint8_t regValue){
	(regValue > regA) ? vSetBit(flagReg, C_FLAG): vResetBit(flagReg, C_FLAG);
	((regValue & 0x0F) > (regA & 0x0F)) ? vSetBit(flagReg, H_FLAG): vResetBit(flagReg, H_FLAG);
	(regA == regValue) ? vSetBit(flagReg, Z_FLAG): vResetBit(flagReg, Z_FLAG);
	vSetBit(flagReg, N_FLAG);
}

/**
 * @brief Pop two bytes from the stack and jump to that address
 * @param regSP Pointer to the Stack Pointer register
 * @param regPC Pointer to the Program Counter register
 * @returns Nothing
 */
void vGBFunctionRET(uint16_t *regSP, uint16_t *regPC){
	*regPC = usConcat16BitBigEndian(ucGBMemoryRead(*regSP), ucGBMemoryRead(*regSP + 1));
	*regSP += 2;
}

/**
 * @brief Pop two bytes off of the stack and into 16 bit register, increments the stack pointer by 2
 * @param regSP Pointer to the Stack Pointer register
 * @param reg16 Pointer to 16 bit register
 * @returns Nothing
 */
void vGBFunctionPOP(uint16_t *regSP, uint16_t *reg16){
	*reg16 = usConcat16BitBigEndian(ucGBMemoryRead(*regSP), ucGBMemoryRead(*regSP + 1));
	*regSP += 2;
}

/**
 * @brief Jump to Immediate unsigned short address if Z flag is reset
 * @param regPC Pointer to Program Counter register
 * @param flagReg Pointer to the flag register
 * @param addr Immediate unsigned short address
 * @returns Duration in T-states for the current instruction
 */
uint8_t vGBFunctionJP_NZ_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t addr){
	if(ucCheckBit(*flagReg, Z_FLAG) != 0){
		return 12;
	}else{
		*regPC = addr;
		return 16;
	}
}

/**
 * @brief Pushes 16 bit register onto stack and then jumps to immediate unsigned short address if Z flag is reset
 * @param regPC Pointer to Program Counter register
 * @param flagReg Pointer to the flag register
 * @param regSP Pointer to Stack Pointer register
 * @returns Duration in T-states for the current instruction
 */
uint8_t vGBFunctionCALL_NZ_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t *regSP){
	if(ucCheckBit(*flagReg, Z_FLAG) != 0){
		return 12;
	}else{
		vGBFunctionPUSH(regSP, regPC);
		*regPC = usConcat16BitBigEndian(ucGBMemoryRead(*regPC - 2), ucGBMemoryRead(*regPC - 1));
		return 24;
	}
}

/**
 * @brief Pushes 16 bit register onto stack
 * @param regSP Pointer to Stack Pointer register
 * @param reg16 Pointer to 16 bit register
 * @returns Nothing
 */
void vGBFunctionPUSH(uint16_t *regSP, uint16_t *reg16){
	vGBMemoryWrite(*regSP - 1, (uint8_t) ((*reg16 & 0xFF00) >> 8));
	vGBMemoryWrite(*regSP - 2, (uint8_t) (*reg16 & 0x00FF));
	*regSP -= 2;
}

/**
 * @brief Jump to Immediate unsigned short address if Z flag is set
 * @param regPC Pointer to Program Counter register
 * @param flagReg Pointer to the flag register
 * @param addr Immediate unsigned short address
 * @returns Duration in T-states for the current instruction
 */
uint8_t vGBFunctionJP_Z_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t addr){
	if(ucCheckBit(*flagReg, Z_FLAG) != 0){
		*regPC = addr;
		return 16;
	}else{
		return 12;
	}
}

/**
 * @brief Pushes 16 bit register onto stack and then jumps to immediate unsigned short address if Z flag is set
 * @param regPC Pointer to Program Counter register
 * @param flagReg Pointer to the flag register
 * @param regSP Pointer to Stack Pointer register
 * @returns Duration in T-states for the current instruction
 */
uint8_t vGBFunctionCALL_Z_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t *regSP){
	if(ucCheckBit(*flagReg, Z_FLAG) != 0){
		vGBFunctionPUSH(regSP, regPC);
		*regPC = usConcat16BitBigEndian(ucGBMemoryRead(*regPC - 2), ucGBMemoryRead(*regPC - 1));
		return 24;
	}else{
		return 12;
	}
}

/**
 * @brief Jump to Immediate unsigned short address if C flag is reset
 * @param regPC Pointer to Program Counter register
 * @param flagReg Pointer to the flag register
 * @param addr Immediate unsigned short address
 * @returns Duration in T-states for the current instruction
 */
uint8_t vGBFunctionJP_NC_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t addr){
	if(ucCheckBit(*flagReg, C_FLAG) != 0){
		return 12;
	}else{
		*regPC = addr;
		return 16;
	}
}

/**
 * @brief Pushes 16 bit register onto stack and then jumps to immediate unsigned short address if C flag is reset
 * @param regPC Pointer to Program Counter register
 * @param flagReg Pointer to the flag register
 * @param regSP Pointer to Stack Pointer register
 * @returns Duration in T-states for the current instruction
 */
uint8_t vGBFunctionCALL_NC_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t *regSP){
	if(ucCheckBit(*flagReg, C_FLAG) != 0){
		return 12;
	}else{
		vGBFunctionPUSH(regSP, regPC);
		*regPC = usConcat16BitBigEndian(ucGBMemoryRead(*regPC - 2), ucGBMemoryRead(*regPC - 1));
		return 24;
	}
}

/**
 * @brief Jump to Immediate unsigned short address if C flag is set
 * @param regPC Pointer to Program Counter register
 * @param flagReg Pointer to the flag register
 * @param addr Immediate unsigned short address
 * @returns Duration in T-states for the current instruction
 */
uint8_t vGBFunctionJP_C_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t addr){
	if(ucCheckBit(*flagReg, C_FLAG) != 0){
		*regPC = addr;
		return 16;
	}else{
		return 12;
	}
}

/**
 * @brief Pushes 16 bit register onto stack and then jumps to immediate unsigned short address if C flag is set
 * @param regPC Pointer to Program Counter register
 * @param flagReg Pointer to the flag register
 * @param regSP Pointer to Stack Pointer register
 * @returns Duration in T-states for the current instruction
 */
uint8_t vGBFunctionCALL_C_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t *regSP){
	if(ucCheckBit(*flagReg, C_FLAG) != 0){
		vGBFunctionPUSH(regSP, regPC);
		*regPC = usConcat16BitBigEndian(ucGBMemoryRead(*regPC - 2), ucGBMemoryRead(*regPC - 1));
		return 24;
	}else{
		return 12;
	}
}

/**
 * @brief Add immediate signed byte value to the stack pointer
 * @param regSP Pointer to the Stack Pointer register
 * @param flagReg Pointer to the flag register
 * @param r8value Immediate signed byte value
 * @returns Nothing
 */
void vGBFunctionADD_SP_r8(uint16_t *regSP, uint8_t *flagReg, uint8_t r8value){
	uint32_t tempRes = *regSP +r8value;
	((tempRes & 0xFF) < (*regSP & 0xFF)) ? vSetBit(flagReg, C_FLAG): vResetBit(flagReg, C_FLAG);
	((tempRes & 0xF) < (*regSP & 0xF)) ? vSetBit(flagReg, H_FLAG): vResetBit(flagReg, H_FLAG);
	vResetBit(flagReg, Z_FLAG);
	vResetBit(flagReg, N_FLAG);
	*regSP += (int8_t) r8value;
}

/**
 * @brief Load the Stack Pointer + immediate signed byte value into register HL
 * @param regHL Pointer to the HL register
 * @param regSP Pointer to the SP register
 * @param flagReg Pointer to the flag register
 * @param r8value Immediate signed byte value
 * @returns Nothing
 */
void vGBFunctionLD_HL_SP_r8(uint16_t *regHL, uint16_t *regSP, uint8_t *flagReg, uint8_t r8value){
	uint32_t tempRes = *regSP +r8value;
	((tempRes & 0xFF) < (*regSP & 0xFF)) ? vSetBit(flagReg, C_FLAG): vResetBit(flagReg, C_FLAG);
	((tempRes & 0xF) < (*regSP & 0xF)) ? vSetBit(flagReg, H_FLAG): vResetBit(flagReg, H_FLAG);
	vResetBit(flagReg, Z_FLAG);
	vResetBit(flagReg, N_FLAG);
	 *regHL = *regSP + (int8_t) r8value;
}

/*prefix functions*/

/**
 * @brief Rotate register/memory left, bit 7 becomes carry flag
 * @param regValue Register/memory value to be rotated
 * @param flagReg Pointer to the flag register
 * @returns New value of the register/memory
 */
uint8_t ucGBFunctionRLC(uint8_t regValue, uint8_t *flagReg){
	uint8_t tempCarry = ucCheckBit(regValue, 7);
	regValue <<= 1;
	regValue += tempCarry;
	(regValue == 0) ? vSetBit(flagReg, Z_FLAG): vResetBit(flagReg, Z_FLAG);
	vResetBit(flagReg, N_FLAG);
	vResetBit(flagReg, H_FLAG);
	(tempCarry != 0) ? vSetBit(flagReg, C_FLAG): vResetBit(flagReg, C_FLAG);
	return regValue;
}

/**
 * @brief Rotate register/memory right, bit 0 becomes carry flag
 * @param regValue Register/memory value to be rotated
 * @param flagReg Pointer to the flag register
 * @returns New value of the register/memory
 */
uint8_t ucGBFunctionRRC(uint8_t regValue, uint8_t *flagReg){
	uint8_t tempCarry = ucCheckBit(regValue, 0);
	regValue >>= 1;
	if(tempCarry) vSetBit(&regValue, 7);
	(regValue == 0) ? vSetBit(flagReg, Z_FLAG): vResetBit(flagReg, Z_FLAG);
	vResetBit(flagReg, N_FLAG);
	vResetBit(flagReg, H_FLAG);
	(tempCarry != 0) ? vSetBit(flagReg, C_FLAG): vResetBit(flagReg, C_FLAG);
	return regValue;
}

/**
 * @brief Rotate register/memory left through carry
 * @param regValue Register/memory value to be rotated
 * @param flagReg Pointer to the flag register
 * @returns New value of the register/memory
 */
uint8_t ucGBFunctionRL(uint8_t regValue, uint8_t *flagReg){
	uint8_t prevCarry =  ucCheckBit(*flagReg, C_FLAG);
	((regValue & 0x80) != 0) ? vSetBit(flagReg, C_FLAG): vResetBit(flagReg, C_FLAG);
	regValue <<= 1;
	regValue += prevCarry;
	vResetBit(flagReg, N_FLAG);
	vResetBit(flagReg, H_FLAG);
	(regValue == 0) ? vSetBit(flagReg, Z_FLAG): vResetBit(flagReg, Z_FLAG);
	return regValue;
}

/**
 * @brief Rotate register/memory right through carry
 * @param regValue Register/memory value to be rotated
 * @param flagReg Pointer to the flag register
 * @returns New value of the register/memory
 */
uint8_t ucGBFunctionRR(uint8_t regValue, uint8_t *flagReg){
	uint8_t prevCarry =  ucCheckBit(*flagReg, C_FLAG);
	((regValue & 0x01) != 0) ? vSetBit(flagReg, C_FLAG): vResetBit(flagReg, C_FLAG);
	regValue >>= 1;
	regValue += (prevCarry << 7);
	vResetBit(flagReg, N_FLAG);
	vResetBit(flagReg, H_FLAG);
	(regValue == 0) ? vSetBit(flagReg, Z_FLAG): vResetBit(flagReg, Z_FLAG);
	return regValue;
}

/**
 * @brief Shift register/memory left into the carry, The least significant bit is set to zero
 * @param regValue Register/memory value to be shifted
 * @param flagReg Pointer to the flag register
 * @returns New value of the register/memory
 */
uint8_t ucGBFunctionSLA(uint8_t regValue, uint8_t *flagReg){
	((regValue & 0x80) != 0) ? vSetBit(flagReg, C_FLAG): vResetBit(flagReg, C_FLAG);
	regValue <<= 1;
	vResetBit(flagReg, N_FLAG);
	vResetBit(flagReg, H_FLAG);
	(regValue == 0) ? vSetBit(flagReg, Z_FLAG): vResetBit(flagReg, Z_FLAG);
	return regValue;
}

/**
 * @brief Shift register/memory right into the carry
 * @param regValue Register/memory value to be shifted
 * @param flagReg Pointer to the flag register
 * @returns New value of the register/memory
 */
uint8_t ucGBFunctionSRA(uint8_t regValue, uint8_t *flagReg){
	uint8_t tempMSB = regValue & 0x80;
	((regValue & 0x01) != 0) ? vSetBit(flagReg, C_FLAG): vResetBit(flagReg, C_FLAG);
	regValue >>= 1;
	regValue += tempMSB;
	vResetBit(flagReg, N_FLAG);
	vResetBit(flagReg, H_FLAG);
	(regValue == 0) ? vSetBit(flagReg, Z_FLAG): vResetBit(flagReg, Z_FLAG);
	return regValue;
}

/**
 * @brief Swap upper and lower nibbles of a register or memory location
 * @param regValue value of the register or memory location
 * @param flagReg Pointer to the flag register
 * @returns New value of the register or memory
 */
uint8_t ucGBFunctionSWAP(uint8_t regValue, uint8_t *flagReg){
	regValue = ((regValue & 0x0F) << 4) | ((regValue & 0xF0) >> 4);
	(regValue == 0) ? vSetBit(flagReg, Z_FLAG): vResetBit(flagReg, Z_FLAG);
	vResetBit(flagReg, N_FLAG);
	vResetBit(flagReg, H_FLAG);
	vResetBit(flagReg, C_FLAG);
	return regValue;
}

/**
 * @brief Shift register/memory right into the carry, The most significant bit is set to zero
 * @param regValue Register/memory value to be shifted
 * @param flagReg Pointer to the flag register
 * @returns New value of the register/memory
 */
uint8_t ucGBFunctionSRL(uint8_t regValue, uint8_t *flagReg){
	((regValue & 0x01) != 0) ? vSetBit(flagReg, C_FLAG): vResetBit(flagReg, C_FLAG);
	regValue >>= 1;
	vResetBit(flagReg, N_FLAG);
	vResetBit(flagReg, H_FLAG);
	(regValue == 0) ? vSetBit(flagReg, Z_FLAG): vResetBit(flagReg, Z_FLAG);
	return regValue;
}

/**
 * @brief Test the value of a bit in a register/memory and sets the appropriate flags in the flag register.
 * @param regValue Register/memory value to be tested
 * @param bit The bit to be tested
 * @param flagReg Pointer to the flag register
 * @returns Nothing
 */
void vGBFunctionBIT(uint8_t regValue, uint8_t bit, uint8_t *flagReg){
	(ucCheckBit(regValue, bit) != 0) ?  vResetBit(flagReg, Z_FLAG): vSetBit(flagReg, Z_FLAG);
	vResetBit(flagReg, N_FLAG);
	vSetBit(flagReg, H_FLAG);
}

/**
 * @brief Resets a bit in the memory address that is held in the HL register
 * @param regHLaddr Memory address held inside HL register
 * @param bit The bit to be reset (0 - 7)
 * @returns New value held at memory address denoted by HL
 */
uint8_t ucGBFunctionRESHL(uint16_t regHLaddr, uint8_t bit){
	uint8_t tempRes = ucGBMemoryRead(regHLaddr);
	vResetBit(&tempRes, bit);
	return tempRes;
}

/**
 * @brief Sets a bit in the memory address that is held in the HL register
 * @param regHLaddr Memory address held inside HL register
 * @param bit The bit to be set (0 - 7)
 * @returns New value held at memory address denoted by HL
 */
uint8_t ucGBFunctionSETHL(uint16_t regHLaddr, uint8_t bit){
	uint8_t tempRes = ucGBMemoryRead(regHLaddr);
	vSetBit(&tempRes, bit);
	return tempRes;
}
