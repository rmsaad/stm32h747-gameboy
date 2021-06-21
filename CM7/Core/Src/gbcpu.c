/**
  ******************************************************************************
  * @file           : gbcpu.c
  * @brief          : Gameboy CPU Functionality
  * @author         : Rami
  * @date           : Mar 28, 2021
  *
  ******************************************************************************
  *
  ******************************************************************************
  */

#include "gbcpu.h"
#include "gbmemory.h"
#include "gbfunctions.h"
//#include "Tetris.gb.h"

#define SET   1
#define RESET 0

#define VBLANK_INTERRUPT  (1 << 0)
#define LCDSTAT_INTERRUPT (1 << 1)
#define TIMER_INTERRUPT   (1 << 2)
#define SERIAL_INTERRUPT  (1 << 3)
#define JOYPAD_INTERRUPT  (1 << 4)

#define IF_ADDR           0xFF0F
#define IE_ADDR           0xFFFF

#define VBLANK_VECTOR     0x0040
#define LCDSTAT_VECTOR    0x0048
#define TIMER_VECTOR      0x0050
#define SERIAL_VECTOR     0x0058
#define JOYPAD_VECTOR     0x0060

//#include "Tetris.gb.h"
//#include "dmg_boot.bin.h"


/* Private variables ---------------------------------------------------------*/

uint8_t ucSTOPPED = 0;
uint8_t ucHALTED = 0;
uint8_t ucInterruptMasterEnable = 0;
uint8_t ucOneCycleInterruptDelay = 0;
uint8_t customDuration = 0;
uint8_t tStates = 0;
uint8_t print = 0;
uint8_t num = 0;
uint64_t numcount;
extern registers reg;
extern unsigned char Tetris_gb[];
extern unsigned char cpu_instrs_gb[];

/* Game Boy CPU instruction set */

void vNOP();
void vLDs_BC_d16();
void vLD_BC_A();
void vINCs_BC();
void vINC_B();
void vDEC_B();
void vLD_B_d8();
void vRLCA();
void vLDs_a16_SP();
void vADDs_HL_BC();
void vLD_A_BC();
void vDECs_BC();
void vINC_C();
void vDEC_C();
void vLD_C_d8();
void vRRCA();

void vSTOP();
void vLDs_DE_d16();
void vLD_DE_A();
void vINCs_DE();
void vINC_D();
void vDEC_D();
void vLD_D_d8();
void vRLA();
void vJR_r8();
void vADDs_HL_DE();
void vLD_A_DE();
void vDECs_DE();
void vINC_E();
void vDEC_E();
void vLD_E_d8();
void vRRA();

void vJR_NZ_r8();
void vLDs_HL_d16();
void vLD_HLI_A();
void vINCs_HL();
void vINC_H();
void vDEC_H();
void vLD_H_d8();
void vDAA();
void vJR_Z_r8();
void vADDs_HL_HL();
void vLD_A_HLI();
void vDECs_HL();
void vINC_L();
void vDEC_L();
void vLD_L_d8();
void vCPL();

void vJR_NC_r8();
void vLDs_SP_d16();
void vLD_HLD_A();
void vINCs_SP();
void vINC_HL();
void vDEC_HL();
void vLD_HL_d8();
void vSCF();
void vJR_C_r8();
void vADDs_HL_SP();
void vLD_A_HLD();
void vDECs_SP();
void vINC_A();
void vDEC_A();
void vLD_A_d8();
void vCCF();

void vLD_B_B();
void vLD_B_C();
void vLD_B_D();
void vLD_B_E();
void vLD_B_H();
void vLD_B_L();
void vLD_B_HL();
void vLD_B_A();
void vLD_C_B();
void vLD_C_C();
void vLD_C_D();
void vLD_C_E();
void vLD_C_H();
void vLD_C_L();
void vLD_C_HL();
void vLD_C_A();

void vLD_D_B();
void vLD_D_C();
void vLD_D_D();
void vLD_D_E();
void vLD_D_H();
void vLD_D_L();
void vLD_D_HL();
void vLD_D_A();
void vLD_E_B();
void vLD_E_C();
void vLD_E_D();
void vLD_E_E();
void vLD_E_H();
void vLD_E_L();
void vLD_E_HL();
void vLD_E_A();

void vLD_H_B();
void vLD_H_C();
void vLD_H_D();
void vLD_H_E();
void vLD_H_H();
void vLD_H_L();
void vLD_H_HL();
void vLD_H_A();
void vLD_L_B();
void vLD_L_C();
void vLD_L_D();
void vLD_L_E();
void vLD_L_H();
void vLD_L_L();
void vLD_L_HL();
void vLD_L_A();

void vLD_HL_B();
void vLD_HL_C();
void vLD_HL_D();
void vLD_HL_E();
void vLD_HL_H();
void vLD_HL_L();
void vHALT();
void vLD_HL_A();
void vLD_A_B();
void vLD_A_C();
void vLD_A_D();
void vLD_A_E();
void vLD_A_H();
void vLD_A_L();
void vLD_A_HL();
void vLD_A_A();

void vADD_A_B();
void vADD_A_C();
void vADD_A_D();
void vADD_A_E();
void vADD_A_H();
void vADD_A_L();
void vADD_A_HL();
void vADD_A_A();
void vADC_A_B();
void vADC_A_C();
void vADC_A_D();
void vADC_A_E();
void vADC_A_H();
void vADC_A_L();
void vADC_A_HL();
void vADC_A_A();

void vSUB_B();
void vSUB_C();
void vSUB_D();
void vSUB_E();
void vSUB_H();
void vSUB_L();
void vSUB_HL();
void vSUB_A();
void vSBC_A_B();
void vSBC_A_C();
void vSBC_A_D();
void vSBC_A_E();
void vSBC_A_H();
void vSBC_A_L();
void vSBC_A_HL();
void vSBC_A_A();

void vAND_B();
void vAND_C();
void vAND_D();
void vAND_E();
void vAND_H();
void vAND_L();
void vAND_HL();
void vAND_A();
void vXOR_B();
void vXOR_C();
void vXOR_D();
void vXOR_E();
void vXOR_H();
void vXOR_L();
void vXOR_HL();
void vXOR_A();

void vOR_B();
void vOR_C();
void vOR_D();
void vOR_E();
void vOR_H();
void vOR_L();
void vOR_HL();
void vOR_A();
void vCP_B();
void vCP_C();
void vCP_D();
void vCP_E();
void vCP_H();
void vCP_L();
void vCP_HL();
void vCP_A();

void vRET_NZ();
void vPOP_BC();
void vJP_NZ_a16();
void vJP_a16();
void vCALL_NZ_a16();
void vPUSH_BC();
void vADD_A_d8();
void vRST_00H();
void vRET_Z();
void vRET();
void vJP_Z_a16();
void vPREFIX();
void vCALL_Z_a16();
void vCALL_a16();
void vADC_A_d8();
void vRST_08H();

void vRET_NC();
void vPOP_DE();
void vJP_NC_a16();
// -----------
void vCALL_NC_a16();
void vPUSH_DE();
void vSUB_d8();
void vRST_10H();
void vRET_C();
void vRETI();
void vJP_C_a16();
// -----------
void vCALL_C_a16();
// -----------
void vSBC_A_d8();
void vRST_18H();

void vLDH_a8_A();
void vPOP_HL();
void vLD_fC_A();
// -----------
// -----------
void vPUSH_HL();
void vAND_d8();
void vRST20H();
void vADDs_SP_r8();
void vJP_HL();
void vLD_a16_A();
// -----------
// -----------
// -----------
void vXOR_d8();
void vRST_28H();

void vLDH_A_a8();
void vPOP_AF();
void vLD_A_fC();
void vDI();
// -----------
void vPUSH_AF();
void vOR_d8();
void vRST_30H();
void vLDs_HL_SP_r8();
void vLDs_SP_HL();
void vLD_A_a16();
void vEI();
// -----------
// -----------
void vCP_d8();
void vRST_38H();

/* Prefix instruction */

void vRLC_B();
void vRLC_C();
void vRLC_D();
void vRLC_E();
void vRLC_H();
void vRLC_L();
void vRLC_HL();
void vRLC_A();
void vRRC_B();
void vRRC_C();
void vRRC_D();
void vRRC_E();
void vRRC_H();
void vRRC_L();
void vRRC_HL();
void vRRC_A();

void vRL_B();
void vRL_C();
void vRL_D();
void vRL_E();
void vRL_H();
void vRL_L();
void vRL_HL();
void vRL_A();
void vRR_B();
void vRR_C();
void vRR_D();
void vRR_E();
void vRR_H();
void vRR_L();
void vRR_HL();
void vRR_A();

void vSLA_B();
void vSLA_C();
void vSLA_D();
void vSLA_E();
void vSLA_H();
void vSLA_L();
void vSLA_HL();
void vSLA_A();
void vSRA_B();
void vSRA_C();
void vSRA_D();
void vSRA_E();
void vSRA_H();
void vSRA_L();
void vSRA_HL();
void vSRA_A();

void vSWAP_B();
void vSWAP_C();
void vSWAP_D();
void vSWAP_E();
void vSWAP_H();
void vSWAP_L();
void vSWAP_HL();
void vSWAP_A();
void vSRL_B();
void vSRL_C();
void vSRL_D();
void vSRL_E();
void vSRL_H();
void vSRL_L();
void vSRL_HL();
void vSRL_A();

void vBIT_0_B();
void vBIT_0_C();
void vBIT_0_D();
void vBIT_0_E();
void vBIT_0_H();
void vBIT_0_L();
void vBIT_0_HL();
void vBIT_0_A();
void vBIT_1_B();
void vBIT_1_C();
void vBIT_1_D();
void vBIT_1_E();
void vBIT_1_H();
void vBIT_1_L();
void vBIT_1_HL();
void vBIT_1_A();

void vBIT_2_B();
void vBIT_2_C();
void vBIT_2_D();
void vBIT_2_E();
void vBIT_2_H();
void vBIT_2_L();
void vBIT_2_HL();
void vBIT_2_A();
void vBIT_3_B();
void vBIT_3_C();
void vBIT_3_D();
void vBIT_3_E();
void vBIT_3_H();
void vBIT_3_L();
void vBIT_3_HL();
void vBIT_3_A();

void vBIT_4_B();
void vBIT_4_C();
void vBIT_4_D();
void vBIT_4_E();
void vBIT_4_H();
void vBIT_4_L();
void vBIT_4_HL();
void vBIT_4_A();
void vBIT_5_B();
void vBIT_5_C();
void vBIT_5_D();
void vBIT_5_E();
void vBIT_5_H();
void vBIT_5_L();
void vBIT_5_HL();
void vBIT_5_A();

void vBIT_6_B();
void vBIT_6_C();
void vBIT_6_D();
void vBIT_6_E();
void vBIT_6_H();
void vBIT_6_L();
void vBIT_6_HL();
void vBIT_6_A();
void vBIT_7_B();
void vBIT_7_C();
void vBIT_7_D();
void vBIT_7_E();
void vBIT_7_H();
void vBIT_7_L();
void vBIT_7_HL();
void vBIT_7_A();

