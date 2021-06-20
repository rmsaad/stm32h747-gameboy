/**
  ******************************************************************************
  * @file           : gbppu.c
  * @brief          : Gameboy PPU Functionality
  * @author         : Rami
  * @date           : Jun 11, 2021
  *
  ******************************************************************************
  *
  ******************************************************************************
  */


#include "gbcpu.h"
#include "gbmemory.h"
#include "string.h"
#include "stm32h7_display.h"

// ppu modes
#define MODE_0 0    // HBLANK
#define MODE_1 1    // VBLANK
#define MODE_2 2    // accessing OAM
#define MODE_3 3    // accessing VRAM

// colors
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

#define SRAM1		   0xD1000000UL

// Flags
uint16_t BackWinTileDataAddr;
uint16_t BackTileDisplayAddr;
uint8_t Mode;
uint8_t ucSCY;
uint8_t ucSCX;

uint32_t tStatesTotal;

uint8_t ly = 0;
uint32_t color_to_pallette[4];

// use this to access d2_sram
uint32_t *gb_frame = (uint32_t*)SRAM1;

/*Function Prototypes*/

void vCheckBGP();
void LYC_check(uint8_t ly);
void setMode(uint8_t mode);
void vGBPPUDrawLine(uint8_t ly, uint8_t SCX, uint8_t SCY);
void vCheckBackWinTileDataSel();
void vCheckBackTileDisplaySel();

/**
 * @brief Zeros All Memory in the Frame Buffer
 * @return Nothing
 */
void vSetFrameBuffer(){
	memset(gb_frame, 0, 160*144*4);
}

/**
  * @brief Steps the PPU by tStates
  * @details This function steps the PPU by the tStates variable if the screen enable (7th) bit of LCDC Register is high,
  * the function sets the mode in the STAT Register using the setMode() function depending on how many tStates have elapsed.
  * after a full line is has passed the LY register is updated vGBPPUDrawLine() is called to place data in the frame buffer.
  * @return Nothing
  * @note The function and the Gameboy both mimic CRT Displays in that their are both a horizontal H-Blank after a Line is "drawn"
  * and there is a vertical V-Blank after a full frame is drawn
  * @attention The placement of the functions vCheckBackWinTileDataSel() and vCheckBackTileDisplaySel() are just educated guesses at
  * the current moment, more research into when and how often these Registers are updated must be conducted.
  */
void gbPPUStep(){

	if(ucGBMemoryRead(LCDC_ADDR) & 0x80){															// check MSB of LCDC for screen en
		tStatesTotal += ucGetTstate();
		if (tStatesTotal > 456){												// end of hblank or vblank
			ly++;

			if(ly > 153){												// end of vblank
				dummy2_code(gb_frame);
				setMode(MODE_2);
				ly = 0;
			}

			vGBMemoryWrite(LY_ADDR, ly);								// update LY register
			tStatesTotal -= 456;
		}

		LYC_check(ly);

		if (ly > 143){													// vblank
			setMode(MODE_1);
		}else{
			if (tStatesTotal <= 80 && Mode != MODE_2)											// oam
				setMode(MODE_2);
			else if(tStatesTotal > 80 && tStatesTotal <= 252 && Mode != MODE_3){										// vram
				vCheckBGP();
				vCheckBackWinTileDataSel();
				vCheckBackTileDisplaySel();
				ucSCY = ucGBMemoryRead(SCY_ADDR);
				ucSCX = ucGBMemoryRead(SCX_ADDR);
				vGBPPUDrawLine(ly, ucSCX, ucSCY);
				setMode(MODE_3);
			}else if(tStatesTotal > 252 && tStatesTotal <= 456 && Mode != MODE_0)										// hblank
				setMode(MODE_0);
		}
	}
}

/**
 * @brief Background color to palette translation
 * @details Matches a color to its appropriate palette based on value of the Background Palette Register
 * @return Nothing
 */
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

/**
 * @brief Read LCDC Register to find appropriate Background & Tile Data Address
 * @details This function Reads from the LCDC Register and depending on the value of the 4th bit, it will change the Background
 * & Window Tile Data Select Address to 0x8000 on high or to 0x8800 on low.
 * @return Nothing
 * @note Background & Window Tile Data Select should be 0x9000 on a low 4th bit in a real Gameboy,
 * but it is much simpler to implement this way with emulation, the  0x400 offset is accounted for in getTileLineData()
 */
void vCheckBackWinTileDataSel(){
	BackWinTileDataAddr = (ucGBMemoryRead(LCDC_ADDR) & 0x10) ? TILE_DATA_UNSIGNED_ADDR : TILE_DATA_SIGNED_ADDR;
}

/**
 * @brief Read LCDC Register to find appropriate Background Tile Display Select Address
 * @details This function Reads from the LCDC Register and depending on the value of the 3rd bit, it will change the Background Tile
 * Display Address to 0X9C00 on high or to 0x9800 on low.
 * @return Nothing
 */
