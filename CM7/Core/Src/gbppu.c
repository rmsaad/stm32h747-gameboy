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

#define MAX_TILES_DECODED_PER_LINE 9

// colours
#define BACKGROUND 0
#define WINDOW     1
#define SPRITE     2

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
#define TILE_DATA_SIGNED_ADDR   0x8800

// LCDC BIT 3, TILE MAP OFFSETS BASES ADDRESSES
#define TILE_MAP_LOCATION_LOW  0x9800
#define TILE_MAP_LOCATION_HIGH 0x9C00

#define DARKEST_GREEN  0XFF0F380FUL
#define DARK_GREEN     0XFF306230UL
#define LIGHT_GREEN    0XFF8BAC0FUL
#define LIGHTEST_GREEN 0XFF9BBC0FUL

// Flags
uint8_t BackWinTileDataAddr;
uint8_t BackTileDisplayAddr;
uint8_t Mode;

extern uint32_t tStates;
uint8_t ly = 0;
uint32_t color_to_pallette[4];
uint32_t gb_frame[160*144];

void vCheckBGP();
void LYC_check(uint8_t ly);
void setMode(uint8_t mode);
void draw_line(uint8_t ly, uint8_t SCX, uint8_t SCY);
void vCheckBackWinTileDataSel();
void vCheckBackTileDisplaySel();

void gbPPUStep(){
	if(ucGBMemoryRead(LCDC_ADDR) & 0x80){															// check MSB of LCDC for screen en
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
			// draw frame
			setMode(MODE_1);
		}else{
			if (tStates <= 80 && Mode != MODE_2)											// oam
				setMode(MODE_2);
			else if(tStates <= 252 && Mode != MODE_3){										// vram
				vCheckBGP();
				vCheckBackWinTileDataSel();
				vCheckBackTileDisplaySel();
				draw_line(ly, ucGBMemoryRead(SCX_ADDR), ucGBMemoryRead(SCY_ADDR));
				setMode(MODE_3);
			}else if(tStates <= 456 && Mode != MODE_0)										// hblank
				setMode(MODE_0);
		}
	}
}

// check per line
void vCheckBGP(){
	uint8_t BGP = ucGBMemoryRead(BGP_ADDR);
	for(int i = 0; i < 4; i++){
		switch ((BGP >> (i*2)) & 0x03) {
			case 0: color_to_pallette[i] = LIGHTEST_GREEN; break;
			case 1: color_to_pallette[i] = LIGHT_GREEN;    break;
			case 2: color_to_pallette[i] = DARK_GREEN;     break;
			case 3: color_to_pallette[i] = DARKEST_GREEN;  break;
			default:                                       break;
		}

	}

}



void vCheckBackWinTileDataSel(){
	BackWinTileDataAddr = (ucGBMemoryRead(LCDC_ADDR) & 0x10) ? TILE_DATA_UNSIGNED_ADDR : TILE_DATA_SIGNED_ADDR;
}

void vCheckBackTileDisplaySel(){
	BackTileDisplayAddr = (ucGBMemoryRead(LCDC_ADDR) & 0x08) ? TILE_MAP_LOCATION_HIGH : TILE_MAP_LOCATION_LOW;
}

uint16_t getTileLineData(uint8_t tile_offset, uint8_t line_offset){
	if (BackTileDisplayAddr == 0x8000){
		return usGBMemoryReadShort(BackWinTileDataAddr + ucGBMemoryRead(BackTileDisplayAddr + tile_offset) + line_offset);
	}else{
		int8_t temp  = ucGBMemoryRead(BackTileDisplayAddr + tile_offset);
		uint8_t temp2 = temp + 128;
		return usGBMemoryReadShort(BackWinTileDataAddr + temp2 + line_offset);
	}

}

void draw_line(uint8_t ly, uint8_t SCX, uint8_t SCY){


	uint8_t tile_offset = (((SCY + ly) / 8) * 32) + (SCX / 8);			                   	   // gives the address offset in the tile map
	uint8_t line_offset = (SCY % 8) * 2;									                   // gives the line offset in the tile
	uint8_t pixl_offset = SCX % 8;											                   // gives current pixel offset

	uint16_t tile_data = getTileLineData(tile_offset, line_offset);           // tile data holds tile line information

		for(int j = 0; j < 160; j++){
			switch ((tile_data & 0x00010001) >> pixl_offset){
				case 0x000: gb_frame[j + (ly * 160)] = color_to_pallette[0]; break;
				case 0x001: gb_frame[j + (ly * 160)] = color_to_pallette[2]; break;
				case 0x100: gb_frame[j + (ly * 160)] = color_to_pallette[1]; break;
				case 0x101: gb_frame[j + (ly * 160)] = color_to_pallette[3]; break;
				default: break;
			}

			pixl_offset++;

			if(pixl_offset == 8){
				tile_offset++;
				pixl_offset = 0;
				tile_data = getTileLineData(tile_offset, line_offset);
			}
		}
}

void drawFrame(){

		if(ucGBMemoryRead(LCDC_ADDR) & 0x10){														// check bit 4 for addressing BG and Windows
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
	Mode = mode;
	switch (mode) {
		case MODE_0: vGBMemoryResetBit(STAT_ADDR, 1); vGBMemoryResetBit(STAT_ADDR, 0); break;		// 00
		case MODE_1: vGBMemoryResetBit(STAT_ADDR, 1);   vGBMemorySetBit(STAT_ADDR, 0); break;		// 01
		case MODE_2:   vGBMemorySetBit(STAT_ADDR, 1); vGBMemoryResetBit(STAT_ADDR, 0); break;		// 10
		case MODE_3:   vGBMemorySetBit(STAT_ADDR, 1);   vGBMemorySetBit(STAT_ADDR, 0); break;		// 11
		default:                                                                       break;
	}
}