void vRES_0_B();
void vRES_0_C();
void vRES_0_D();
void vRES_0_E();
void vRES_0_H();
void vRES_0_L();
void vRES_0_HL();
void vRES_0_A();
void vRES_1_B();
void vRES_1_C();
void vRES_1_D();
void vRES_1_E();
void vRES_1_H();
void vRES_1_L();
void vRES_1_HL();
void vRES_1_A();

void vRES_2_B();
void vRES_2_C();
void vRES_2_D();
void vRES_2_E();
void vRES_2_H();
void vRES_2_L();
void vRES_2_HL();
void vRES_2_A();
void vRES_3_B();
void vRES_3_C();
void vRES_3_D();
void vRES_3_E();
void vRES_3_H();
void vRES_3_L();
void vRES_3_HL();
void vRES_3_A();

void vRES_4_B();
void vRES_4_C();
void vRES_4_D();
void vRES_4_E();
void vRES_4_H();
void vRES_4_L();
void vRES_4_HL();
void vRES_4_A();
void vRES_5_B();
void vRES_5_C();
void vRES_5_D();
void vRES_5_E();
void vRES_5_H();
void vRES_5_L();
void vRES_5_HL();
void vRES_5_A();

void vRES_6_B();
void vRES_6_C();
void vRES_6_D();
void vRES_6_E();
void vRES_6_H();
void vRES_6_L();
void vRES_6_HL();
void vRES_6_A();
void vRES_7_B();
void vRES_7_C();
void vRES_7_D();
void vRES_7_E();
void vRES_7_H();
void vRES_7_L();
void vRES_7_HL();
void vRES_7_A();

void vSET_0_B();
void vSET_0_C();
void vSET_0_D();
void vSET_0_E();
void vSET_0_H();
void vSET_0_L();
void vSET_0_HL();
void vSET_0_A();
void vSET_1_B();
void vSET_1_C();
void vSET_1_D();
void vSET_1_E();
void vSET_1_H();
void vSET_1_L();
void vSET_1_HL();
void vSET_1_A();

void vSET_2_B();
void vSET_2_C();
void vSET_2_D();
void vSET_2_E();
void vSET_2_H();
void vSET_2_L();
void vSET_2_HL();
void vSET_2_A();
void vSET_3_B();
void vSET_3_C();
void vSET_3_D();
void vSET_3_E();
void vSET_3_H();
void vSET_3_L();
void vSET_3_HL();
void vSET_3_A();

void vSET_4_B();
void vSET_4_C();
void vSET_4_D();
void vSET_4_E();
void vSET_4_H();
void vSET_4_L();
void vSET_4_HL();
void vSET_4_A();
void vSET_5_B();
void vSET_5_C();
void vSET_5_D();
void vSET_5_E();
void vSET_5_H();
void vSET_5_L();
void vSET_5_HL();
void vSET_5_A();

void vSET_6_B();
void vSET_6_C();
void vSET_6_D();
void vSET_6_E();
void vSET_6_H();
void vSET_6_L();
void vSET_6_HL();
void vSET_6_A();
void vSET_7_B();
void vSET_7_C();
void vSET_7_D();
void vSET_7_E();
void vSET_7_H();
void vSET_7_L();
void vSET_7_HL();
void vSET_7_A();

/* structures */

Assembly_Instr instructions[256] = {
		{vNOP,			1,	4},
		{vLDs_BC_d16, 	3,	12},
		{vLD_BC_A,		1,	8},
		{vINCs_BC,		1,	8},
		{vINC_B,		1,	4},
		{vDEC_B,		1,	4},
		{vLD_B_d8,		2,	8},
		{vRLCA,			1,	4},
		{vLDs_a16_SP,	3,	20},
		{vADDs_HL_BC,	1,	8},
		{vLD_A_BC,		1,	8},
		{vDECs_BC,		1,	8},
		{vINC_C,		1,	4},
		{vDEC_C,		1,	4},
		{vLD_C_d8,		2,	8},
		{vRRCA,			1,	4},
		{vSTOP,			2,	4},
		{vLDs_DE_d16,	3,	12},
		{vLD_DE_A,		1,	8},
		{vINCs_DE,		1,	8},
		{vINC_D,		1,	4},
		{vDEC_D,		1,	4},
		{vLD_D_d8,		2,	8},
		{vRLA,			1,	4},
		{vJR_r8,		2,	12},
		{vADDs_HL_DE,	1,	8},
		{vLD_A_DE,		1,	8},
		{vDECs_DE,		1,	8},
		{vINC_E,		1,	4},
		{vDEC_E,		1,	4},
		{vLD_E_d8,		2,	8},
		{vRRA,			1,	4},
		{vJR_NZ_r8,		2,	255},
		{vLDs_HL_d16,	3,	12},
		{vLD_HLI_A,		1,	8},
		{vINCs_HL,		1,	8},
		{vINC_H,		1,	4},
		{vDEC_H,		1,	4},
		{vLD_H_d8,		2,	8},
		{vDAA,			1,	4},
		{vJR_Z_r8,		2,	255},
		{vADDs_HL_HL,	1,	8},
		{vLD_A_HLI,		1,	8},
		{vDECs_HL,		1,	8},
		{vINC_L,		1,	4},
		{vDEC_L,		1,	4},
		{vLD_L_d8,		2,	8},
		{vCPL,			1,	4},
		{vJR_NC_r8,		2,	255},
		{vLDs_SP_d16,	3,	12},
		{vLD_HLD_A,		1,	8},
		{vINCs_SP,		1,	8},
		{vINC_HL,		1,	12},
		{vDEC_HL,		1,	12},
		{vLD_HL_d8,		2,	12},
		{vSCF,			1,	4},
		{vJR_C_r8,		2,	255},
		{vADDs_HL_SP,	1,	8},
		{vLD_A_HLD,		1,	8},
		{vDECs_SP,		1,	8},
		{vINC_A,		1,	4},
		{vDEC_A,		1,	4},
		{vLD_A_d8,		2,	8},
		{vCCF,			1,	4},
		{vLD_B_B,		1,	4},
		{vLD_B_C,		1,	4},
		{vLD_B_D,		1,	4},
		{vLD_B_E,		1,	4},
		{vLD_B_H,		1,	4},
		{vLD_B_L,		1,	4},
		{vLD_B_HL,		1,	8},
		{vLD_B_A,		1,	4},
		{vLD_C_B,		1,	4},
		{vLD_C_C,		1,	4},
		{vLD_C_D,		1,	4},
		{vLD_C_E,		1,	4},
		{vLD_C_H,		1,	4},
		{vLD_C_L,		1,	4},
		{vLD_C_HL,		1,	8},
		{vLD_C_A,		1,	4},
		{vLD_D_B,		1,	4},
		{vLD_D_C,		1,	4},
		{vLD_D_D,		1,	4},
		{vLD_D_E,		1,	4},
		{vLD_D_H,		1,	4},
		{vLD_D_L,		1,	4},
		{vLD_D_HL,		1,	8},
		{vLD_D_A,		1,	4},
		{vLD_E_B,		1,	4},
		{vLD_E_C,		1,	4},
		{vLD_E_D,		1,	4},
		{vLD_E_E,		1,	4},
		{vLD_E_H,		1,	4},
		{vLD_E_L,		1,	4},
		{vLD_E_HL,		1,	8},
		{vLD_E_A,		1,	4},
		{vLD_H_B,		1,	4},
		{vLD_H_C,		1,	4},
		{vLD_H_D,		1,	4},
		{vLD_H_E,		1,	4},
		{vLD_H_H,		1,	4},
		{vLD_H_L,		1,	4},
		{vLD_H_HL,		1,	8},
		{vLD_H_A,		1,	4},
		{vLD_L_B,		1,	4},
		{vLD_L_C,		1,	4},
		{vLD_L_D,		1,	4},
		{vLD_L_E,		1,	4},
		{vLD_L_H,		1,	4},
		{vLD_L_L,		1,	4},
		{vLD_L_HL,		1,	8},
		{vLD_L_A,		1,	4},
		{vLD_HL_B,		1,	8},
		{vLD_HL_C,		1,	8},
		{vLD_HL_D,		1,	8},
		{vLD_HL_E,		1,	8},
		{vLD_HL_H,		1,	8},
		{vLD_HL_L,		1,	8},
		{vHALT,			1,	4},
		{vLD_HL_A,		1,	8},
		{vLD_A_B,		1,	4},
		{vLD_A_C,		1,	4},
		{vLD_A_D,		1,	4},
		{vLD_A_E,		1,	4},
		{vLD_A_H,		1,	4},
		{vLD_A_L,		1,	4},
		{vLD_A_HL,		1,	8},
		{vLD_A_A,		1,	4},
		{vADD_A_B,		1,	4},
		{vADD_A_C,		1,	4},
		{vADD_A_D,		1,	4},
		{vADD_A_E,		1,	4},
		{vADD_A_H,		1,	4},
		{vADD_A_L,		1,	4},
		{vADD_A_HL,		1,	8},
		{vADD_A_A,		1,	4},
		{vADC_A_B,		1,	4},
		{vADC_A_C,		1,	4},
		{vADC_A_D,		1,	4},
		{vADC_A_E,		1,	4},
		{vADC_A_H,		1,	4},
		{vADC_A_L,		1,	4},
		{vADC_A_HL,		1,	8},
		{vADC_A_A,		1,	4},
		{vSUB_B,		1,	4},
		{vSUB_C,		1,	4},
		{vSUB_D,		1,	4},
		{vSUB_E,		1,	4},
		{vSUB_H,		1,	4},
		{vSUB_L,		1,	4},
		{vSUB_HL,		1,	8},
		{vSUB_A,		1,	4},
		{vSBC_A_B,		1,	4},
		{vSBC_A_C,		1,	4},
		{vSBC_A_D,		1,	4},
		{vSBC_A_E,		1,	4},
		{vSBC_A_H,		1,	4},
		{vSBC_A_L,		1,	4},
		{vSBC_A_HL,		1,	8},
		{vSBC_A_A,		1,	4},
		{vAND_B,		1,	4},
		{vAND_C,		1,	4},
		{vAND_D,		1,	4},
		{vAND_E,		1,	4},
		{vAND_H,		1,	4},
		{vAND_L,		1,	4},
		{vAND_HL,		1,	8},
		{vAND_A,		1,	4},
		{vXOR_B,		1,	4},
		{vXOR_C,		1,	4},
		{vXOR_D,		1,	4},
		{vXOR_E,		1,	4},
		{vXOR_H,		1,	4},
		{vXOR_L,		1,	4},
		{vXOR_HL,		1,	8},
		{vXOR_A,		1,	4},
		{vOR_B,			1,	4},
		{vOR_C,			1,	4},
		{vOR_D,			1,	4},
		{vOR_E,			1,	4},
		{vOR_H,			1,	4},
		{vOR_L,			1,	4},
		{vOR_HL,		1,	8},
		{vOR_A,			1,	4},
		{vCP_B,			1,	4},
		{vCP_C,			1,	4},
		{vCP_D,			1,	4},
		{vCP_E,			1,	4},
		{vCP_H,			1,	4},
		{vCP_L,			1,	4},
		{vCP_HL,		1,	8},
		{vCP_A,			1,	4},
		{vRET_NZ,		1,	255},
		{vPOP_BC,		1,	12},
		{vJP_NZ_a16,	3,	255},
		{vJP_a16,		3,	16},
		{vCALL_NZ_a16,	3,	255},
		{vPUSH_BC,		1,	16},
		{vADD_A_d8,		2,	8},
		{vRST_00H,		1,	16},
		{vRET_Z,		1,	255},
		{vRET,			1,	16},
		{vJP_Z_a16,		3,	255},
		{vPREFIX,		1,	4},
		{vCALL_Z_a16,	3,	255},
		{vCALL_a16,		3,	24},
		{vADC_A_d8,		2,	8},
		{vRST_08H,		1,	16},
		{vRET_NC,		1,	255},
		{vPOP_DE,		1,	12},
		{vJP_NC_a16,	3,	255},
		{NULL,			0,	0},
		{vCALL_NC_a16,	3,	255},
		{vPUSH_DE,		1,	16},
		{vSUB_d8,		2,	8},
		{vRST_10H,		1,	16},
		{vRET_C,		1,	255},
		{vRETI,			1,	16},
		{vJP_C_a16,		3,	255},
		{NULL,			0,	0},
		{vCALL_C_a16,	3,	255},
		{NULL,			0,	0},
		{vSBC_A_d8,		2,	8},
		{vRST_18H,		1,	16},
		{vLDH_a8_A,		2,	12},
		{vPOP_HL,		1,	12},
		{vLD_fC_A,		1,	8},
		{NULL,			0,	0},
		{NULL,			0,	0},
		{vPUSH_HL,		1,	16},
		{vAND_d8,		2,	8},
		{vRST20H,		1,	16},
		{vADDs_SP_r8,	2,	16},
		{vJP_HL,		1,	4},
		{vLD_a16_A,		3,	16},
		{NULL,			0,	0},
		{NULL,			0,	0},
		{NULL,			0,	0},
		{vXOR_d8,		2,	8},
		{vRST_28H,		1,	16},
		{vLDH_A_a8,		2,	12},
		{vPOP_AF,		1,	12},
		{vLD_A_fC,		1,	8},
		{vDI,			1,	4},
		{NULL,			0,	0},
		{vPUSH_AF,		1,	16},
		{vOR_d8,		2,	8},
		{vRST_30H,		1,	16},
		{vLDs_HL_SP_r8,	2,	12},
		{vLDs_SP_HL,	1,	8},
		{vLD_A_a16,		3,	16},
		{vEI,			1,	4},
		{NULL,			0,	0},
		{NULL,			0,	0},
		{vCP_d8,		2,	8},
		{vRST_38H,		1,	16}
};

