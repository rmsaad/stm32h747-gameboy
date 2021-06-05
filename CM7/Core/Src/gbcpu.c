/*
 * gbcpu.c
 *
 *  Created on: Mar 28, 2021
 *      Author: Rami
 */

#include "gbcpu.h"
#include "gbmemory.h"
//#include "Tetris.gb.h"
//#include "dmg_boot.bin.h"

/* Private variables ---------------------------------------------------------*/
extern registers reg;

/* Game Boy CPI instruction set */

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
void vADC_A_HL();
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
void vLD_C_A();
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
void vLD_A_C();
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

/* structs */

Assembly_Instr instructions[256] = {
		{vNOP, 1, 4},
		{vLDs_BC_d16, 3, 12},
		{vLD_BC_A,1,8},
		{vINCs_BC,1,8},
		{vINC_B,1,4},
		{vDEC_B,1,4},
		{vLD_B_d8,2,8},
		{vRLCA,1,4},
		{vLDs_a16_SP,3,20},
		{vADDs_HL_BC,1,8},
		{vLD_A_BC,1,8},
		{vDECs_BC,1,8},
		{vINC_C,1,4},
		{vDEC_C,1,4},
		{vLD_C_d8,2,8},
		{vRRCA,1,4},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,},
		{,,}
};

void vNOP(){

}

void vLD_BC_d16(){

}

void vGBCPUreset(){
	// TEMP until OPCODES implemented
	reg.PC = 0x100;
	reg.SP = 0xFFFE;
	reg.AF = 0x01B0;
	reg.BC = 0x0013;
	reg.DE = 0x00D8;
	reg.HL = 0x014D;
}

void vGBCPUboot(){
	reg.PC = 0;
	while(reg.PC <= 0xFF){
		//vGBCPUinstr(dmg_boot_bin[reg.PC]);
	}
}