void vCheckBackTileDisplaySel(){
	BackTileDisplayAddr = (ucGBMemoryRead(LCDC_ADDR) & 0x08) ? TILE_MAP_LOCATION_HIGH : TILE_MAP_LOCATION_LOW;
}

/**
 * @brief Finds Specific line data of a tile using the tile_offset and line_offset
 * @details Called by vGBPPUDrawLine() and returns color information of a specific tile in the Gameboy's VRAM located by using the tile_offset and line_offset parameters.
 * @param tile_offset gives the address offset in the tile map in Gameboy's VRAM
 * @param line_offset gives the line offset in the tile
 * @return uint16_t data containing the color information of each pixel of a particular line belonging to a tile in the Gameboy's VRAM
 */
uint16_t getTileLineData(uint16_t tile_offset, uint8_t line_offset){
	if (BackWinTileDataAddr == 0x8000){
		return usGBMemoryReadShort(BackWinTileDataAddr + (ucGBMemoryRead(BackTileDisplayAddr + tile_offset) * 0x10) + line_offset);
		//return usGBMemoryReadShort(0x8000 + 0x190 + line_offset);
	}else{
		int8_t temp  = (int8_t)(ucGBMemoryRead(BackTileDisplayAddr + tile_offset));
		uint16_t temp2 =( temp + 128) * 0x10;
		return usGBMemoryReadShort(BackWinTileDataAddr + temp2 + line_offset);
		//return usGBMemoryReadShort(BackWinTileDataAddr + (ucGBMemoryRead(0x9910) * 0x10) + line_offset);
		//return usGBMemoryReadShort(0x8000 + 0x190 + line_offset);
	}

}

/**
 * @brief Compares LY Register to LYC
 * @details LYC Register compares itself to LY Register and Sets the appropriate flag (2nd bit) in the STAT Register if they hold the same value
 * @param ly value of the LY Register
 * @return Nothing
 */
void LYC_check(uint8_t ly){
	if(ly == ucGBMemoryRead(LYC_ADDR)){
		vGBMemorySetBit(STAT_ADDR, 2);
	}else{
		vGBMemoryResetBit(STAT_ADDR, 2);
	}
}

/**
 * @brief Sets the mode of the STAT Register
 * @param mode current status of the LCD controller
 * @return Nothing
 */
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

/**
 *
 * @param res
 * @param j
 * @param amt
 */
void update_buffer(uint16_t res, int pixelPos, uint16_t amt){
	for (int n = 1; n <= amt; n++){
		switch (res){
				case 0x0000: gb_frame[pixelPos + (2 * ly * amt * 160) + (amt * 160 * n)] = color_to_pallette[0]; gb_frame[(pixelPos+1) + (2 * ly * amt * 160)  + (amt * 160 * n)] = color_to_pallette[0]; break;
				case 0x0080: gb_frame[pixelPos + (2 * ly * amt * 160) + (amt * 160 * n)] = color_to_pallette[2]; gb_frame[(pixelPos+1) + (2 * ly * amt * 160)  + (amt * 160 * n)] = color_to_pallette[2]; break;
				case 0x8000: gb_frame[pixelPos + (2 * ly * amt * 160) + (amt * 160 * n)] = color_to_pallette[1]; gb_frame[(pixelPos+1) + (2 * ly * amt * 160)  + (amt * 160 * n)] = color_to_pallette[1]; break;
				case 0x8080: gb_frame[pixelPos + (2 * ly * amt * 160) + (amt * 160 * n)] = color_to_pallette[3]; gb_frame[(pixelPos+1) + (2 * ly * amt * 160)  + (amt * 160 * n)] = color_to_pallette[3]; break;
				default: break;
			}
	}

}

/**
 * @brief Update data in the frame buffer for 1 line of the Gameboy
 * @details populates the frame buffer with data related to a line ly
 * @param ly lY Register Data
 * @param SCX Scroll X Register
 * @param SCY Scroll Y Register
 * @returns Nothing
 */
void vGBPPUDrawLine(uint8_t ly, uint8_t SCX, uint8_t SCY){

	uint16_t tile_offset = (((SCY + ly) / 8) * 32) + (SCX / 8);			                   	   // gives the address offset in the tile map
	uint8_t line_offset = (((SCY % 8) + ly) % 8) * 2;										   // gives the line offset in the tile
	uint8_t pixl_offset = SCX % 8;											                   // gives current pixel offset

	uint16_t tile_data = getTileLineData(tile_offset, line_offset);                            // tile data holds tile line information

		for(int j = 0; j < 160*2; j+= 2){

			update_buffer(((tile_data << pixl_offset) & 0x8080), j,  2);
			pixl_offset++;

			if(pixl_offset == 8){
				tile_offset++;
				pixl_offset = 0;
				tile_data = getTileLineData(tile_offset, line_offset);

			}

		}

}