Assembly_Instr prefix_instructions[256] = {
		{vRLC_B,		2,	8},
		{vRLC_C,		2,	8},
		{vRLC_D,		2,	8},
		{vRLC_E,		2,	8},
		{vRLC_H,		2,	8},
		{vRLC_L,		2,	8},
		{vRLC_HL,		2,	16},
		{vRLC_A,		2,	8},
		{vRRC_B,		2,	8},
		{vRRC_C,		2,	8},
		{vRRC_D,		2,	8},
		{vRRC_E,		2,	8},
		{vRRC_H,		2,	8},
		{vRRC_L,		2,	8},
		{vRRC_HL,		2,	16},
		{vRRC_A,		2,	8},
		{vRL_B,			2,	8},
		{vRL_C,			2,	8},
		{vRL_D,			2,	8},
		{vRL_E,			2,	8},
		{vRL_H,			2,	8},
		{vRL_L,			2,	8},
		{vRL_HL,		2,	16},
		{vRL_A,			2,	8},
		{vRR_B,			2,	8},
		{vRR_C,			2,	8},
		{vRR_D,			2,	8},
		{vRR_E,			2,	8},
		{vRR_H,			2,	8},
		{vRR_L,			2,	8},
		{vRR_HL,		2,	16},
		{vRR_A,			2,	8},
		{vSLA_B,		2,	8},
		{vSLA_C,		2,	8},
		{vSLA_D,		2,	8},
		{vSLA_E,		2,	8},
		{vSLA_H,		2,	8},
		{vSLA_L,		2,	8},
		{vSLA_HL,		2,	16},
		{vSLA_A,		2,	8},
		{vSRA_B,		2,	8},
		{vSRA_C,		2,	8},
		{vSRA_D,		2,	8},
		{vSRA_E,		2,	8},
		{vSRA_H,		2,	8},
		{vSRA_L,		2,	8},
		{vSRA_HL,		2,	16},
		{vSRA_A,		2,	8},
		{vSWAP_B,		2,	8},
		{vSWAP_C,		2,	8},
		{vSWAP_D,		2,	8},
		{vSWAP_E,		2,	8},
		{vSWAP_H,		2,	8},
		{vSWAP_L,		2,	8},
		{vSWAP_HL,		2,	16},
		{vSWAP_A,		2,	8},
		{vSRL_B,		2,	8},
		{vSRL_C,		2,	8},
		{vSRL_D,		2,	8},
		{vSRL_E,		2,	8},
		{vSRL_H,		2,	8},
		{vSRL_L,		2,	8},
		{vSRL_HL,		2,	16},
		{vSRL_A,		2,	8},
		{vBIT_0_B,		2,	8},
		{vBIT_0_C,		2,	8},
		{vBIT_0_D,		2,	8},
		{vBIT_0_E,		2,	8},
		{vBIT_0_H,		2,	8},
		{vBIT_0_L,		2,	8},
		{vBIT_0_HL,		2,	16},
		{vBIT_0_A,		2,	8},
		{vBIT_1_B,		2,	8},
		{vBIT_1_C,		2,	8},
		{vBIT_1_D,		2,	8},
		{vBIT_1_E,		2,	8},
		{vBIT_1_H,		2,	8},
		{vBIT_1_L,		2,	8},
		{vBIT_1_HL,		2,	16},
		{vBIT_1_A,		2,	8},
		{vBIT_2_B,		2,	8},
		{vBIT_2_C,		2,	8},
		{vBIT_2_D,		2,	8},
		{vBIT_2_E,		2,	8},
		{vBIT_2_H,		2,	8},
		{vBIT_2_L,		2,	8},
		{vBIT_2_HL,		2,	16},
		{vBIT_2_A,		2,	8},
		{vBIT_3_B,		2,	8},
		{vBIT_3_C,		2,	8},
		{vBIT_3_D,		2,	8},
		{vBIT_3_E,		2,	8},
		{vBIT_3_H,		2,	8},
		{vBIT_3_L,		2,	8},
		{vBIT_3_HL,		2,	16},
		{vBIT_3_A,		2,	8},
		{vBIT_4_B,		2,	8},
		{vBIT_4_C,		2,	8},
		{vBIT_4_D,		2,	8},
		{vBIT_4_E,		2,	8},
		{vBIT_4_H,		2,	8},
		{vBIT_4_L,		2,	8},
		{vBIT_4_HL,		2,	16},
		{vBIT_4_A,		2,	8},
		{vBIT_5_B,		2,	8},
		{vBIT_5_C,		2,	8},
		{vBIT_5_D,		2,	8},
		{vBIT_5_E,		2,	8},
		{vBIT_5_H,		2,	8},
		{vBIT_5_L,		2,	8},
		{vBIT_5_HL,		2,	16},
		{vBIT_5_A,		2,	8},
		{vBIT_6_B,		2,	8},
		{vBIT_6_C,		2,	8},
		{vBIT_6_D,		2,	8},
		{vBIT_6_E,		2,	8},
		{vBIT_6_H,		2,	8},
		{vBIT_6_L,		2,	8},
		{vBIT_6_HL,		2,	16},
		{vBIT_6_A,		2,	8},
		{vBIT_7_B,		2,	8},
		{vBIT_7_C,		2,	8},
		{vBIT_7_D,		2,	8},
		{vBIT_7_E,		2,	8},
		{vBIT_7_H,		2,	8},
		{vBIT_7_L,		2,	8},
		{vBIT_7_HL,		2,	16},
		{vBIT_7_A,		2,	8},
		{vRES_0_B,		2,	8},
		{vRES_0_C,		2,	8},
		{vRES_0_D,		2,	8},
		{vRES_0_E,		2,	8},
		{vRES_0_H,		2,	8},
		{vRES_0_L,		2,	8},
		{vRES_0_HL,		2,	16},
		{vRES_0_A,		2,	8},
		{vRES_1_B,		2,	8},
		{vRES_1_C,		2,	8},
		{vRES_1_D,		2,	8},
		{vRES_1_E,		2,	8},
		{vRES_1_H,		2,	8},
		{vRES_1_L,		2,	8},
		{vRES_1_HL,		2,	16},
		{vRES_1_A,		2,	8},
		{vRES_2_B,		2,	8},
		{vRES_2_C,		2,	8},
		{vRES_2_D,		2,	8},
		{vRES_2_E,		2,	8},
		{vRES_2_H,		2,	8},
		{vRES_2_L,		2,	8},
		{vRES_2_HL,		2,	16},
		{vRES_2_A,		2,	8},
		{vRES_3_B,		2,	8},
		{vRES_3_C,		2,	8},
		{vRES_3_D,		2,	8},
		{vRES_3_E,		2,	8},
		{vRES_3_H,		2,	8},
		{vRES_3_L,		2,	8},
		{vRES_3_HL,		2,	16},
		{vRES_3_A,		2,	8},
		{vRES_4_B,		2,	8},
		{vRES_4_C,		2,	8},
		{vRES_4_D,		2,	8},
		{vRES_4_E,		2,	8},
		{vRES_4_H,		2,	8},
		{vRES_4_L,		2,	8},
		{vRES_4_HL,		2,	16},
		{vRES_4_A,		2,	8},
		{vRES_5_B,		2,	8},
		{vRES_5_C,		2,	8},
		{vRES_5_D,		2,	8},
		{vRES_5_E,		2,	8},
		{vRES_5_H,		2,	8},
		{vRES_5_L,		2,	8},
		{vRES_5_HL,		2,	16},
		{vRES_5_A,		2,	8},
		{vRES_6_B,		2,	8},
		{vRES_6_C,		2,	8},
		{vRES_6_D,		2,	8},
		{vRES_6_E,		2,	8},
		{vRES_6_H,		2,	8},
		{vRES_6_L,		2,	8},
		{vRES_6_HL,		2,	16},
		{vRES_6_A,		2,	8},
		{vRES_7_B,		2,	8},
		{vRES_7_C,		2,	8},
		{vRES_7_D,		2,	8},
		{vRES_7_E,		2,	8},
		{vRES_7_H,		2,	8},
		{vRES_7_L,		2,	8},
		{vRES_7_HL,		2,	16},
		{vRES_7_A,		2,	8},
		{vSET_0_B,		2,	8},
		{vSET_0_C,		2,	8},
		{vSET_0_D,		2,	8},
		{vSET_0_E,		2,	8},
		{vSET_0_H,		2,	8},
		{vSET_0_L,		2,	8},
		{vSET_0_HL,		2,	16},
		{vSET_0_A,		2,	8},
		{vSET_1_B,		2,	8},
		{vSET_1_C,		2,	8},
		{vSET_1_D,		2,	8},
		{vSET_1_E,		2,	8},
		{vSET_1_H,		2,	8},
		{vSET_1_L,		2,	8},
		{vSET_1_HL,		2,	16},
		{vSET_1_A,		2,	8},
		{vSET_2_B,		2,	8},
		{vSET_2_C,		2,	8},
		{vSET_2_D,		2,	8},
		{vSET_2_E,		2,	8},
		{vSET_2_H,		2,	8},
		{vSET_2_L,		2,	8},
		{vSET_2_HL,		2,	16},
		{vSET_2_A,		2,	8},
		{vSET_3_B,		2,	8},
		{vSET_3_C,		2,	8},
		{vSET_3_D,		2,	8},
		{vSET_3_E,		2,	8},
		{vSET_3_H,		2,	8},
		{vSET_3_L,		2,	8},
		{vSET_3_HL,		2,	16},
		{vSET_3_A,		2,	8},
		{vSET_4_B,		2,	8},
		{vSET_4_C,		2,	8},
		{vSET_4_D,		2,	8},
		{vSET_4_E,		2,	8},
		{vSET_4_H,		2,	8},
		{vSET_4_L,		2,	8},
		{vSET_4_HL,		2,	16},
		{vSET_4_A,		2,	8},
		{vSET_5_B,		2,	8},
		{vSET_5_C,		2,	8},
		{vSET_5_D,		2,	8},
		{vSET_5_E,		2,	8},
		{vSET_5_H,		2,	8},
		{vSET_5_L,		2,	8},
		{vSET_5_HL,		2,	16},
		{vSET_5_A,		2,	8},
		{vSET_6_B,		2,	8},
		{vSET_6_C,		2,	8},
		{vSET_6_D,		2,	8},
		{vSET_6_E,		2,	8},
		{vSET_6_H,		2,	8},
		{vSET_6_L,		2,	8},
		{vSET_6_HL,		2,	16},
		{vSET_6_A,		2,	8},
		{vSET_7_B,		2,	8},
		{vSET_7_C,		2,	8},
		{vSET_7_D,		2,	8},
		{vSET_7_E,		2,	8},
		{vSET_7_H,		2,	8},
		{vSET_7_L,		2,	8},
		{vSET_7_HL,		2,	16},
		{vSET_7_A,		2,	8}
};