void vGBCPUinstr(uint8_t opcode){
	switch (opcode) {								// Instr				bytes  T-states		Flags

		case 0x08:	break;							// LD (a16), SP				3  20
		case 0x09:	break;							// ADD HL, BC				1  8			- 0 H C
		case 0x0A:	break;							// LD A, (BC)				1  8
		case 0x0B:	break;							// DEC BC					1  8
		case 0x0C:	break;							// INC C					1  4			Z 0 H -
		case 0x0D:	break;							// DEC C					1  4			Z 1 H -
		case 0x0E:	break;							// LD C, d8					2  8
		case 0x0F:	break;							// RRCA						1  4			0 0 0 C
		case 0x10:	break;							// STOP d8					2  4
		case 0x11:	break;							// LD DE, d16				3  12
		case 0x12:	break;							// LD (DE), A				1  8
		case 0x13:	break;							// INC DE					1  8
		case 0x14:	break;							// INC D					1  4			Z 0 H -
		case 0x15:	break;							// DEC D					1  4			Z 1 H -
		case 0x16:	break;							// LD D, d8					2  8
		case 0x17:	break;							// RLA						1  4			0 0 0 C
		case 0x18:	break;							// JR r8					2  12
		case 0x19:	break;							// ADD HL, DE				1  8			- 0 H C
		case 0x1A:	break;							// LD A, (DE)				1  8
		case 0x1B:	break;							// DEC DE					1  8
		case 0x1C:	break;							// INC E					1  4			Z 0 H -
		case 0x1D:	break;							// DEC E					1  4			Z 1 H -
		case 0x1E:	break;							// LD E, d8					2  8
		case 0x1F:	break;							// RRA						1  4			0 0 0 C
		case 0x20:	break;							// JR NZ, r8				2  12/8
		case 0x21:	break;							// LD HL, d16				3  12
		case 0x22:	break;							// LD (HL+), A				1  8
		case 0x23:	break;							// INC HL					1  8
		case 0x24:	break;							// INC H					1  4			Z 0 H -
		case 0x25:	break;							// DEC H					1  4			Z 1 H -
		case 0x26:	break;							// LD H, d8					2  8
		case 0x27:	break;							// DAA						1  4			Z - 0 C
		case 0x28:	break;							// JR Z, r8					2  12/8
		case 0x29:	break;							// ADD HL, HL				1  8			- 0 H C
		case 0x2A:	break;							// LD A, (HL+)				1  8
		case 0x2B:	break;							// DEC HL					1  8
		case 0x2C:	break;							// INC L					1  4			Z 0 H -
		case 0x2D:	break;							// DEC L					1  4			Z 1 H -
		case 0x2E:	break;							// LD L, d8					2  8
		case 0x2F:	break;							// CPL						1  4			- 1 1 -
		case 0x30:	break;							// JR NC, r8				2  12/8
		//case 0x31:	__asm volatile("MOV r0, r0"); break;							// LD SP, d16				3  12
		case 0x32:	break;							// LD (HL-), A				1  8
		case 0x33:	break;							// INC SP					1  8
		case 0x34:	break;							// INC (HL)					1  12			Z 0 H -
		case 0x35:	break;							// DEC (HL)					1  12			Z 1 H -
		case 0x36:	break;							// LD (HL), d8				2  12
		case 0x37:	break;							// SCF						1  4			- 0 0 1
		case 0x38:	break;							// JR C, r8					2  12/8
		case 0x39:	break;							// ADD HL, SP				1  8			- 0 H C
		case 0x3A:	break;							// LD A, (HL-)				1  8
		case 0x3B:	break;							// DEC SP					1  8
		case 0x3C:	break;							// INC A					1  4			Z 0 H -
		case 0x3D:	break;							// DEC A					1  4			Z 1 H -
		case 0x3E:	break;							// LD A, d8					2  8
		case 0x3F:	break;							// CCF						1  4			- 0 0 C
		case 0x40:	break;							// LD B, B					1  4
		case 0x41:	break;							// LD B, C					1  4
		case 0x42:	break;							// LD B, D					1  4
		case 0x43:	break;							// LD B, E					1  4
		case 0x44:	break;							// LD B, H					1  4
		case 0x45:	break;							// LD B, L					1  4
		case 0x46:	break;							// LD B, (HL)				1  8
		case 0x47:	break;							// LD B, A					1  4
		case 0x48:	break;							// LD C, B					1  4
		case 0x49:	break;							// LD C, C					1  4
		case 0x4A:	break;							// LD C, D					1  4
		case 0x4B:	break;							// LD C, E					1  4
		case 0x4C:	break;							// LD C, H					1  4
		case 0x4D:	break;							// LD C, L					1  4
		case 0x4E:	break;							// LD C, (HL)				1  8
		case 0x4F:	break;							// LD C, A					1  4
		case 0x50:	break;							// LD D, B					1  4
		case 0x51:	break;							// LD D, C					1  4
		case 0x52:	break;							// LD D, D					1  4
		case 0x53:	break;							// LD D, E					1  4
		case 0x54:	break;							// LD D, H					1  4
		case 0x55:	break;							// LD D, L					1  4
		case 0x56:	break;							// LD D, (HL)				1  8
		case 0x57:	break;							// LD D, A					1  4
		case 0x58:	break;							// LD E, B					1  4
		case 0x59:	break;							// LD E, C					1  4
		case 0x5A:	break;							// LD E, D					1  4
		case 0x5B:	break;							// LD E, E					1  4
		case 0x5C:	break;							// LD E, H					1  4
		case 0x5D:	break;							// LD E, L					1  4
		case 0x5E:	break;							// LD E, (HL)				1  8
		case 0x5F:	break;							// LD E, A					1  4
		case 0x60:	break;							// LD H, B					1  4
		case 0x61:	break;							// LD H, C					1  4
		case 0x62:	break;							// LD H, D					1  4
		case 0x63:	break;							// LD H, E					1  4
		case 0x64:	break;							// LD H, H					1  4
		case 0x65:	break;							// LD H, L					1  4
		case 0x66:	break;							// LD H, (HL)				1  8
		case 0x67:	break;							// LD H, A					1  4
		case 0x68:	break;							// LD L, B					1  4
		case 0x69:	break;							// LD L, C					1  4
		case 0x6A:	break;							// LD L, D					1  4
		case 0x6B:	break;							// LD L, E					1  4
		case 0x6C:	break;							// LD L, H					1  4
		case 0x6D:	break;							// LD L, L					1  4
		case 0x6E:	break;							// LD L, (HL)				1  8
		case 0x6F:	break;							// LD L, A					1  4
		case 0x70:	break;							// LD (HL), B				1  8
		case 0x71:	break;							// LD (HL), C				1  8
		case 0x72:	break;							// LD (HL), D				1  8
		case 0x73:	break;							// LD (HL), E				1  8
		case 0x74:	break;							// LD (HL), H				1  8
		case 0x75:	break;							// LD (HL), L				1  8
		case 0x76:	break;							// HALT						1  4
		case 0x77:	break;							// LD (HL), A				1  8
		case 0x78:	break;							// LD A, B					1  4
		case 0x79:	break;							// LD A, C					1  4
		case 0x7A:	break;							// LD A, D					1  4
		case 0x7B:	break;							// LD A, E					1  4
		case 0x7C:	break;							// LD A, H					1  4
		case 0x7D:	break;							// LD A, L					1  4
		case 0x7E:	break;							// LD A, (HL)				1  8
		case 0x7F:	break;							// LD A, A					1  4
		case 0x80:	break;							// ADD A, B					1  4			Z 0 H C
		case 0x81:	break;							// ADD A, C					1  4			Z 0 H C
		case 0x82:	break;							// ADD A, D					1  4			Z 0 H C
		case 0x83:	break;							// ADD A, E					1  4			Z 0 H C
		case 0x84:	break;							// ADD A, H					1  4			Z 0 H C
		case 0x85:	break;							// ADD A, L					1  4			Z 0 H C
		case 0x86:	break;							// ADD A, (HL)				1  8			Z 0 H C
		case 0x87:	break;							// ADD A, A					1  4			Z 0 H C
		case 0x88:	break;							// ADC A, B					1  4			Z 0 H C
		case 0x89:	break;							// ADC A, C					1  4			Z 0 H C
		case 0x8A:	break;							// ADC A, D					1  4			Z 0 H C
		case 0x8B:	break;							// ADC A, E					1  4			Z 0 H C
		case 0x8C:	break;							// ADC A, H					1  4			Z 0 H C
		case 0x8D:	break;							// ADC A, L					1  4			Z 0 H C
		case 0x8E:	break;							// ADC A, (HL)				1  8			Z 0 H C
		case 0x8F:	break;							// ADC A, A					1  4			Z 0 H C
		case 0x90:	break;							// SUB B					1  4			Z 1 H C
		case 0x91:	break;							// SUB C					1  4			Z 1 H C
		case 0x92:	break;							// SUB D					1  4			Z 1 H C
		case 0x93:	break;							// SUB E					1  4			Z 1 H C
		case 0x94:	break;							// SUB H					1  4			Z 1 H C
		case 0x95:	break;							// SUB L					1  4			Z 1 H C
		case 0x96:	break;							// SUB (HL)					1  8			Z 1 H C
		case 0x97:	break;							// SUB A					1  4			1 1 0 0
		case 0x98:	break;							// SBC A, B					1  4			Z 1 H C
		case 0x99:	break;							// SBC A, C					1  4			Z 1 H C
		case 0x9A:	break;							// SBC A, D					1  4			Z 1 H C
		case 0x9B:	break;							// SBC A, E					1  4			Z 1 H C
		case 0x9C:	break;							// SBC A, H					1  4			Z 1 H C
		case 0x9D:	break;							// SBC A, L					1  4			Z 1 H C
		case 0x9E:	break;							// SBC A, (HL)				1  8			Z 1 H C
		case 0x9F:	break;							// SBC A, A					1  4			Z 1 H C
		case 0xA0:	break;							// AND B					1  4			Z 0 1 0
		case 0xA1:	break;							// AND C					1  4			Z 0 1 0
		case 0xA2:	break;							// AND D					1  4			Z 0 1 0
		case 0xA3:	break;							// AND E					1  4			Z 0 1 0
		case 0xA4:	break;							// AND H					1  4			Z 0 1 0
		case 0xA5:	break;							// AND L					1  4			Z 0 1 0
		case 0xA6:	break;							// AND (HL)					1  8			Z 0 1 0
		case 0xA7:	break;							// AND A					1  4			Z 0 1 0
		case 0xA8:	break;							// XOR B					1  4			Z 0 0 0
		case 0xA9:	break;							// XOR C					1  4			Z 0 0 0
		case 0xAA:	break;							// XOR D					1  4			Z 0 0 0
		case 0xAB:	break;							// XOR E					1  4			Z 0 0 0
		case 0xAC:	break;							// XOR H					1  4			Z 0 0 0
		case 0xAD:	break;							// XOR L					1  4			Z 0 0 0
		case 0xAE:	break;							// XOR (HL)					1  8			Z 0 0 0
		case 0xAF:	break;							// XOR A					1  4			1 0 0 0
		case 0xB0:	break;							// OR B						1  4			Z 0 0 0
		case 0xB1:	break;							// OR C						1  4			Z 0 0 0
		case 0xB2:	break;							// OR D						1  4			Z 0 0 0
		case 0xB3:	break;							// OR E						1  4			Z 0 0 0
		case 0xB4:	break;							// OR H						1  4			Z 0 0 0
		case 0xB5:	break;							// OR L						1  4			Z 0 0 0
		case 0xB6:	break;							// OR (HL)					1  8			Z 0 0 0
		case 0xB7:	break;							// OR A						1  4			Z 0 0 0
		case 0xB8:	break;							// CP B						1  4			Z 1 H C
		case 0xB9:	break;							// CP C						1  4			Z 1 H C
		case 0xBA:	break;							// CP D						1  4			Z 1 H C
		case 0xBB:	break;							// CP E						1  4			Z 1 H C
		case 0xBC:	break;							// CP H						1  4			Z 1 H C
		case 0xBD:	break;							// CP L						1  4			Z 1 H C
		case 0xBE:	break;							// CP (HL)					1  8			Z 1 H C
		case 0xBF:	break;							// CP A						1  4			1 1 0 0
		case 0xC0:	break;							// RET NZ					1  20/8
		case 0xC1:	break;							// POP BC					1  12
		case 0xC2:	break;							// JP NZ, a16				3  16/12
		case 0xC3:	break;							// JP a16					3  16
		case 0xC4:	break;							// CALL NZ, a16				3  24/12
		case 0xC5:	break;							// PUSH BC					1  16
		case 0xC6:	break;							// ADD A, d8				2  8			Z 0 H C
		case 0xC7:	break;							// RST 00H					1  16
		case 0xC8:	break;							// RET Z					1  20/8
		case 0xC9:	break;							// RET						1  16
		case 0xCA:	break;							// JP Z, a16				3  16/12
		case 0xCB:	break;							// PREFIX					1  4
		case 0xCC:	break;							// CALL Z, a16				3  24/12
		case 0xCD:	break;							// CALL a16					3  24
		case 0xCE:	break;							// ADC A, d8				2  8			Z 0 H C
		case 0xCF:	break;							// RST 08H					1  16
		case 0xD0:	break;							// RET NC					1  20/8
		case 0xD1:	break;							// POP DE					1  12
		case 0xD2:	break;							// JP NC, a16				3  16/12

		case 0xD4:	break;							// CALL NC, a16				3  24/12
		case 0xD5:	break;							// PUSH DE					1  16
		case 0xD6:	break;							// SUB d8					2  8			Z 1 H C
		case 0xD7:	break;							// RST 10H					1  16
		case 0xD8:	break;							// RET C					1  20/8
		case 0xD9:	break;							// RETI						1  16
		case 0xDA:	break;							// JP C, a16				3  16/12

		case 0xDC:	break;							// CALL C, a16				3  24/12

		case 0xDE:	break;							// SBC A, d8				2  8			Z 1 H C
		case 0xDF:	break;							// RST 18H					1  16
		case 0xE0:	break;							// LDH (a8), A				2  12
		case 0xE1:	break;							// POP HL					1  12
		case 0xE2:	break;							// LD (C), A				1  8


		case 0xE5:	break;							// PUSH HL					1  16
		case 0xE6:	break;							// AND d8					2  8			Z 0 1 0
		case 0xE7:	break;							// RST 20H					1  16
		case 0xE8:	break;							// ADD SP, r8				2  16			0 0 H C
		case 0xE9:	break;							// JP HL					1  4
		case 0xEA:	break;							// LD (a16), A				3  16



		case 0xEE:	break;							// XOR d8					2  8			Z 0 0 0
		case 0xEF:	break;							// RST 28H					1  16
		case 0xF0:	break;							// LDH A, (a8)				2  12
		case 0xF1:	break;							// POP AF					1  12			Z N H C
		case 0xF2:	break;							// LD A, (C)				1  8
		case 0xF3:	break;							// DI						1  4

		case 0xF5:	break;							// PUSH AF					1  16
		case 0xF6:	break;							// OR d8					2  8			Z 0 0 0
		case 0xF7:	break;							// RST 30H					1  16
		case 0xF8:	break;							// LD HL, SP + r8			2  12			0 0 H C
		case 0xF9:	break;							// LD SP, HL				1  8
		case 0xFA:	break;							// LD A, (a16)				3  16
		case 0xFB:	break;							// EI						1  4

		case 0xFE:	break;							// CP d8					2  8			Z 1 H C
		case 0xFF:	break;							// RST 38H					1  16
		default:    break;							//
	}

}
