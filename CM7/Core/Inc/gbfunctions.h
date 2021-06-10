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
void vGBFunctionRLCA(uint8_t *reg, uint8_t *flagReg);
void vGBFunctionRRCA(uint8_t *reg, uint8_t *flagReg);
void vGBFunction16bitADD(uint16_t *regHL, uint16_t reg16, uint8_t *flagReg);
void vGBFunctionRLA(uint8_t *regA, uint8_t *flagReg);
void vGBFunctionRRA(uint8_t *regA, uint8_t *flagReg);
uint8_t vGBFunctionJR_NZ(uint16_t *regPC, uint8_t *flagReg, uint8_t r8value);
void vGBFunctionDAA(uint8_t *regA, uint8_t *flagReg);
uint8_t vGBFunctionJR_Z(uint16_t *regPC, uint8_t *flagReg, uint8_t r8value);
uint8_t vGBFunctionJR_NC(uint16_t *regPC, uint8_t *flagReg, uint8_t r8value);
void vMemoryLocationINC(uint16_t loc, uint8_t *flagReg);
void vMemoryLocationDEC(uint16_t loc, uint8_t *flagReg);
uint8_t vGBFunctionJR_C(uint16_t *regPC, uint8_t *flagReg, uint8_t r8value);
void vGBFunctionADD(uint8_t *regA, uint8_t *flagReg, uint8_t regValue);
void vGBFunctionADC(uint8_t *regA, uint8_t *flagReg, uint8_t regValue);
void vGBFunctionSUB(uint8_t *regA, uint8_t *flagReg, uint8_t regValue);
void vGBFunctionSBC(uint8_t *regA, uint8_t *flagReg, uint8_t regValue);
void vGBFunctionCP(uint8_t regA, uint8_t *flagReg, uint8_t regValue);
void vGBFunctionRET(uint16_t *regSP, uint16_t *regPC);
void vGBFunctionPOP(uint16_t *regSP, uint16_t *reg16);
uint8_t vGBFunctionJP_NZ_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t addr);
uint8_t vGBFunctionCALL_NZ_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t *regSP);
void vGBFunctionPUSH(uint16_t *regSP, uint16_t *reg16);
uint8_t vGBFunctionJP_Z_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t addr);
uint8_t vGBFunctionCALL_Z_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t *regSP);
uint8_t vGBFunctionJP_NC_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t addr);
uint8_t vGBFunctionCALL_NC_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t *regSP);
uint8_t vGBFunctionJP_C_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t addr);
uint8_t vGBFunctionCALL_C_a16(uint16_t *regPC, uint8_t *flagReg, uint16_t *regSP);
void vGBFunctionADD_SP_r8(uint16_t *regSP, uint8_t *flagReg, uint8_t r8value);
void vGBFunctionLD_HL_SP_r8(uint16_t *regHL, uint16_t *regSP, uint8_t *flagReg, uint8_t r8value);

// prefix
uint8_t ucGBFunctionRLC(uint8_t regValue, uint8_t *flagReg);
uint8_t ucGBFunctionRRC(uint8_t regValue, uint8_t *flagReg);
uint8_t ucGBFunctionRL(uint8_t regValue, uint8_t *flagReg);
uint8_t ucGBFunctionRR(uint8_t regValue, uint8_t *flagReg);
uint8_t ucGBFunctionSLA(uint8_t regValue, uint8_t *flagReg);
uint8_t ucGBFunctionSRA(uint8_t regValue, uint8_t *flagReg);
uint8_t ucGBFunctionSWAP(uint8_t regValue, uint8_t *flagReg);
uint8_t ucGBFunctionSRL(uint8_t regValue, uint8_t *flagReg);
void vGBFunctionBIT(uint8_t regValue, uint8_t bit, uint8_t *flagReg);
uint8_t ucGBFunctionRESHL(uint16_t regHLaddr, uint8_t bit);
uint8_t ucGBFunctionSETHL(uint16_t regHLaddr, uint8_t bit);

#endif /* INC_GBFUNCTIONS_H_ */