/* Implementation*/

/*********************0x0X*/
void vNOP(){          }
void vLDs_BC_d16(){   reg.BC = concat_16bit_bigEndian(ucGBMemoryRead(reg.PC - 2), ucGBMemoryRead(reg.PC - 1));}
void vLD_BC_A(){      vGBMemoryWrite(reg.BC, reg.A);}
void vINCs_BC(){      reg.BC++;}
void vINC_B(){        v8bitRegisterINC(&reg.B, &reg.F);}
void vDEC_B(){        v8bitRegisterDEC(&reg.B, &reg.F);}
void vLD_B_d8(){      reg.B = ucGBMemoryRead(reg.PC - 1);}
void vRLCA(){         vGBFunctionRLCA(&reg.A, &reg.F);}
void vLDs_a16_SP(){   vGBMemoryWrite(concat_16bit_bigEndian(ucGBMemoryRead(reg.PC -2), ucGBMemoryRead(reg.PC - 1)), reg.SP);}
void vADDs_HL_BC(){   vGBFunction16bitADD(&reg.HL, reg.BC, &reg.F);}
void vLD_A_BC(){      reg.A = ucGBMemoryRead(reg.BC);}
void vDECs_BC(){      reg.BC--;}
void vINC_C(){        v8bitRegisterINC(&reg.C, &reg.F);}
void vDEC_C(){        v8bitRegisterDEC(&reg.C, &reg.F);}
void vLD_C_d8(){      reg.C = ucGBMemoryRead(reg.PC - 1);}
void vRRCA(){         vGBFunctionRRCA(&reg.A, &reg.F);}

/*********************0x1X*/
void vSTOP(){         ucSTOPPED = 1;}                                    // MORE NEEDED TO IMPLEMENT LATER
void vLDs_DE_d16(){   reg.DE = concat_16bit_bigEndian(ucGBMemoryRead(reg.PC - 2), ucGBMemoryRead(reg.PC - 1));}
void vLD_DE_A(){      vGBMemoryWrite(reg.DE, reg.A);}
void vINCs_DE(){      reg.DE++;}
void vINC_D(){        v8bitRegisterINC(&reg.D, &reg.F);}
void vDEC_D(){        v8bitRegisterDEC(&reg.D, &reg.F);}
void vLD_D_d8(){      reg.D = ucGBMemoryRead(reg.PC - 1);}
void vRLA(){          vGBFunctionRLA(&reg.A, &reg.F);}
void vJR_r8(){        reg.PC += (int8_t) ucGBMemoryRead(reg.PC - 1);}
void vADDs_HL_DE(){   vGBFunction16bitADD(&reg.HL, reg.DE, &reg.F);}
void vLD_A_DE(){      reg.A = ucGBMemoryRead(reg.DE);}
void vDECs_DE(){      reg.DE--;}
void vINC_E(){        v8bitRegisterINC(&reg.E, &reg.F);}
void vDEC_E(){        v8bitRegisterDEC(&reg.E, &reg.F);}
void vLD_E_d8(){      reg.E = ucGBMemoryRead(reg.PC - 1);}
void vRRA(){          vGBFunctionRRA(&reg.A, &reg.F);}

/*********************0x2X*/
void vJR_NZ_r8(){     customDuration = vGBFunctionJR_NZ(&reg.PC, &reg.F, ucGBMemoryRead(reg.PC - 1));}
void vLDs_HL_d16(){   reg.HL = concat_16bit_bigEndian(ucGBMemoryRead(reg.PC - 2), ucGBMemoryRead(reg.PC - 1));}
void vLD_HLI_A(){     vGBMemoryWrite(reg.HL, reg.A); reg.HL++;}
void vINCs_HL(){      reg.HL++;}
void vINC_H(){        v8bitRegisterINC(&reg.H, &reg.F);}
void vDEC_H(){        v8bitRegisterDEC(&reg.H, &reg.F);}
void vLD_H_d8(){      reg.H = ucGBMemoryRead(reg.PC - 1);}
void vDAA(){          vGBFunctionDAA(&reg.A, &reg.F);}
void vJR_Z_r8(){      customDuration = vGBFunctionJR_Z(&reg.PC, &reg.F, ucGBMemoryRead(reg.PC - 1));}
void vADDs_HL_HL(){   vGBFunction16bitADD(&reg.HL, reg.HL, &reg.F);}
void vLD_A_HLI(){     reg.A = ucGBMemoryRead(reg.HL); reg.HL++;}
void vDECs_HL(){      reg.HL--;}
void vINC_L(){        v8bitRegisterINC(&reg.L, &reg.F);}
void vDEC_L(){        v8bitRegisterDEC(&reg.L, &reg.F);}
void vLD_L_d8(){      reg.L = ucGBMemoryRead(reg.PC - 1);}
void vCPL(){          reg.A = ~(reg.A); setbit(&reg.F, N_FLAG); setbit(&reg.F, H_FLAG);}

/*********************0x3X*/
void vJR_NC_r8(){     customDuration = vGBFunctionJR_NC(&reg.PC, &reg.F, ucGBMemoryRead(reg.PC - 1));}
void vLDs_SP_d16(){   reg.SP = concat_16bit_bigEndian(ucGBMemoryRead(reg.PC - 2), ucGBMemoryRead(reg.PC - 1));}
void vLD_HLD_A(){     vGBMemoryWrite(reg.HL, reg.A); reg.HL--;}
void vINCs_SP(){      reg.SP++;}
void vINC_HL(){       vMemoryLocationINC(reg.HL, &reg.F);}
void vDEC_HL(){       vMemoryLocationDEC(reg.HL, &reg.F);}
void vLD_HL_d8(){     vGBMemoryWrite(reg.HL, ucGBMemoryRead(reg.PC - 1));}
void vSCF(){          resetbit(&reg.F, N_FLAG); resetbit(&reg.F, H_FLAG); setbit(&reg.F, C_FLAG);}
void vJR_C_r8(){      customDuration = vGBFunctionJR_C(&reg.PC, &reg.F, ucGBMemoryRead(reg.PC - 1));}
void vADDs_HL_SP(){   vGBFunction16bitADD(&reg.HL, reg.SP, &reg.F);}
void vLD_A_HLD(){     reg.A = ucGBMemoryRead(reg.HL); reg.HL--;}
void vDECs_SP(){      reg.SP--;}
void vINC_A(){        v8bitRegisterINC(&reg.A, &reg.F);}
void vDEC_A(){        v8bitRegisterDEC(&reg.A, &reg.F);}
void vLD_A_d8(){      reg.A = ucGBMemoryRead(reg.PC - 1);}
void vCCF(){          resetbit(&reg.F, N_FLAG); resetbit(&reg.F, H_FLAG); (checkbit(reg.F, C_FLAG)) ? resetbit(&reg.F, C_FLAG) : setbit(&reg.F, C_FLAG);}

/*********************0x4X*/
void vLD_B_B(){       reg.B = reg.B;}
void vLD_B_C(){       reg.B = reg.C;}
void vLD_B_D(){       reg.B = reg.D;}
void vLD_B_E(){       reg.B = reg.E;}
void vLD_B_H(){       reg.B = reg.H;}
void vLD_B_L(){       reg.B = reg.L;}
void vLD_B_HL(){      reg.B = ucGBMemoryRead(reg.HL);}
void vLD_B_A(){       reg.B = reg.A;}
void vLD_C_B(){       reg.C = reg.B;}
void vLD_C_C(){       reg.C = reg.C;}
void vLD_C_D(){       reg.C = reg.D;}
void vLD_C_E(){       reg.C = reg.E;}
void vLD_C_H(){       reg.C = reg.H;}
void vLD_C_L(){       reg.C = reg.L;}
void vLD_C_HL(){      reg.C = ucGBMemoryRead(reg.HL);}
void vLD_C_A(){       reg.C = reg.A;}

/*********************0x5X*/
void vLD_D_B(){       reg.D = reg.B;}
void vLD_D_C(){       reg.D = reg.C;}
void vLD_D_D(){       reg.D = reg.D;}
void vLD_D_E(){       reg.D = reg.E;}
void vLD_D_H(){       reg.D = reg.H;}
void vLD_D_L(){       reg.D = reg.L;}
void vLD_D_HL(){      reg.D = ucGBMemoryRead(reg.HL);}
void vLD_D_A(){       reg.D = reg.A;}
void vLD_E_B(){       reg.E = reg.B;}
void vLD_E_C(){       reg.E = reg.C;}
void vLD_E_D(){       reg.E = reg.D;}
void vLD_E_E(){       reg.E = reg.E;}
void vLD_E_H(){       reg.E = reg.H;}
void vLD_E_L(){       reg.E = reg.L;}
void vLD_E_HL(){      reg.E = ucGBMemoryRead(reg.HL);}
void vLD_E_A(){       reg.E = reg.A;}

