/*
 * gbmemory.h
 *
 *  Created on: Mar 28, 2021
 *      Author: Rami
 */

#ifndef INC_GBMEMORY_H_
#define INC_GBMEMORY_H_

#include <stdio.h>
#include <stdint.h>

// ppu modes
#define MODE_0 0    // HBLANK
#define MODE_1 1    // VBLANK
#define MODE_2 2    // accessing OAM
#define MODE_3 3    // accessing VRAM

#define CARTROM_BANK0 0x0000
#define CARTROM_BANKX 0x4000
#define VRAM_BASE     0x8000
#define CARTRAM_BASE  0xA000
#define GBRAM_BANK0   0xC000
#define ECHORAM_BASE  0xE000
#define OAM_BASE      0xFE00
#define IO_BASE       0xFF00

// Drawing Related Register Addresses
#define JOY_ADDR   0xFF00
#define SB_ADDR	   0xFF01
#define STC_ADDR   0xFF02
#define DIV_ADDR   0xFF04
#define TIMA_ADDR  0xFF05
#define TMA_ADDR   0xFF06
#define TAC_ADDR   0xFF07

#define NR10_ADDR  0xFF10
#define NR11_ADDR  0xFF11
#define NR12_ADDR  0xFF12
#define NR13_ADDR  0xFF13
#define NR14_ADDR  0xFF14
#define NR21_ADDR  0xFF16
#define NR22_ADDR  0xFF17
#define NR23_ADDR  0xFF18
#define NR24_ADDR  0xFF19
#define NR30_ADDR  0xFF1A
#define NR31_ADDR  0xFF1B
#define NR32_ADDR  0xFF1C
#define NR33_ADDR  0xFF1D
#define NR34_ADDR  0xFF1E
#define NR41_ADDR  0xFF20
#define NR42_ADDR  0xFF21
#define NR43_ADDR  0xFF22
#define NR44_ADDR  0xFF23
#define NR50_ADDR  0xFF24
#define NR51_ADDR  0xFF25
#define NR52_ADDR  0xFF26
#define WPRAM_BASE 0xFF30

#define LCDC_ADDR  0xFF40
#define STAT_ADDR  0xFF41
#define SCY_ADDR   0xFF42
#define SCX_ADDR   0xFF43
#define LY_ADDR    0xFF44
#define LYC_ADDR   0xFF45
#define DMA_ADDR   0xFF46
#define BGP_ADDR   0xFF47
#define OBP0_ADDR  0xFF48
#define OBP1_ADDR  0xFF49
#define WY_ADDR    0xFF4A
#define WX_ADDR    0xFF4B

#define IF_ADDR    0xFF0F
#define IE_ADDR    0xFFFF

// LCDC BIT 4, TILE DATA BASE ADDRESSES
#define TILE_DATA_UNSIGNED_ADDR 0x8000
#define TILE_DATA_SIGNED_ADDR   0x8800

// LCDC BIT 3, TILE MAP OFFSETS BASES ADDRESSES
#define TILE_MAP_LOCATION_LOW  0x9800
#define TILE_MAP_LOCATION_HIGH 0x9C00

typedef struct{
	union{
	  struct{
		uint8_t F;
		uint8_t A;
	  };
	  uint16_t AF;
	};

	union{
	  struct{
		uint8_t C;
		uint8_t B;
	  };
	  uint16_t BC;
	};

	union{
	  struct{
		uint8_t E;
		uint8_t D;
	  };
	  uint16_t DE;
	};

	union{
	  struct{
		uint8_t L;
		uint8_t H;
	  };
	  uint16_t HL;
	};

	uint16_t SP;

	uint16_t PC;
}registers;

typedef struct{
	uint8_t ram[65536];
}memory;

/* Public function prototypes -----------------------------------------------*/

void vGBMemorySetControlFunctionPtr(uint8_t (*controlsFunctionPtr)(uint8_t*, uint8_t*));
void vGBMemorySetROM(const unsigned char* pROM);
const unsigned char* ucGBMemoryGetRomPointer();
void vGBMemoryLoad(const void* data, uint32_t size);
void vGBMemoryWrite(uint16_t address, uint8_t data);
void vGBMemoryWriteShort(uint16_t address, uint16_t data);
uint8_t ucGBMemoryRead(uint16_t address);
uint16_t usGBMemoryReadShort(uint16_t address);
void vGBMemoryIncTimers(uint8_t duration);
void vGBMemorySetBit(uint16_t address, uint8_t bit);
void vGBMemoryResetBit(uint16_t address, uint8_t bit);
void vGBMemoryPrint();
void vGBMemorySetOP(uint8_t op);
void vGBMemoryInit();
#endif /* INC_GBMEMORY_H_ */

