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

#include "main.h"
#include "gbfunctions.h"
#include "gbcpu.h"
#include "gbmemory.h"
#include "string.h"
#include "stm32h7_display.h"

// colors
#define BACKGROUND 0
#define WINDOW     1
#define SPRITE     2

#define DARKEST_GREEN  0XFF0F380FUL
#define DARK_GREEN     0XFF306230UL
#define LIGHT_GREEN    0XFF8BAC0FUL
#define LIGHTEST_GREEN 0XFF9BBC0FUL

#define SRAM1		   0x30000000UL

// Flags
uint16_t BackWinTileDataAddr;
uint16_t BackTileDisplayAddr;
uint8_t Mode;

uint32_t tStatesTotal;

uint8_t ly = 0;
uint8_t BGPColorToPalette[4];
uint8_t OBP0ColorToPalette[4];
uint8_t OBP1ColorToPalette[4];


// use this to access d2_sram

uint8_t *gb_frame = (uint8_t*)SRAM1;
//uint8_t gb_frame[144*160];

uint8_t ucBGWINline[160];
uint8_t scaleAmount = 3;
uint32_t curLine;
uint32_t lineadd;
/*Function Prototypes*/

void vCheckBGP();
void vCheckOBP0();
void vCheckOBP1();
void LYC_check(uint8_t ly);
void setMode(uint8_t mode);
void vGBPPUDrawLine(uint8_t ly, uint8_t SCX, uint8_t SCY);
uint16_t usGetBackWinTileDataSel();
uint16_t usGetBackTileDisplaySel();
uint16_t usGetWinTileDisplaySel();
/**
 * @brief Zeros All Memory in the Frame Buffer
 * @return Nothing
 */
void vSetFrameBuffer(){
	memset(gb_frame, 0, 160 * 144 * 4 * scaleAmount);
}
//38912
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
	static int n = 0;
	if(ucGBMemoryRead(LCDC_ADDR) & 0x80){															// check MSB of LCDC for screen en
		tStatesTotal += ucGetTstate();

		if (tStatesTotal > 456){												// end of hblank or vblank
			ly++;
			LYC_check(ly);
			if(ly > 153){												// end of vblank
				if(n % 1 == 0){
					displayFrameBuffer(gb_frame, scaleAmount);
				}
				n++;
				setMode(MODE_2);
				ly = 0;

				if(checkbit(ucGBMemoryRead(STAT_ADDR), 5))
					vGBMemorySetBit(IF_ADDR, 1);
			}

			vGBMemoryWrite(LY_ADDR, ly);								// update LY register
			tStatesTotal -= 456;
		}

		if (ly > 143){													// vblank
			if(Mode != MODE_1){
				setMode(MODE_1);
				if(checkbit(ucGBMemoryRead(STAT_ADDR), 4))
					vGBMemorySetBit(IF_ADDR, 1);
				if(ly == 0x90){
					vGBMemorySetBit(IF_ADDR, 0);
				}
			}
		}else{
			if (tStatesTotal <= 80 && Mode != MODE_2)											// oam
				setMode(MODE_2);
			else if(tStatesTotal > 80 && tStatesTotal <= 252 && Mode != MODE_3){										// vram

				if (n % 1 == 0){
					vGBPPUDrawLine(ly, ucGBMemoryRead(SCX_ADDR), ucGBMemoryRead(SCY_ADDR));
				}

				setMode(MODE_3);
			}else if(tStatesTotal > 252 && tStatesTotal <= 456 && Mode != MODE_0){										// hblank
				setMode(MODE_0);
				if(checkbit(ucGBMemoryRead(STAT_ADDR), 3))
					vGBMemorySetBit(IF_ADDR, 1);
			}
		}
	}else{
		ly = 0;
		vGBMemoryWrite(LY_ADDR, ly);
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
			case 0: BGPColorToPalette[i] = 1;  break;
			case 1: BGPColorToPalette[i] = 2;  break;
			case 2: BGPColorToPalette[i] = 3;  break;
			case 3: BGPColorToPalette[i] = 4;  break;
			default:                           break;
		}

	}

}

void vCheckOBP0(){
	uint8_t BGP = ucGBMemoryRead(OBP0_ADDR);
	for(int i = 0; i < 4; i++){
		switch ((BGP >> (i*2)) & 0x03) {
			case 0: OBP0ColorToPalette[i] = 0;    break;
			case 1: OBP0ColorToPalette[i] = 2;    break;
			case 2: OBP0ColorToPalette[i] = 3;    break;
			case 3: OBP0ColorToPalette[i] = 4;    break;
			default:                              break;
		}

	}
}

