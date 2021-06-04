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

// https://gbdev.io/gb-opcodes/optables/#prefixed

void vNOP();
void vLD_BC_d16();
void vLD_BC_A();
void vINC_BC();
void vINC_B();
void vDEC_B();
void vLD_B_d8();
void vRLCA();
void vLD_a16_SP();
void vADD_HL_BC();
void vLD_A_BC();
void vDEC_BC();
void vINC_C();
void vDEC_C();
void vLD_C_d8();
void vRRCA();

void vSTOP();
void vLD_DE_d16();
void vLD_DE_A();
void vINC_DE();
void vINC_D();
void vDEC_D();
void vLD_D_d8();
void vRLA();
void vJR_r8();
void vADD_HL_DE();
void vLD_A_DE();
void vDEC_DE();
void vINC_E();
void vDEC_E();
void vLD_E_d8();
void vRRA();

void vJR_NZ_r8();
void vLD_HL_d16();
void vLD_HLI_A();
void vINC_HL();
void vINC_H();
void vDEC_H();
void vLD_H_d8();
void vDAA();
void vJR_Z_r8();
void vADD_HL_HL();
void vLD_A_HLI();
void vDEC_HL();
void vINC_L();
void vDEC_L();
void vLD_L_d8();
void vCPL();

void vJR_NC_r8();
void vLD_SP_d16();
void vLD_HLD_A();
void vINC_SP();


Assembly_Instr instructions[2] = {
		{vNOP, 1, 4},
		{vLD_BC_d16, 3, 12}
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
		//case 0x00: __asm volatile("MOV r0, r0"); break;							// NOP 						1  4
		case 0x01:	break;							// LD BC, d16				3  12
		case 0x02:	break;							// LD (BC), A				1  8
		case 0x03:	break;							// INC BC					1  8
		case 0x04:	break;							// INC B					1  4			Z 0 H -
		case 0x05:	break;							// DEC B					1  4			Z 1 H -
		case 0x06:	break;							// LD B, d8					2  8
		case 0x07:	break;							// RLCA						1  4			0 0 0 C
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