/*********************0x6X*/
void vLD_H_B(){       reg.H = reg.B;}
void vLD_H_C(){       reg.H = reg.C;}
void vLD_H_D(){       reg.H = reg.D;}
void vLD_H_E(){       reg.H = reg.E;}
void vLD_H_H(){       reg.H = reg.H;}
void vLD_H_L(){       reg.H = reg.L;}
void vLD_H_HL(){      reg.H = ucGBMemoryRead(reg.HL);}
void vLD_H_A(){       reg.H = reg.A;}
void vLD_L_B(){       reg.L = reg.B;}
void vLD_L_C(){       reg.L = reg.C;}
void vLD_L_D(){       reg.L = reg.D;}
void vLD_L_E(){       reg.L = reg.E;}
void vLD_L_H(){       reg.L = reg.H;}
void vLD_L_L(){       reg.L = reg.L;}
void vLD_L_HL(){      reg.L = ucGBMemoryRead(reg.HL);}
void vLD_L_A(){       reg.L = reg.A;}

/*********************0x7X*/
void vLD_HL_B(){      vGBMemoryWrite(reg.HL, reg.B);}
void vLD_HL_C(){      vGBMemoryWrite(reg.HL, reg.C);}
void vLD_HL_D(){      vGBMemoryWrite(reg.HL, reg.D);}
void vLD_HL_E(){      vGBMemoryWrite(reg.HL, reg.E);}
void vLD_HL_H(){      vGBMemoryWrite(reg.HL, reg.H);}
void vLD_HL_L(){      vGBMemoryWrite(reg.HL, reg.L);}
void vHALT(){         ucHALTED = 1;}
void vLD_HL_A(){      vGBMemoryWrite(reg.HL, reg.A);}
void vLD_A_B(){       reg.A = reg.B;}
void vLD_A_C(){       reg.A = reg.C;}
void vLD_A_D(){       reg.A = reg.D;}
void vLD_A_E(){       reg.A = reg.E;}
void vLD_A_H(){       reg.A = reg.H;}
void vLD_A_L(){       reg.A = reg.L;}
void vLD_A_HL(){      reg.A = ucGBMemoryRead(reg.HL);}
void vLD_A_A(){       reg.A = reg.A;}

/*********************0x8X*/
void vADD_A_B(){      vGBFunctionADD(&reg.A, &reg.F, reg.B);}
void vADD_A_C(){      vGBFunctionADD(&reg.A, &reg.F, reg.C);}
void vADD_A_D(){      vGBFunctionADD(&reg.A, &reg.F, reg.D);}
void vADD_A_E(){      vGBFunctionADD(&reg.A, &reg.F, reg.E);}
void vADD_A_H(){      vGBFunctionADD(&reg.A, &reg.F, reg.H);}
void vADD_A_L(){      vGBFunctionADD(&reg.A, &reg.F, reg.L);}
void vADD_A_HL(){     vGBFunctionADD(&reg.A, &reg.F, ucGBMemoryRead(reg.HL));}
void vADD_A_A(){      vGBFunctionADD(&reg.A, &reg.F, reg.A);}
void vADC_A_B(){      vGBFunctionADC(&reg.A, &reg.F, reg.B);}
void vADC_A_C(){      vGBFunctionADC(&reg.A, &reg.F, reg.C);}
void vADC_A_D(){      vGBFunctionADC(&reg.A, &reg.F, reg.D);}
void vADC_A_E(){      vGBFunctionADC(&reg.A, &reg.F, reg.E);}
void vADC_A_H(){      vGBFunctionADC(&reg.A, &reg.F, reg.H);}
void vADC_A_L(){      vGBFunctionADC(&reg.A, &reg.F, reg.L);}
void vADC_A_HL(){     vGBFunctionADC(&reg.A, &reg.F, ucGBMemoryRead(reg.HL));}
void vADC_A_A(){      vGBFunctionADC(&reg.A, &reg.F, reg.A);}

/*********************0x9X*/
void vSUB_B(){        vGBFunctionSUB(&reg.A, &reg.F, reg.B);}
void vSUB_C(){        vGBFunctionSUB(&reg.A, &reg.F, reg.C);}
void vSUB_D(){        vGBFunctionSUB(&reg.A, &reg.F, reg.D);}
void vSUB_E(){        vGBFunctionSUB(&reg.A, &reg.F, reg.E);}
void vSUB_H(){        vGBFunctionSUB(&reg.A, &reg.F, reg.H);}
void vSUB_L(){        vGBFunctionSUB(&reg.A, &reg.F, reg.L);}
void vSUB_HL(){       vGBFunctionSUB(&reg.A, &reg.F, ucGBMemoryRead(reg.HL));}
void vSUB_A(){        vGBFunctionSUB(&reg.A, &reg.F, reg.A);}
void vSBC_A_B(){      vGBFunctionSBC(&reg.A, &reg.F, reg.B);}
void vSBC_A_C(){      vGBFunctionSBC(&reg.A, &reg.F, reg.C);}
void vSBC_A_D(){      vGBFunctionSBC(&reg.A, &reg.F, reg.D);}
void vSBC_A_E(){      vGBFunctionSBC(&reg.A, &reg.F, reg.E);}
void vSBC_A_H(){      vGBFunctionSBC(&reg.A, &reg.F, reg.H);}
void vSBC_A_L(){      vGBFunctionSBC(&reg.A, &reg.F, reg.L);}
void vSBC_A_HL(){     vGBFunctionSBC(&reg.A, &reg.F, ucGBMemoryRead(reg.HL));}
void vSBC_A_A(){      vGBFunctionSBC(&reg.A, &reg.F, reg.A);}

/*********************0xAX*/
void vAND_B(){        reg.A &= reg.B; reg.F = (reg.A == 0) ? 0xA0 : 0x20;}
void vAND_C(){        reg.A &= reg.C; reg.F = (reg.A == 0) ? 0xA0 : 0x20;}
void vAND_D(){        reg.A &= reg.D; reg.F = (reg.A == 0) ? 0xA0 : 0x20;}
void vAND_E(){        reg.A &= reg.E; reg.F = (reg.A == 0) ? 0xA0 : 0x20;}
void vAND_H(){        reg.A &= reg.H; reg.F = (reg.A == 0) ? 0xA0 : 0x20;}
void vAND_L(){        reg.A &= reg.L; reg.F = (reg.A == 0) ? 0xA0 : 0x20;}
void vAND_HL(){       reg.A &= ucGBMemoryRead(reg.HL); reg.F = (reg.A == 0) ? 0xA0 : 0x20;}
void vAND_A(){        reg.A &= reg.A; reg.F = (reg.A == 0) ? 0xA0 : 0x20;}
void vXOR_B(){        reg.A ^= reg.B; reg.F = (reg.A == 0) ? 0x80 : 0x00;}
void vXOR_C(){        reg.A ^= reg.C; reg.F = (reg.A == 0) ? 0x80 : 0x00;}
void vXOR_D(){        reg.A ^= reg.D; reg.F = (reg.A == 0) ? 0x80 : 0x00;}
void vXOR_E(){        reg.A ^= reg.E; reg.F = (reg.A == 0) ? 0x80 : 0x00;}
void vXOR_H(){        reg.A ^= reg.H; reg.F = (reg.A == 0) ? 0x80 : 0x00;}
void vXOR_L(){        reg.A ^= reg.L; reg.F = (reg.A == 0) ? 0x80 : 0x00;}
void vXOR_HL(){       reg.A ^= ucGBMemoryRead(reg.HL); reg.F = (reg.A == 0) ? 0x80 : 0x00;}
void vXOR_A(){        reg.A ^= reg.A; reg.F = (reg.A == 0) ? 0x80 : 0x00;}

/*********************0xBX*/
void vOR_B(){         reg.A |= reg.B; reg.F = (reg.A == 0) ? 0x80 : 0x00;}
void vOR_C(){         reg.A |= reg.C; reg.F = (reg.A == 0) ? 0x80 : 0x00;}
void vOR_D(){         reg.A |= reg.D; reg.F = (reg.A == 0) ? 0x80 : 0x00;}
void vOR_E(){         reg.A |= reg.E; reg.F = (reg.A == 0) ? 0x80 : 0x00;}
void vOR_H(){         reg.A |= reg.H; reg.F = (reg.A == 0) ? 0x80 : 0x00;}
void vOR_L(){         reg.A |= reg.L; reg.F = (reg.A == 0) ? 0x80 : 0x00;}
void vOR_HL(){        reg.A |= ucGBMemoryRead(reg.HL); reg.F = (reg.A == 0) ? 0x80 : 0x00;}
void vOR_A(){         reg.A |= reg.A; reg.F = (reg.A == 0) ? 0x80 : 0x00;}
void vCP_B(){         vGBFunctionCP(reg.A, &reg.F, reg.B);}
void vCP_C(){         vGBFunctionCP(reg.A, &reg.F, reg.C);}
void vCP_D(){         vGBFunctionCP(reg.A, &reg.F, reg.D);}
void vCP_E(){         vGBFunctionCP(reg.A, &reg.F, reg.E);}
void vCP_H(){         vGBFunctionCP(reg.A, &reg.F, reg.H);}
void vCP_L(){         vGBFunctionCP(reg.A, &reg.F, reg.L);}
void vCP_HL(){        vGBFunctionCP(reg.A, &reg.F, ucGBMemoryRead(reg.HL));}
void vCP_A(){         vGBFunctionCP(reg.A, &reg.F, reg.A);}

