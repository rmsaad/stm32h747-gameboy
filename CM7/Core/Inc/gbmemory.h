/*
 * gbmemory.h
 *
 *  Created on: Mar 28, 2021
 *      Author: Rami
 */

#ifndef INC_GBMEMORY_H_
#define INC_GBMEMORY_H_

#include <stdio.h>

// ppu modes
#define MODE_0 0    // HBLANK
#define MODE_1 1    // VBLANK
#define MODE_2 2    // accessing OAM
#define MODE_3 3    // accessing VRAM

#define CARTRAM_BASE 0xA000
#define VRAM_BASE    0x8000

// Drawing Related Register Addresses
#define JOY_ADDR  0xFF00
#define LCDC_ADDR 0xFF40
#define STAT_ADDR 0xFF41
#define SCY_ADDR  0xFF42
#define SCX_ADDR  0xFF43
#define LY_ADDR   0xFF44
#define LYC_ADDR  0xFF45
#define BGP_ADDR  0xFF47

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

void vGBMemoryLoad(const void* data, uint32_t size);
void vGBMemoryWrite(uint16_t address, uint8_t data);
uint8_t ucGBMemoryRead(uint16_t address);
uint16_t usGBMemoryReadShort(uint16_t address);
void vGBMemorySetBit(uint16_t address, uint8_t bit);
void vGBMemoryResetBit(uint16_t address, uint8_t bit);
void vGBMemoryPrint();
void vGBMemorySetOP(uint8_t op);
void vGBMemoryInit();
#endif /* INC_GBMEMORY_H_ */