void vCheckOBP1(){
	uint8_t BGP = ucGBMemoryRead(OBP1_ADDR);
	for(int i = 0; i < 4; i++){
		switch ((BGP >> (i*2)) & 0x03) {
			case 0: OBP1ColorToPalette[i] = 0;  break;
			case 1: OBP1ColorToPalette[i] = 2;  break;
			case 2: OBP1ColorToPalette[i] = 3;  break;
			case 3: OBP1ColorToPalette[i] = 4;  break;
			default:                            break;
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
uint16_t usGetBackWinTileDataSel(){
	return (ucGBMemoryRead(LCDC_ADDR) & 0x10) ? TILE_DATA_UNSIGNED_ADDR : TILE_DATA_SIGNED_ADDR;
}

/**
 * @brief Read LCDC Register to find appropriate Background Tile Display Select Address
 * @details This function Reads from the LCDC Register and depending on the value of the 3rd bit, it will change the Background Tile
 * Display Address to 0X9C00 on high or to 0x9800 on low.
 * @return Nothing
 */
uint16_t usGetBackTileDisplaySel(){
	return (ucGBMemoryRead(LCDC_ADDR) & 0x08) ? TILE_MAP_LOCATION_HIGH : TILE_MAP_LOCATION_LOW;
}

/**
 *
 */
uint16_t usGetWinTileDisplaySel(){
	return (ucGBMemoryRead(LCDC_ADDR) & 0x40) ? TILE_MAP_LOCATION_HIGH : TILE_MAP_LOCATION_LOW;
}

/**
 * @brief Finds Specific line data of a tile using the tile_offset and line_offset
 * @details Called by vGBPPUDrawLine() and returns color information of a specific tile in the Gameboy's VRAM located by using the tile_offset and line_offset parameters.
 * @param tile_offset gives the address offset in the tile map in Gameboy's VRAM
 * @param line_offset gives the line offset in the tile
 * @return uint16_t data containing the color information of each pixel of a particular line belonging to a tile in the Gameboy's VRAM
 */
uint16_t getTileLineData(uint16_t tile_offset, uint8_t line_offset, uint16_t TileDataAddr, uint16_t DisplayAddr){
	if (TileDataAddr == 0x8000){
		return usGBMemoryReadShort(TileDataAddr + (ucGBMemoryRead(DisplayAddr + tile_offset) * 0x10) + line_offset);
	}else{
		int8_t temp  = (int8_t)(ucGBMemoryRead(DisplayAddr + tile_offset));
		uint16_t temp2 =( temp + 128) * 0x10;
		return usGBMemoryReadShort(TileDataAddr + temp2 + line_offset);
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
		if(checkbit(ucGBMemoryRead(STAT_ADDR), 6))
			vGBMemorySetBit(IF_ADDR, 1);
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

void updateBufferObj(uint8_t data, int pixelPos){
	pixelPos *= scaleAmount;
	for (int yStretch = 1; yStretch <= scaleAmount; yStretch++){
		for(int xStretch = 0; xStretch < scaleAmount; xStretch++){
			gb_frame[pixelPos + xStretch + (curLine) + (lineadd * yStretch)] = data;
		}
	}
}

void vGBPPUDrawLineBackground(uint8_t ly, uint8_t SCX, uint8_t SCY, uint16_t TileDataAddr, uint16_t DisplayAddr){
	uint16_t tile_offset = (((uint8_t)(SCY + ly) / 8) * 32) + (SCX / 8);			           // gives the address offset in the tile map
	uint8_t line_offset = (((SCY % 8) + ly) % 8) * 2;										   // gives the line offset in the tile
	uint8_t pixl_offset = SCX % 8;											                   // gives current pixel offset

	uint16_t first_tile = tile_offset % 32;
	uint16_t tile_data = getTileLineData(tile_offset, line_offset, TileDataAddr, DisplayAddr);                            // tile data holds tile line information

	for(int j = 0; j < 160; j++){

		uint8_t pixelData = 0;

		switch (((tile_data << pixl_offset) & 0x8080)) {
			case 0x0000: pixelData = BGPColorToPalette[0]; ucBGWINline[j] = 0; break;
			case 0x0080: pixelData = BGPColorToPalette[1]; ucBGWINline[j] = 1; break;
			case 0x8000: pixelData = BGPColorToPalette[2]; ucBGWINline[j] = 2; break;
			case 0x8080: pixelData = BGPColorToPalette[3]; ucBGWINline[j] = 3; break;
		}
		updateBufferObj(pixelData, j);

		pixl_offset++;

		if(pixl_offset == 8){
			tile_offset++;
			pixl_offset = 0;
			if(first_tile + (tile_offset % 32) >= 12 && (tile_offset % 32) < first_tile)
				tile_data = getTileLineData(tile_offset - 32, line_offset, TileDataAddr, DisplayAddr);
			else
				tile_data = getTileLineData(tile_offset, line_offset, TileDataAddr, DisplayAddr);


		}

	}
}

void vGBPPUDrawLineWindow(uint8_t ly, uint8_t WX, uint8_t WY, uint16_t TileDataAddr, uint16_t DisplayAddr){
	if(WY > ly || WY > 143 || WX > 166)
		return;

	uint16_t tile_offset = (((uint8_t)(ly - WY) / 8) * 32);			           // gives the address offset in the tile map
	uint8_t line_offset = (((ly - WY) % 8)) * 2;										   // gives the line offset in the tile
	uint8_t pixl_offset = (WX - 7) % 8;											                   // gives current pixel offset

	uint16_t tile_data = getTileLineData(tile_offset, line_offset, TileDataAddr, DisplayAddr);                            // tile data holds tile line information

	for(int j = (WX - 7); j < 160; j++){
		uint8_t pixelData = 0;

		switch (((tile_data << pixl_offset) & 0x8080)) {
			case 0x0000: pixelData = BGPColorToPalette[0]; ucBGWINline[j] = 0; break;
			case 0x0080: pixelData = BGPColorToPalette[1]; ucBGWINline[j] = 1; break;
			case 0x8000: pixelData = BGPColorToPalette[2]; ucBGWINline[j] = 2; break;
			case 0x8080: pixelData = BGPColorToPalette[3]; ucBGWINline[j] = 3; break;
		}
		updateBufferObj(pixelData, j);
			pixl_offset++;

			if(pixl_offset == 8){
				tile_offset++;
				pixl_offset = 0;
				tile_data = getTileLineData(tile_offset, line_offset, TileDataAddr, DisplayAddr);

			}

		}
}

void vGBPPUDrawLineObjects(uint8_t ly){
	for(int obj = 0; obj < 40; obj++){
		uint8_t yCoordinate = ucGBMemoryRead(OAM_BASE + (obj*4)) - 16;
		uint8_t xCoordinate = ucGBMemoryRead(OAM_BASE + (obj*4) + 1) - 8;
		uint8_t dataTile    = ucGBMemoryRead(OAM_BASE + (obj*4) + 2);
		uint8_t objPrio     = checkbit(ucGBMemoryRead(OAM_BASE + (obj*4) + 3), 7);
		uint8_t objYFlip    = checkbit(ucGBMemoryRead(OAM_BASE + (obj*4) + 3), 6);
		uint8_t objXFlip    = checkbit(ucGBMemoryRead(OAM_BASE + (obj*4) + 3), 5);
		uint8_t objPalette  = checkbit(ucGBMemoryRead(OAM_BASE + (obj*4) + 3), 4);

		if(yCoordinate <= ly && (yCoordinate + 8) > ly){

			uint8_t lineOffset = objYFlip ? (7 - (ly - yCoordinate)) * 2 : (ly - yCoordinate) * 2;
			uint16_t tile_data = usGBMemoryReadShort(TILE_DATA_UNSIGNED_ADDR + (dataTile * 0x10) + lineOffset);
			uint8_t *palette = (objPalette) ? &OBP1ColorToPalette[0] : &OBP0ColorToPalette[0];

			for(int pixelNum = 0; pixelNum < 8; pixelNum++){

				uint16_t colorInfo = (objXFlip) ? (((tile_data >> pixelNum) & 0x0101) << 7) : ((tile_data << pixelNum) & 0x8080);
				uint8_t pixelData = 0;

				switch (colorInfo) {
					case 0x0000: pixelData = palette[0]; break;
					case 0x0080: pixelData = palette[1]; break;
					case 0x8000: pixelData = palette[2]; break;
					case 0x8080: pixelData = palette[3]; break;
				}

				if(pixelData != 0 && xCoordinate + pixelNum >= 0 && (xCoordinate + pixelNum) < 160){
					if((objPrio) && ucBGWINline[xCoordinate + pixelNum]){

					}else{
						updateBufferObj(pixelData, xCoordinate + pixelNum);
					}
				}
			}

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
	// update Palettes
	vCheckBGP();
	vCheckOBP0();
	vCheckOBP1();
	uint16_t TileDataAddr = usGetBackWinTileDataSel();
	curLine = ly * scaleAmount * scaleAmount * 160;
	lineadd = scaleAmount * 160;

	if(ucGBMemoryRead(LCDC_ADDR) & 0x01){
		vGBPPUDrawLineBackground(ly, SCX, SCY, TileDataAddr, usGetBackTileDisplaySel());
		if(ucGBMemoryRead(LCDC_ADDR) & 0x20)
			vGBPPUDrawLineWindow(ly, ucGBMemoryRead(WX_ADDR), ucGBMemoryRead(WY_ADDR), TileDataAddr, usGetWinTileDisplaySel());
	}else{
		for(int j = 0; j < 160; j++){
			updateBufferObj(1, j);
		}
	}
	if(ucGBMemoryRead(LCDC_ADDR) & 0x02)
		vGBPPUDrawLineObjects(ly);

}


