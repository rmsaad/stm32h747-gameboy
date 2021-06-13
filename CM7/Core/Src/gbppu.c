/*
 * gbppu.c
 *
 *  Created on: Jun 11, 2021
 *      Author: Rami
 */

#include "gbcpu.h"
#include "gbmemory.h"

// ppu modes
#define MODE_0 0    // HBLANK
#define MODE_1 1    // VBLANK
#define MODE_2 2    // accessing OAM
#define MODE_3 3    // accessing VRAM

// colours
#define COLOUR_0 0
#define COLOUR_1 1
#define COLOUR_2 2
#define COLOUR_3 3

//
#define PALLETTE_

// Drawing Related Register Addresses
#define LCDC_ADDR 0xFF40
#define STAT_ADDR 0xFF41
#define SCY_ADDR  0xFF42
#define SCX_ADDR  0xFF43
#define LY_ADDR   0xFF44
#define LYC_ADDR  0xFF45
#define BGP_ADDR  0xFF47

// LCDC BIT 4, TILE DATA BASE ADDRESSES
#define TILE_DATA_UNSIGNED_ADDR 0x8000
#define TILE_DATA_SIGNED_ADDR   0x9000

// LCDC BIT 3, TILE MAP OFFSETS BASES ADDRESSES
#define TILE_MAP_LOCATION_LOW  0x9800
#define TILE_MAP_LOCATION_HIGH 0x9C00

// Flags


extern uint32_t tStates;
uint8_t ly = 0;


void LYC_check(uint8_t ly);
void setMode(uint8_t mode);

void gbPPUStep(){

	if (tStates > 456){												// end of hblank or vblank
		ly++;

		if(ly > 153){												// end of vblank
			setMode(MODE_2);
			ly = 0;
		}

		vGBMemoryWrite(LY_ADDR, ly);								// update LY register
		tStates -= 456;

	}

	LYC_check(ly);

	if (ly > 143){													// vblank
		setMode(MODE_1);
	}else{
		if (tStates <= 80)											// oam
			setMode(MODE_2);
		else if(tStates <= 252)										// vram
			setMode(MODE_3);
		else if(tStates <= 456)										// hblank
			setMode(MODE_0);
	}
}

void decode_tile(){

}

void draw_line(uint8_t ly, uint8_t SCX, uint8_t SCY){

	uint8_t first_tile_offset = ((SCY + ly) / 8) * 32;				// finding y offset in Tile map


	// (scy+ly) div 8 for y pos tile
	first_tile_offset = ((SCY + ly) / 8) * 32;						// finding y offset in Tile map

	// scx mod 8 == 0 only 8 needed, else 9 based on				// finding leftmost tile in Tile map
	// decode line segments based on tiles from those registers
}

void drawFrame(){
	if(ucGBMemoryRead(LCDC_ADDR) & 0x80){															// check MSB of LCDC for screen en
		if(ucGBMemoryRead(LCDC_ADDR) & 0x10){														// check bit 4 for addressing BG and Windows
																										// 9000 base address -127 to 128
		}else{
																										// 8000 base address 0 to 255
		}

	}else{
		// no picture on screen
	}
}

void LYC_check(uint8_t ly){
	if(ly == ucGBMemoryRead(LYC_ADDR)){
		vGBMemorySetBit(STAT_ADDR, 2);
	}else{
		vGBMemoryResetBit(STAT_ADDR, 2);
	}
}

void setMode(uint8_t mode){
	switch (mode) {
		case MODE_0: vGBMemoryResetBit(STAT_ADDR, 1); vGBMemoryResetBit(STAT_ADDR, 0); break;		// 00
		case MODE_1: vGBMemoryResetBit(STAT_ADDR, 1);   vGBMemorySetBit(STAT_ADDR, 0); break;		// 01
		case MODE_2:   vGBMemorySetBit(STAT_ADDR, 1); vGBMemoryResetBit(STAT_ADDR, 0); break;		// 10
		case MODE_3:   vGBMemorySetBit(STAT_ADDR, 1);   vGBMemorySetBit(STAT_ADDR, 0); break;		// 11
		default:                                                                       break;
	}
}