/*********************0xCX*/
void vRET_NZ(){       customDuration = (checkbit(reg.F, Z_FLAG)) ?  8 : 20; if(checkbit(reg.F, Z_FLAG) == 0) vGBFunctionRET(&reg.SP, &reg.PC);}
void vPOP_BC(){       vGBFunctionPOP(&reg.SP, &reg.BC);}
void vJP_NZ_a16(){    customDuration = vGBFunctionJP_NZ_a16(&reg.PC, &reg.F, concat_16bit_bigEndian(ucGBMemoryRead(reg.PC - 2), ucGBMemoryRead(reg.PC - 1)));}
void vJP_a16(){       reg.PC = concat_16bit_bigEndian(ucGBMemoryRead(reg.PC - 2), ucGBMemoryRead(reg.PC - 1));}
void vCALL_NZ_a16(){  customDuration = vGBFunctionCALL_NZ_a16(&reg.PC, &reg.F, &reg.SP);}
void vPUSH_BC(){      vGBFunctionPUSH(&reg.SP, &reg.BC);}
void vADD_A_d8(){     vGBFunctionADD(&reg.A, &reg.F, ucGBMemoryRead(reg.PC - 1));}
void vRST_00H(){      vGBFunctionPUSH(&reg.SP, &reg.PC); reg.PC = 0x0000;}
void vRET_Z(){        customDuration = (checkbit(reg.F, Z_FLAG)) ?  20 : 8; if(checkbit(reg.F, Z_FLAG)) vGBFunctionRET(&reg.SP, &reg.PC);}
void vRET(){          vGBFunctionRET(&reg.SP, &reg.PC);}
void vJP_Z_a16(){     customDuration = vGBFunctionJP_Z_a16(&reg.PC, &reg.F, concat_16bit_bigEndian(ucGBMemoryRead(reg.PC - 2), ucGBMemoryRead(reg.PC - 1)));}
void vPREFIX(){       ((void (*)(void))prefix_instructions[ucGBMemoryRead(reg.PC - 1)].instr)();}
void vCALL_Z_a16(){   customDuration = vGBFunctionCALL_Z_a16(&reg.PC, &reg.F, &reg.SP);}
void vCALL_a16(){     vGBFunctionPUSH(&reg.SP, &reg.PC); reg.PC = concat_16bit_bigEndian(ucGBMemoryRead(reg.PC - 2), ucGBMemoryRead(reg.PC - 1));}
void vADC_A_d8(){     vGBFunctionADD(&reg.A, &reg.F, ucGBMemoryRead(reg.PC - 1));}
void vRST_08H(){      vGBFunctionPUSH(&reg.SP, &reg.PC); reg.PC = 0x0008;}

/*********************0xDX*/
void vRET_NC(){       customDuration =(checkbit(reg.F, C_FLAG)) ?  8 : 20; if(checkbit(reg.F, C_FLAG) == 0) vGBFunctionRET(&reg.SP, &reg.PC);}
void vPOP_DE(){       vGBFunctionPOP(&reg.SP, &reg.DE);}
void vJP_NC_a16(){    customDuration = vGBFunctionJP_NC_a16(&reg.PC, &reg.F, concat_16bit_bigEndian(ucGBMemoryRead(reg.PC - 2), ucGBMemoryRead(reg.PC - 1)));}
// -----------
void vCALL_NC_a16(){  customDuration = vGBFunctionCALL_NC_a16(&reg.PC, &reg.F, &reg.SP);}
void vPUSH_DE(){      vGBFunctionPUSH(&reg.SP, &reg.DE);}
void vSUB_d8(){       vGBFunctionSUB(&reg.A, &reg.F, ucGBMemoryRead(reg.PC - 1));}
void vRST_10H(){      vGBFunctionPUSH(&reg.SP, &reg.PC); reg.PC = 0x0010;}
void vRET_C(){        customDuration =(checkbit(reg.F, C_FLAG)) ?  20 : 8; if(checkbit(reg.F, C_FLAG)) vGBFunctionRET(&reg.SP, &reg.PC);}
void vRETI(){         ucInterruptMasterEnable = 1; vGBFunctionRET(&reg.SP, &reg.PC);}
void vJP_C_a16(){     customDuration = vGBFunctionJP_C_a16(&reg.PC, &reg.F, concat_16bit_bigEndian(ucGBMemoryRead(reg.PC - 2), ucGBMemoryRead(reg.PC - 1)));}
// -----------
void vCALL_C_a16(){   customDuration = vGBFunctionCALL_C_a16(&reg.PC, &reg.F, &reg.SP);}
// -----------
void vSBC_A_d8(){     vGBFunctionSBC(&reg.A, &reg.F, ucGBMemoryRead(reg.PC - 1));}
void vRST_18H(){      vGBFunctionPUSH(&reg.SP, &reg.PC); reg.PC = 0x0018;}

/*********************0xEX*/
void vLDH_a8_A(){     vGBMemoryWrite(0xFF00 + ucGBMemoryRead(reg.PC - 1), reg.A);}
void vPOP_HL(){       vGBFunctionPOP(&reg.SP, &reg.HL);}
void vLD_fC_A(){      vGBMemoryWrite(0xFF00 + reg.C, reg.A);}
// -----------
// -----------
void vPUSH_HL(){      vGBFunctionPUSH(&reg.SP, &reg.HL);}
void vAND_d8(){       reg.A &= ucGBMemoryRead(reg.PC - 1); reg.F = (reg.A == 0) ? 0xA0 : 0x20;}
void vRST20H(){       vGBFunctionPUSH(&reg.SP, &reg.PC); reg.PC = 0x0020;}
void vADDs_SP_r8(){   vGBFunctionADD_SP_r8(&reg.SP, &reg.F, ucGBMemoryRead(reg.PC - 1));}
void vJP_HL(){        reg.PC = reg.HL;}
void vLD_a16_A(){     vGBMemoryWrite(concat_16bit_bigEndian(ucGBMemoryRead(reg.PC - 2), ucGBMemoryRead(reg.PC - 1)), reg.A);}
// -----------
// -----------
// -----------
void vXOR_d8(){       reg.A ^= ucGBMemoryRead(reg.PC - 1); reg.F = (reg.A == 0) ? 0x80 : 0x00;}
void vRST_28H(){      vGBFunctionPUSH(&reg.SP, &reg.PC); reg.PC = 0x0028;}

/*********************0xFX*/
void vLDH_A_a8(){     reg.A = ucGBMemoryRead(0xFF00 + ucGBMemoryRead(reg.PC - 1));}
void vPOP_AF(){       vGBFunctionPOP(&reg.SP, &reg.AF); reg.AF &= 0xFFF0;}
void vLD_A_fC(){      reg.A = ucGBMemoryRead(0xFF00 + reg.C);}
void vDI(){           ucInterruptMasterEnable = 0;}
// -----------
void vPUSH_AF(){      vGBFunctionPUSH(&reg.SP, &reg.AF);}
void vOR_d8(){        reg.A |= ucGBMemoryRead(reg.PC - 1); reg.F = (reg.A == 0) ? 0x80 : 0x00;}
void vRST_30H(){      vGBFunctionPUSH(&reg.SP, &reg.PC); reg.PC = 0x0030;}
void vLDs_HL_SP_r8(){ vGBFunctionLD_HL_SP_r8(&reg.HL, &reg.SP, &reg.F, ucGBMemoryRead(reg.PC - 1));}
void vLDs_SP_HL(){    reg.SP = reg.HL;}
void vLD_A_a16(){     reg.A = ucGBMemoryRead(concat_16bit_bigEndian(ucGBMemoryRead(reg.PC - 2), ucGBMemoryRead(reg.PC - 1)));}
void vEI(){           ucInterruptMasterEnable = 1;}
// -----------
// -----------
void vCP_d8(){        vGBFunctionCP(reg.A, &reg.F, ucGBMemoryRead(reg.PC - 1));}
void vRST_38H(){      vGBFunctionPUSH(&reg.SP, &reg.PC); reg.PC = 0x0038;}

/*Prefix implementation*/

/*********************0x0X*/
void vRLC_B(){        reg.B = ucGBFunctionRLC(reg.B, &reg.F);}
void vRLC_C(){        reg.C = ucGBFunctionRLC(reg.C, &reg.F);}
void vRLC_D(){        reg.D = ucGBFunctionRLC(reg.D, &reg.F);}
void vRLC_E(){        reg.E = ucGBFunctionRLC(reg.E, &reg.F);}
void vRLC_H(){        reg.H = ucGBFunctionRLC(reg.H, &reg.F);}
void vRLC_L(){        reg.L = ucGBFunctionRLC(reg.L, &reg.F);}
void vRLC_HL(){       vGBMemoryWrite(reg.HL, ucGBFunctionRLC(ucGBMemoryRead(reg.HL), &reg.F));}
void vRLC_A(){        reg.A = ucGBFunctionRLC(reg.A, &reg.F);}
void vRRC_B(){        reg.B = ucGBFunctionRRC(reg.B, &reg.F);}
void vRRC_C(){        reg.C = ucGBFunctionRRC(reg.C, &reg.F);}
void vRRC_D(){        reg.D = ucGBFunctionRRC(reg.D, &reg.F);}
void vRRC_E(){        reg.E = ucGBFunctionRRC(reg.E, &reg.F);}
void vRRC_H(){        reg.H = ucGBFunctionRRC(reg.H, &reg.F);}
void vRRC_L(){        reg.L = ucGBFunctionRRC(reg.L, &reg.F);}
void vRRC_HL(){       vGBMemoryWrite(reg.HL, ucGBFunctionRRC(ucGBMemoryRead(reg.HL), &reg.F));}
void vRRC_A(){        reg.A = ucGBFunctionRRC(reg.A, &reg.F);}

/*********************0x1X*/
void vRL_B(){         reg.B = ucGBFunctionRL(reg.B, &reg.F);}
void vRL_C(){         reg.C = ucGBFunctionRL(reg.C, &reg.F);}
void vRL_D(){         reg.D = ucGBFunctionRL(reg.D, &reg.F);}
void vRL_E(){         reg.E = ucGBFunctionRL(reg.E, &reg.F);}
void vRL_H(){         reg.H = ucGBFunctionRL(reg.H, &reg.F);}
void vRL_L(){         reg.L = ucGBFunctionRL(reg.L, &reg.F);}
void vRL_HL(){        vGBMemoryWrite(reg.HL, ucGBFunctionRL(ucGBMemoryRead(reg.HL), &reg.F));}
void vRL_A(){         reg.A = ucGBFunctionRL(reg.A, &reg.F);}
void vRR_B(){         reg.B = ucGBFunctionRR(reg.B, &reg.F);}
void vRR_C(){         reg.C = ucGBFunctionRR(reg.C, &reg.F);}
void vRR_D(){         reg.D = ucGBFunctionRR(reg.D, &reg.F);}
void vRR_E(){         reg.E = ucGBFunctionRR(reg.E, &reg.F);}
void vRR_H(){         reg.H = ucGBFunctionRR(reg.H, &reg.F);}
void vRR_L(){         reg.L = ucGBFunctionRR(reg.L, &reg.F);}
void vRR_HL(){        vGBMemoryWrite(reg.HL, ucGBFunctionRR(ucGBMemoryRead(reg.HL), &reg.F));}
void vRR_A(){         reg.A = ucGBFunctionRR(reg.A, &reg.F);}

/*********************0x2X*/
void vSLA_B(){        reg.B = ucGBFunctionSLA(reg.B, &reg.F);}
void vSLA_C(){        reg.C = ucGBFunctionSLA(reg.C, &reg.F);}
void vSLA_D(){        reg.D = ucGBFunctionSLA(reg.D, &reg.F);}
void vSLA_E(){        reg.E = ucGBFunctionSLA(reg.E, &reg.F);}
void vSLA_H(){        reg.H = ucGBFunctionSLA(reg.H, &reg.F);}
void vSLA_L(){        reg.L = ucGBFunctionSLA(reg.L, &reg.F);}
void vSLA_HL(){       vGBMemoryWrite(reg.HL, ucGBFunctionSLA(ucGBMemoryRead(reg.HL), &reg.F));}
void vSLA_A(){        reg.A = ucGBFunctionSLA(reg.A, &reg.F);}
void vSRA_B(){        reg.B = ucGBFunctionSRA(reg.B, &reg.F);}
void vSRA_C(){        reg.C = ucGBFunctionSRA(reg.C, &reg.F);}
void vSRA_D(){        reg.D = ucGBFunctionSRA(reg.D, &reg.F);}
void vSRA_E(){        reg.E = ucGBFunctionSRA(reg.E, &reg.F);}
void vSRA_H(){        reg.H = ucGBFunctionSRA(reg.H, &reg.F);}
void vSRA_L(){        reg.L = ucGBFunctionSRA(reg.L, &reg.F);}
void vSRA_HL(){       vGBMemoryWrite(reg.HL, ucGBFunctionSRA(ucGBMemoryRead(reg.HL), &reg.F));}
void vSRA_A(){        reg.A = ucGBFunctionSRA(reg.A, &reg.F);}

/*********************0x3X*/
void vSWAP_B(){       reg.B = ucGBFunctionSWAP(reg.B, &reg.F);}
void vSWAP_C(){       reg.C = ucGBFunctionSWAP(reg.C, &reg.F);}
void vSWAP_D(){       reg.D = ucGBFunctionSWAP(reg.D, &reg.F);}
void vSWAP_E(){       reg.E = ucGBFunctionSWAP(reg.E, &reg.F);}
void vSWAP_H(){       reg.H = ucGBFunctionSWAP(reg.H, &reg.F);}
void vSWAP_L(){       reg.L = ucGBFunctionSWAP(reg.L, &reg.F);}
void vSWAP_HL(){      vGBMemoryWrite(reg.HL, ucGBFunctionSWAP(ucGBMemoryRead(reg.HL), &reg.F));}
void vSWAP_A(){       reg.A = ucGBFunctionSWAP(reg.A, &reg.F);}
void vSRL_B(){        reg.B = ucGBFunctionSRL(reg.B, &reg.F);}
void vSRL_C(){        reg.C = ucGBFunctionSRL(reg.C, &reg.F);}
void vSRL_D(){        reg.D = ucGBFunctionSRL(reg.D, &reg.F);}
void vSRL_E(){        reg.E = ucGBFunctionSRL(reg.E, &reg.F);}
void vSRL_H(){        reg.H = ucGBFunctionSRL(reg.H, &reg.F);}
void vSRL_L(){        reg.L = ucGBFunctionSRL(reg.L, &reg.F);}
void vSRL_HL(){       vGBMemoryWrite(reg.HL, ucGBFunctionSRL(ucGBMemoryRead(reg.HL), &reg.F));}
void vSRL_A(){        reg.A = ucGBFunctionSRL(reg.A, &reg.F);}

/*********************0x4X*/
void vBIT_0_B(){      vGBFunctionBIT(reg.B, 0, &reg.F);}
void vBIT_0_C(){      vGBFunctionBIT(reg.C, 0, &reg.F);}
void vBIT_0_D(){      vGBFunctionBIT(reg.D, 0, &reg.F);}
void vBIT_0_E(){      vGBFunctionBIT(reg.E, 0, &reg.F);}
void vBIT_0_H(){      vGBFunctionBIT(reg.H, 0, &reg.F);}
void vBIT_0_L(){      vGBFunctionBIT(reg.L, 0, &reg.F);}
void vBIT_0_HL(){     vGBFunctionBIT(ucGBMemoryRead(reg.HL), 0, &reg.F);}
void vBIT_0_A(){      vGBFunctionBIT(reg.A, 1, &reg.F);}
void vBIT_1_B(){      vGBFunctionBIT(reg.B, 1, &reg.F);}
void vBIT_1_C(){      vGBFunctionBIT(reg.C, 1, &reg.F);}
void vBIT_1_D(){      vGBFunctionBIT(reg.D, 1, &reg.F);}
void vBIT_1_E(){      vGBFunctionBIT(reg.E, 1, &reg.F);}
void vBIT_1_H(){      vGBFunctionBIT(reg.H, 1, &reg.F);}
void vBIT_1_L(){      vGBFunctionBIT(reg.L, 1, &reg.F);}
void vBIT_1_HL(){     vGBFunctionBIT(ucGBMemoryRead(reg.HL), 1, &reg.F);}
void vBIT_1_A(){      vGBFunctionBIT(reg.A, 1, &reg.F);}

/*********************0x5X*/
void vBIT_2_B(){      vGBFunctionBIT(reg.B, 2, &reg.F);}
void vBIT_2_C(){      vGBFunctionBIT(reg.C, 2, &reg.F);}
void vBIT_2_D(){      vGBFunctionBIT(reg.D, 2, &reg.F);}
void vBIT_2_E(){      vGBFunctionBIT(reg.E, 2, &reg.F);}
void vBIT_2_H(){      vGBFunctionBIT(reg.H, 2, &reg.F);}
void vBIT_2_L(){      vGBFunctionBIT(reg.L, 2, &reg.F);}
void vBIT_2_HL(){     vGBFunctionBIT(ucGBMemoryRead(reg.HL), 2, &reg.F);}
void vBIT_2_A(){      vGBFunctionBIT(reg.A, 2, &reg.F);}
void vBIT_3_B(){      vGBFunctionBIT(reg.B, 3, &reg.F);}
void vBIT_3_C(){      vGBFunctionBIT(reg.C, 3, &reg.F);}
void vBIT_3_D(){      vGBFunctionBIT(reg.D, 3, &reg.F);}
void vBIT_3_E(){      vGBFunctionBIT(reg.E, 3, &reg.F);}
void vBIT_3_H(){      vGBFunctionBIT(reg.H, 3, &reg.F);}
void vBIT_3_L(){      vGBFunctionBIT(reg.L, 3, &reg.F);}
void vBIT_3_HL(){     vGBFunctionBIT(ucGBMemoryRead(reg.HL), 3, &reg.F);}
void vBIT_3_A(){      vGBFunctionBIT(reg.A, 3, &reg.F);}

/*********************0x6X*/
void vBIT_4_B(){      vGBFunctionBIT(reg.B, 4, &reg.F);}
void vBIT_4_C(){      vGBFunctionBIT(reg.C, 4, &reg.F);}
void vBIT_4_D(){      vGBFunctionBIT(reg.D, 4, &reg.F);}
void vBIT_4_E(){      vGBFunctionBIT(reg.E, 4, &reg.F);}
void vBIT_4_H(){      vGBFunctionBIT(reg.H, 4, &reg.F);}
void vBIT_4_L(){      vGBFunctionBIT(reg.L, 4, &reg.F);}
void vBIT_4_HL(){     vGBFunctionBIT(ucGBMemoryRead(reg.HL), 4, &reg.F);}
void vBIT_4_A(){      vGBFunctionBIT(reg.A, 4, &reg.F);}
void vBIT_5_B(){      vGBFunctionBIT(reg.B, 5, &reg.F);}
void vBIT_5_C(){      vGBFunctionBIT(reg.C, 5, &reg.F);}
void vBIT_5_D(){      vGBFunctionBIT(reg.D, 5, &reg.F);}
void vBIT_5_E(){      vGBFunctionBIT(reg.E, 5, &reg.F);}
void vBIT_5_H(){      vGBFunctionBIT(reg.H, 5, &reg.F);}
void vBIT_5_L(){      vGBFunctionBIT(reg.L, 5, &reg.F);}
void vBIT_5_HL(){     vGBFunctionBIT(ucGBMemoryRead(reg.HL), 5, &reg.F);}
void vBIT_5_A(){      vGBFunctionBIT(reg.A, 5, &reg.F);}

/*********************0x7X*/
void vBIT_6_B(){      vGBFunctionBIT(reg.B, 6, &reg.F);}
void vBIT_6_C(){      vGBFunctionBIT(reg.C, 6, &reg.F);}
void vBIT_6_D(){      vGBFunctionBIT(reg.D, 6, &reg.F);}
void vBIT_6_E(){      vGBFunctionBIT(reg.E, 6, &reg.F);}
void vBIT_6_H(){      vGBFunctionBIT(reg.H, 6, &reg.F);}
void vBIT_6_L(){      vGBFunctionBIT(reg.L, 6, &reg.F);}
void vBIT_6_HL(){     vGBFunctionBIT(ucGBMemoryRead(reg.HL), 6, &reg.F);}
void vBIT_6_A(){      vGBFunctionBIT(reg.A, 6, &reg.F);}
void vBIT_7_B(){      vGBFunctionBIT(reg.B, 7, &reg.F);}
void vBIT_7_C(){      vGBFunctionBIT(reg.C, 7, &reg.F);}
void vBIT_7_D(){      vGBFunctionBIT(reg.D, 7, &reg.F);}
void vBIT_7_E(){      vGBFunctionBIT(reg.E, 7, &reg.F);}
void vBIT_7_H(){      vGBFunctionBIT(reg.H, 7, &reg.F);}
void vBIT_7_L(){      vGBFunctionBIT(reg.L, 7, &reg.F);}
void vBIT_7_HL(){     vGBFunctionBIT(ucGBMemoryRead(reg.HL), 7, &reg.F);}
void vBIT_7_A(){      vGBFunctionBIT(reg.A, 7, &reg.F);}

/*********************0x8X*/
void vRES_0_B(){      resetbit(&reg.B, 0);}
void vRES_0_C(){      resetbit(&reg.C, 0);}
void vRES_0_D(){      resetbit(&reg.D, 0);}
void vRES_0_E(){      resetbit(&reg.E, 0);}
void vRES_0_H(){      resetbit(&reg.H, 0);}
void vRES_0_L(){      resetbit(&reg.L, 0);}
void vRES_0_HL(){     vGBMemoryWrite(reg.HL, ucGBFunctionRESHL(reg.HL, 0));}
void vRES_0_A(){      resetbit(&reg.A, 0);}
void vRES_1_B(){      resetbit(&reg.B, 1);}
void vRES_1_C(){      resetbit(&reg.C, 1);}
void vRES_1_D(){      resetbit(&reg.D, 1);}
void vRES_1_E(){      resetbit(&reg.E, 1);}
void vRES_1_H(){      resetbit(&reg.H, 1);}
void vRES_1_L(){      resetbit(&reg.L, 1);}
void vRES_1_HL(){     vGBMemoryWrite(reg.HL, ucGBFunctionRESHL(reg.HL, 1));}
void vRES_1_A(){      resetbit(&reg.A, 1);}

/*********************0x9X*/
void vRES_2_B(){      resetbit(&reg.B, 2);}
void vRES_2_C(){      resetbit(&reg.C, 2);}
void vRES_2_D(){      resetbit(&reg.D, 2);}
void vRES_2_E(){      resetbit(&reg.E, 2);}
void vRES_2_H(){      resetbit(&reg.H, 2);}
void vRES_2_L(){      resetbit(&reg.L, 2);}
void vRES_2_HL(){     vGBMemoryWrite(reg.HL, ucGBFunctionRESHL(reg.HL, 2));}
void vRES_2_A(){      resetbit(&reg.A, 2);}
void vRES_3_B(){      resetbit(&reg.B, 3);}
void vRES_3_C(){      resetbit(&reg.C, 3);}
void vRES_3_D(){      resetbit(&reg.D, 3);}
void vRES_3_E(){      resetbit(&reg.E, 3);}
void vRES_3_H(){      resetbit(&reg.H, 3);}
void vRES_3_L(){      resetbit(&reg.L, 3);}
void vRES_3_HL(){     vGBMemoryWrite(reg.HL, ucGBFunctionRESHL(reg.HL, 3));}
void vRES_3_A(){      resetbit(&reg.A, 3);}

/*********************0xAX*/
void vRES_4_B(){      resetbit(&reg.B, 4);}
void vRES_4_C(){      resetbit(&reg.C, 4);}
void vRES_4_D(){      resetbit(&reg.D, 4);}
void vRES_4_E(){      resetbit(&reg.E, 4);}
void vRES_4_H(){      resetbit(&reg.H, 4);}
void vRES_4_L(){      resetbit(&reg.L, 4);}
void vRES_4_HL(){     vGBMemoryWrite(reg.HL, ucGBFunctionRESHL(reg.HL, 4));}
void vRES_4_A(){      resetbit(&reg.A, 4);}
void vRES_5_B(){      resetbit(&reg.B, 5);}
void vRES_5_C(){      resetbit(&reg.C, 5);}
void vRES_5_D(){      resetbit(&reg.D, 5);}
void vRES_5_E(){      resetbit(&reg.E, 5);}
void vRES_5_H(){      resetbit(&reg.H, 5);}
void vRES_5_L(){      resetbit(&reg.L, 5);}
void vRES_5_HL(){     vGBMemoryWrite(reg.HL, ucGBFunctionRESHL(reg.HL, 5));}
void vRES_5_A(){      resetbit(&reg.A, 5);}

/*********************0xBX*/
void vRES_6_B(){      resetbit(&reg.B, 6);}
void vRES_6_C(){      resetbit(&reg.C, 6);}
void vRES_6_D(){      resetbit(&reg.D, 6);}
void vRES_6_E(){      resetbit(&reg.E, 6);}
void vRES_6_H(){      resetbit(&reg.H, 6);}
void vRES_6_L(){      resetbit(&reg.L, 6);}
void vRES_6_HL(){     vGBMemoryWrite(reg.HL, ucGBFunctionRESHL(reg.HL, 6));}
void vRES_6_A(){      resetbit(&reg.A, 6);}
void vRES_7_B(){      resetbit(&reg.B, 7);}
void vRES_7_C(){      resetbit(&reg.C, 7);}
void vRES_7_D(){      resetbit(&reg.D, 7);}
void vRES_7_E(){      resetbit(&reg.E, 7);}
void vRES_7_H(){      resetbit(&reg.H, 7);}
void vRES_7_L(){      resetbit(&reg.L, 7);}
void vRES_7_HL(){     vGBMemoryWrite(reg.HL, ucGBFunctionRESHL(reg.HL, 7));}
void vRES_7_A(){      resetbit(&reg.A, 7);}

/*********************0xCX*/
void vSET_0_B(){      setbit(&reg.B, 0);}
void vSET_0_C(){      setbit(&reg.C, 0);}
void vSET_0_D(){      setbit(&reg.D, 0);}
void vSET_0_E(){      setbit(&reg.E, 0);}
void vSET_0_H(){      setbit(&reg.H, 0);}
void vSET_0_L(){      setbit(&reg.L, 0);}
void vSET_0_HL(){     vGBMemoryWrite(reg.HL, ucGBFunctionSETHL(reg.HL, 0));}
void vSET_0_A(){      setbit(&reg.A, 0);}
void vSET_1_B(){      setbit(&reg.B, 1);}
void vSET_1_C(){      setbit(&reg.C, 1);}
void vSET_1_D(){      setbit(&reg.D, 1);}
void vSET_1_E(){      setbit(&reg.E, 1);}
void vSET_1_H(){      setbit(&reg.H, 1);}
void vSET_1_L(){      setbit(&reg.L, 1);}
void vSET_1_HL(){     vGBMemoryWrite(reg.HL, ucGBFunctionSETHL(reg.HL, 1));}
void vSET_1_A(){      setbit(&reg.A, 1);}

/*********************0xDX*/
void vSET_2_B(){      setbit(&reg.B, 2);}
void vSET_2_C(){      setbit(&reg.C, 2);}
void vSET_2_D(){      setbit(&reg.D, 2);}
void vSET_2_E(){      setbit(&reg.E, 2);}
void vSET_2_H(){      setbit(&reg.H, 2);}
void vSET_2_L(){      setbit(&reg.L, 2);}
void vSET_2_HL(){     vGBMemoryWrite(reg.HL, ucGBFunctionSETHL(reg.HL, 2));}
void vSET_2_A(){      setbit(&reg.A, 2);}
void vSET_3_B(){      setbit(&reg.B, 3);}
void vSET_3_C(){      setbit(&reg.C, 3);}
void vSET_3_D(){      setbit(&reg.D, 3);}
void vSET_3_E(){      setbit(&reg.E, 3);}
void vSET_3_H(){      setbit(&reg.H, 3);}
void vSET_3_L(){      setbit(&reg.L, 3);}
void vSET_3_HL(){     vGBMemoryWrite(reg.HL, ucGBFunctionSETHL(reg.HL, 3));}
void vSET_3_A(){      setbit(&reg.A, 3);}

/*********************0xEX*/
void vSET_4_B(){      setbit(&reg.B, 4);}
void vSET_4_C(){      setbit(&reg.C, 4);}
void vSET_4_D(){      setbit(&reg.D, 4);}
void vSET_4_E(){      setbit(&reg.E, 4);}
void vSET_4_H(){      setbit(&reg.H, 4);}
void vSET_4_L(){      setbit(&reg.L, 4);}
void vSET_4_HL(){     vGBMemoryWrite(reg.HL, ucGBFunctionSETHL(reg.HL, 4));}
void vSET_4_A(){      setbit(&reg.A, 4);}
void vSET_5_B(){      setbit(&reg.B, 5);}
void vSET_5_C(){      setbit(&reg.C, 5);}
void vSET_5_D(){      setbit(&reg.D, 5);}
void vSET_5_E(){      setbit(&reg.E, 5);}
void vSET_5_H(){      setbit(&reg.H, 5);}
void vSET_5_L(){      setbit(&reg.L, 5);}
void vSET_5_HL(){     vGBMemoryWrite(reg.HL, ucGBFunctionSETHL(reg.HL, 5));}
void vSET_5_A(){      setbit(&reg.A, 5);}

/*********************0xFX*/
void vSET_6_B(){      setbit(&reg.B, 6);}
void vSET_6_C(){      setbit(&reg.C, 6);}
void vSET_6_D(){      setbit(&reg.D, 6);}
void vSET_6_E(){      setbit(&reg.E, 6);}
void vSET_6_H(){      setbit(&reg.H, 6);}
void vSET_6_L(){      setbit(&reg.L, 6);}
void vSET_6_HL(){     vGBMemoryWrite(reg.HL, ucGBFunctionSETHL(reg.HL, 6));}
void vSET_6_A(){      setbit(&reg.A, 6);}
void vSET_7_B(){      setbit(&reg.B, 7);}
void vSET_7_C(){      setbit(&reg.C, 7);}
void vSET_7_D(){      setbit(&reg.D, 7);}
void vSET_7_E(){      setbit(&reg.E, 7);}
void vSET_7_H(){      setbit(&reg.H, 7);}
void vSET_7_L(){      setbit(&reg.L, 7);}
void vSET_7_HL(){     vGBMemoryWrite(reg.HL, ucGBFunctionSETHL(reg.HL, 7));}
void vSET_7_A(){      setbit(&reg.A, 7);}

uint8_t ucGetTstate(){
	return tStates;
}

void vGBCPUboot(){
	if(reg.PC <= 0xFF){
		vGBCPUinstr(ucGBMemoryRead(reg.PC));
	}else{
		numcount++;
		static int n = 0;
		if(n == 0){
			vGBMemoryLoad(Tetris_gb, 256);
			//vGBMemoryLoad(cpu_instrs_gb, 256);
			n = 1;
		}
		vGBCPUinstr(ucGBMemoryRead(reg.PC));
		//vGBMemoryPrint();
	}
}

void vGBCPUInterruptHandler(){
	if(ucOneCycleInterruptDelay == 1){
		if(ucGBMemoryRead(IE_ADDR) & ucGBMemoryRead(IF_ADDR) & 0x1F){
			ucInterruptMasterEnable = 0;
			ucOneCycleInterruptDelay = 0;
			uint8_t InterruptSetandEn = ucGBMemoryRead(IE_ADDR) & ucGBMemoryRead(IF_ADDR);
			if      (InterruptSetandEn &  VBLANK_INTERRUPT){ vGBMemoryResetBit(IF_ADDR, 0); vGBFunctionPUSH(&reg.SP, &reg.PC); reg.PC = VBLANK_VECTOR;
			}else if(InterruptSetandEn & LCDSTAT_INTERRUPT){ vGBMemoryResetBit(IF_ADDR, 1); vGBFunctionPUSH(&reg.SP, &reg.PC); reg.PC = LCDSTAT_VECTOR;
			}else if(InterruptSetandEn &   TIMER_INTERRUPT){ vGBMemoryResetBit(IF_ADDR, 2); vGBFunctionPUSH(&reg.SP, &reg.PC); reg.PC = TIMER_VECTOR;
			}else if(InterruptSetandEn &  SERIAL_INTERRUPT){ vGBMemoryResetBit(IF_ADDR, 3); vGBFunctionPUSH(&reg.SP, &reg.PC); reg.PC = SERIAL_VECTOR;
			}else if(InterruptSetandEn &  JOYPAD_INTERRUPT){ vGBMemoryResetBit(IF_ADDR, 4); vGBFunctionPUSH(&reg.SP, &reg.PC); reg.PC = JOYPAD_VECTOR;
			}

			tStates += 5*4;
		}

	}else{
		ucOneCycleInterruptDelay++;
	}
}

void vGBCPUinstr(uint8_t opcode){

	if(reg.PC == 0x40){
		num = 5;
	}
	vGBMemorySetOP(opcode);

	reg.PC += (opcode != 0xCB) ? instructions[opcode].bytes : prefix_instructions[ucGBMemoryRead(reg.PC + 1)].bytes;
	((void (*)(void))instructions[opcode].instr)();

	if (opcode == 0xCB){
		tStates = prefix_instructions[ucGBMemoryRead(reg.PC - 1)].Tstate;
	}else if(instructions[opcode].Tstate == 255){
		tStates = customDuration;
	}else{
		tStates = instructions[opcode].Tstate;
	}

	if(ucInterruptMasterEnable == 1)
			vGBCPUInterruptHandler();

	//if( print == 1)
	//	vGBMemoryPrint();

	if(reg.PC == 0x2CA){
		print = 1;
	}
}
