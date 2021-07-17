/**
  ******************************************************************************
  * @file           : gbppu.c
  * @brief          : Gameboy PPU Functionality
 *			          This file emulates all functionality of the Gameboy Pixel/Picture Processing Unit (PPU).
  * 				  This includes drawing the Background, Window, and Object Layers to a frame buffer to be
  * 				  displayed 60 times a second.
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

// Frame buffer start address
#define SRAM1 0x30000000UL

// PPU Registers
uint8_t  ucLY = 0;
uint8_t  ucMode;

// State Ticks
uint32_t ultStatesTotal;

// Palettes
uint8_t  ucBGPColorToPalette[4];
uint8_t  ucOBP0ColorToPalette[4];
uint8_t  ucOBP1ColorToPalette[4];

// Frame Buffer variables
uint8_t  *ucGBFrame = (uint8_t*)SRAM1;
uint8_t  ucBGWINline[160];
uint8_t  ulScaleAmount = 3;
uint32_t ulCurLine;
uint32_t ulLineAdd;

/*Function Prototypes*/
void prvCheckBGP();
void prvCheckOBP0();
void prvCheckOBP1();
void prvCheckLYC(uint8_t ly);
void prvSetMode(uint8_t mode);
void prvUpdateBuffer(uint8_t data, int pixelPos);
void prvGBPPUDrawLineBackground(uint8_t ly, uint8_t SCX, uint8_t SCY, uint16_t TileDataAddr, uint16_t DisplayAddr);
void prvGBPPUDrawLineWindow(uint8_t ly, uint8_t WX, uint8_t WY, uint16_t TileDataAddr, uint16_t DisplayAddr);
void prvGBPPUDrawLineObjects(uint8_t ly);
void prvGBPPUDrawLine(uint8_t ly, uint8_t SCX, uint8_t SCY);
uint16_t prvGetBackWinTileDataSel();
uint16_t prvGetBackTileDisplaySel();
uint16_t prvGetWinTileDisplaySel();

/**
 * @brief Zeros All Memory in the Frame Buffer
 * @return Nothing
 */
void vSetFrameBuffer(){
    memset(ucGBFrame, 0, 160 * 144 * 4 * ulScaleAmount);
}


/**
  * @brief Steps the PPU by tStates
  * @details This function steps the PPU by the tStates variable if the screen enable (7th) bit of LCDC Register is high,
  * the function sets the mode in the STAT Register using the prvSetMode() function depending on how many tStates have elapsed.
  * after a full line is has passed the LY register is updated prvGBPPUDrawLine() is called to place data in the frame buffer.
  * @return Nothing
  * @note The function and the Gameboy both mimic CRT Displays in that their are both a horizontal H-Blank after a Line is "drawn"
  * and there is a vertical V-Blank after a full frame is drawn
  * @attention The placement of the functions vCheckBackWinTileDataSel() and vCheckBackTileDisplaySel() are just educated guesses at
  * the current moment, more research into when and how often these Registers are updated must be conducted.
  */
void vGBPPUStep(){
    static uint8_t framePerSecondLimiter = 0;

    if(ucGBMemoryRead(LCDC_ADDR) & 0x80){                                                           // check MSB of LCDC for screen en
        ultStatesTotal += ucGetTstate();

        if(ultStatesTotal > 456){                                                                   // end of hblank or vblank
            ucLY++;
            prvCheckLYC(ucLY);
            if(ucLY > 153){                                                                         // end of vblank
                if(framePerSecondLimiter % 1 == 0){
                    displayFrameBuffer(ucGBFrame, ulScaleAmount);
                }
                framePerSecondLimiter++;
                prvSetMode(MODE_2);
                ucLY = 0;

                if(ucCheckBit(ucGBMemoryRead(STAT_ADDR), 5))
                    vGBMemorySetBit(IF_ADDR, 1);
            }

            vGBMemoryWrite(LY_ADDR, ucLY);                                                          // update LY register
            ultStatesTotal -= 456;
        }

        if (ucLY > 143){                                                                            // vblank region
            if(ucMode != MODE_1){
                prvSetMode(MODE_1);
                if(ucCheckBit(ucGBMemoryRead(STAT_ADDR), 4))
                    vGBMemorySetBit(IF_ADDR, 1);
                if(ucLY == 0x90){
                    vGBMemorySetBit(IF_ADDR, 0);
                }
            }
        }else{
            if (ultStatesTotal <= 80 && ucMode != MODE_2)                                           // oam region
                prvSetMode(MODE_2);
            else if(ultStatesTotal > 80 && ultStatesTotal <= 252 && ucMode != MODE_3){              // vram region

                if (framePerSecondLimiter % 1 == 0){
                    prvGBPPUDrawLine(ucLY, ucGBMemoryRead(SCX_ADDR), ucGBMemoryRead(SCY_ADDR));
                }

                prvSetMode(MODE_3);
            }else if(ultStatesTotal > 252 && ultStatesTotal <= 456 && ucMode != MODE_0){            // hblank region
                prvSetMode(MODE_0);
                if(ucCheckBit(ucGBMemoryRead(STAT_ADDR), 3))
                    vGBMemorySetBit(IF_ADDR, 1);
            }
        }

    }else{                                                                                          // screen is not enabled
        ucLY = 0;
        vGBMemoryWrite(LY_ADDR, ucLY);
    }
}

/**
 * @brief Background and window color to palette conversion
 * @details Updates a color to its appropriate palette based on value of the Background Palette Register. each number assigned corresponds
 * to a color referenced in STM32's L8 color mode.
 * @return Nothing
 */
void prvCheckBGP(){
    uint8_t BGP = ucGBMemoryRead(BGP_ADDR);
    for(int i = 0; i < 4; i++){
        switch ((BGP >> (i*2)) & 0x03) {
            case 0: ucBGPColorToPalette[i] = 1;  break;
            case 1: ucBGPColorToPalette[i] = 2;  break;
            case 2: ucBGPColorToPalette[i] = 3;  break;
            case 3: ucBGPColorToPalette[i] = 4;  break;
            default:                             break;
        }
    }
}


/**
 * @brief OBP0 register color to palette conversion
 * @details Updates a color to its appropriate palette based on value of the OBP0 Register. each number assigned corresponds
 * to a color referenced in STM32's L8 color mode.
 * @return Nothing
 */
void prvCheckOBP0(){
    uint8_t BGP = ucGBMemoryRead(OBP0_ADDR);
    for(int i = 0; i < 4; i++){
        switch ((BGP >> (i*2)) & 0x03) {
            case 0: ucOBP0ColorToPalette[i] = 1; break;
            case 1: ucOBP0ColorToPalette[i] = 2; break;
            case 2: ucOBP0ColorToPalette[i] = 3; break;
            case 3: ucOBP0ColorToPalette[i] = 4; break;
            default:                             break;
        }
    }
}

/**
 * @brief OBP1 register color to palette conversion
 * @details Updates a color to its appropriate palette based on value of the OBP1 Register. each number assigned corresponds
 * to a color referenced in STM32's L8 color mode.
 * @return Nothing
 */
void prvCheckOBP1(){
    uint8_t BGP = ucGBMemoryRead(OBP1_ADDR);
    for(int i = 0; i < 4; i++){
        switch ((BGP >> (i*2)) & 0x03) {
            case 0: ucOBP1ColorToPalette[i] = 1;  break;
            case 1: ucOBP1ColorToPalette[i] = 2;  break;
            case 2: ucOBP1ColorToPalette[i] = 3;  break;
            case 3: ucOBP1ColorToPalette[i] = 4;  break;
            default:                              break;
        }
    }
}

/**
 * @brief Read LCDC Register to find appropriate Background & Tile Data Address
 * @details This function Reads from the LCDC Register and depending on the value of the 4th bit, it will change the Background
 * & Window Tile Data Select Address to 0x8000 on high or to 0x8800 on low.
 * @return Background & Window Tile Data Select Address
 * @note Background & Window Tile Data Select should be 0x9000 on a low 4th bit in a real Gameboy,
 * but it is much simpler to implement this way with emulation, the  0x400 offset is accounted for in getTileLineData()
 */
uint16_t prvGetBackWinTileDataSel(){
    return (ucGBMemoryRead(LCDC_ADDR) & 0x10) ? TILE_DATA_UNSIGNED_ADDR : TILE_DATA_SIGNED_ADDR;
}

/**
 * @brief Read LCDC Register to find appropriate Background Tile Display Select Address
 * @details This function Reads from the LCDC Register and depending on the value of the 3rd bit, it will change the Background Tile
 * Display Address to 0X9C00 on high or to 0x9800 on low.
 * @return Background Tile Display Select Address
 */
uint16_t prvGetBackTileDisplaySel(){
    return (ucGBMemoryRead(LCDC_ADDR) & 0x08) ? TILE_MAP_LOCATION_HIGH : TILE_MAP_LOCATION_LOW;
}

/**
 * @brief Read LCDC Register to find appropriate Window Tile Display Select Address
 * @details This function Reads from the LCDC Register and depending on the value of the 4th bit, it will change the Window Tile
 * Display Address to 0X9C00 on high or to 0x9800 on low.
 * @return Window Tile Display Select Address
 */
uint16_t prvGetWinTileDisplaySel(){
    return (ucGBMemoryRead(LCDC_ADDR) & 0x40) ? TILE_MAP_LOCATION_HIGH : TILE_MAP_LOCATION_LOW;
}

/**
 * @brief Finds specific line data of a tile using the tile_offset and line_offset
 * @details Called by prvGBPPUDrawLine() and returns color information of a specific tile in the Gameboy's VRAM located by using the tile_offset and line_offset parameters.
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
void prvCheckLYC(uint8_t ly){
    if(ly == ucGBMemoryRead(LYC_ADDR)){
        vGBMemorySetBit(STAT_ADDR, 2);
        if(ucCheckBit(ucGBMemoryRead(STAT_ADDR), 6))
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
void prvSetMode(uint8_t mode){
    ucMode = mode;
    switch (mode) {
        case MODE_0: vGBMemoryResetBit(STAT_ADDR, 1); vGBMemoryResetBit(STAT_ADDR, 0); break;		// 00
        case MODE_1: vGBMemoryResetBit(STAT_ADDR, 1);   vGBMemorySetBit(STAT_ADDR, 0); break;		// 01
        case MODE_2:   vGBMemorySetBit(STAT_ADDR, 1); vGBMemoryResetBit(STAT_ADDR, 0); break;		// 10
        case MODE_3:   vGBMemorySetBit(STAT_ADDR, 1);   vGBMemorySetBit(STAT_ADDR, 0); break;		// 11
        default:                                                                       break;
    }
}

/**
 * @brief Updates the frame buffer with pixel information for 1 specified pixel
 * @details Updates and applies a pixel perfect image scaling algorithm on 1 pixel of the frame buffer when called.
 * @param data Color information for the current pixel
 * @param pixelPos X position for the current pixel
 * @returns Nothing
 */
void prvUpdateBuffer(uint8_t data, int pixelPos){
    pixelPos *= ulScaleAmount;
    for (int yStretch = 1; yStretch <= ulScaleAmount; yStretch++){
        for(int xStretch = 0; xStretch < ulScaleAmount; xStretch++){
            ucGBFrame[pixelPos + xStretch + (ulCurLine) + (ulLineAdd * yStretch)] = data;
        }
    }
}

/**
 * @brief Update frame buffer with background information
 * @details Populates the frame buffer with Background information on the line ly
 * @param ly LY Register Value
 * @param SCX X location of the left most position on the background tile map
 * @param SCY Y location of the top most position on the background tile map
 * @param TileDataAddr Start Address of location in VRAM of Background Tiles
 * @param DisplayAddr  Start Address holding the offset in VRAM for each Tile displayed on Screen
 * @returns Nothing
 */
void prvGBPPUDrawLineBackground(uint8_t ly, uint8_t SCX, uint8_t SCY, uint16_t TileDataAddr, uint16_t DisplayAddr){
    uint16_t tile_offset = (((uint8_t)(SCY + ly) / 8) * 32) + (SCX / 8);                        // gives the address offset in the tile map
    uint8_t line_offset = (((SCY % 8) + ly) % 8) * 2;                                           // gives the line offset in the tile
    uint8_t pixl_offset = SCX % 8;                                                              // gives current pixel offset

    uint16_t first_tile = tile_offset % 32;
    uint16_t tile_data = getTileLineData(tile_offset, line_offset, TileDataAddr, DisplayAddr);  // tile data holds tile line information

    for(int j = 0; j < 160; j++){

        uint8_t pixelData = 0;

        switch (((tile_data << pixl_offset) & 0x8080)) {
            case 0x0000: pixelData = ucBGPColorToPalette[0]; ucBGWINline[j] = 0; break;
            case 0x0080: pixelData = ucBGPColorToPalette[1]; ucBGWINline[j] = 1; break;
            case 0x8000: pixelData = ucBGPColorToPalette[2]; ucBGWINline[j] = 2; break;
            case 0x8080: pixelData = ucBGPColorToPalette[3]; ucBGWINline[j] = 3; break;
        }

        prvUpdateBuffer(pixelData, j);
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

/**
 * @brief Update frame buffer with window information
 * @details Populates the frame buffer with window information if it is currently displayed on the frame on the line ly
 * @param ly LY Register Value
 * @param WX Window X position
 * @param WY Window Y position
 * @param TileDataAddr Start Address of location in VRAM of Window Tiles
 * @param DisplayAddr  Start Address holding the offset in VRAM for each Tile displayed on Screen
 * @returns Nothing
 */
void prvGBPPUDrawLineWindow(uint8_t ly, uint8_t WX, uint8_t WY, uint16_t TileDataAddr, uint16_t DisplayAddr){
    if(WY > ly || WY > 143 || WX > 166)
        return;

    uint16_t tile_offset = (((uint8_t)(ly - WY) / 8) * 32);                                     // gives the address offset in the tile map
    uint8_t line_offset = (((ly - WY) % 8)) * 2;                                                // gives the line offset in the tile
    uint8_t pixl_offset = (WX - 7) % 8;                                                         // gives current pixel offset

    uint16_t tile_data = getTileLineData(tile_offset, line_offset, TileDataAddr, DisplayAddr);  // tile data holds tile line information

    for(int j = (WX - 7); j < 160; j++){
        uint8_t pixelData = 0;

        switch (((tile_data << pixl_offset) & 0x8080)) {
            case 0x0000: pixelData = ucBGPColorToPalette[0]; ucBGWINline[j] = 0; break;
            case 0x0080: pixelData = ucBGPColorToPalette[1]; ucBGWINline[j] = 1; break;
            case 0x8000: pixelData = ucBGPColorToPalette[2]; ucBGWINline[j] = 2; break;
            case 0x8080: pixelData = ucBGPColorToPalette[3]; ucBGWINline[j] = 3; break;
        }

        prvUpdateBuffer(pixelData, j);
        pixl_offset++;

        if(pixl_offset == 8){
            tile_offset++;
            pixl_offset = 0;
            tile_data = getTileLineData(tile_offset, line_offset, TileDataAddr, DisplayAddr);
        }
    }
}

/**
 * @brief  Update frame buffer with object information
 * @details Populates the frame buffer with object sprites on line ly
 * @param ly LY Register Value
 * @returns Nothing
 */
void prvGBPPUDrawLineObjects(uint8_t ly){
    for(int obj = 0; obj < 40; obj++){
        int16_t yCoordinate = ucGBMemoryRead(OAM_BASE + (obj*4)) - 16;                      // must be signed for logic to work
        int16_t xCoordinate = ucGBMemoryRead(OAM_BASE + (obj*4) + 1) - 8;                   // "" "" "" same here
        uint8_t dataTile    = ucGBMemoryRead(OAM_BASE + (obj*4) + 2);
        uint8_t objPrio     = ucCheckBit(ucGBMemoryRead(OAM_BASE + (obj*4) + 3), 7);
        uint8_t objYFlip    = ucCheckBit(ucGBMemoryRead(OAM_BASE + (obj*4) + 3), 6);
        uint8_t objXFlip    = ucCheckBit(ucGBMemoryRead(OAM_BASE + (obj*4) + 3), 5);
        uint8_t objPalette  = ucCheckBit(ucGBMemoryRead(OAM_BASE + (obj*4) + 3), 4);
        uint8_t objSize     = ucCheckBit(ucGBMemoryRead(LCDC_ADDR), 2);

        uint8_t objHeight   = (objSize == 0) ? 8 : 16;

        if(yCoordinate <= ly && (yCoordinate + objHeight) > ly){

            uint8_t lineOffset = objYFlip ? ((objHeight - 1) - (ly - yCoordinate)) * 2 : (ly - yCoordinate) * 2;
            uint16_t tile_data = usGBMemoryReadShort(TILE_DATA_UNSIGNED_ADDR + (dataTile * 0x10) + lineOffset);
            uint8_t *palette = (objPalette) ? &ucOBP1ColorToPalette[0] : &ucOBP0ColorToPalette[0];

            for(int pixelNum = 0; pixelNum < 8; pixelNum++){

                uint16_t colorInfo = (objXFlip) ? (((tile_data >> pixelNum) & 0x0101) << 7) : ((tile_data << pixelNum) & 0x8080);
                uint8_t pixelData = 0;

                switch (colorInfo) {
                    case 0x0000: pixelData = 0;          break;
                    case 0x0080: pixelData = palette[1]; break;
                    case 0x8000: pixelData = palette[2]; break;
                    case 0x8080: pixelData = palette[3]; break;
                }

                if(pixelData != 0 && xCoordinate + pixelNum >= 0 && (xCoordinate + pixelNum) < 160){
                    if((objPrio) && ucBGWINline[xCoordinate + pixelNum]){
                        // do nothing this circumstance
                    }else{
                        prvUpdateBuffer(pixelData, xCoordinate + pixelNum);
                    }
                }
            }
        }
    }
}

/**
 * @brief Update data in the frame buffer for 1 line of the Gameboy
 * @details Populates the frame buffer with data related to a line ly
 * @param ly LY Register Value
 * @param SCX Scroll X Register Value
 * @param SCY Scroll Y Register Value
 * @returns Nothing
 */
void prvGBPPUDrawLine(uint8_t ly, uint8_t SCX, uint8_t SCY){

    // update Palettes
    prvCheckBGP();
    prvCheckOBP0();
    prvCheckOBP1();

    uint16_t TileDataAddr = prvGetBackWinTileDataSel();
    ulCurLine = ly * ulScaleAmount * ulScaleAmount * 160;
    ulLineAdd = ulScaleAmount * 160;

    if(ucGBMemoryRead(LCDC_ADDR) & 0x01){
        prvGBPPUDrawLineBackground(ly, SCX, SCY, TileDataAddr, prvGetBackTileDisplaySel());
        if(ucGBMemoryRead(LCDC_ADDR) & 0x20)
            prvGBPPUDrawLineWindow(ly, ucGBMemoryRead(WX_ADDR), ucGBMemoryRead(WY_ADDR), TileDataAddr, prvGetWinTileDisplaySel());
    }else{
        for(int j = 0; j < 160; j++){
            prvUpdateBuffer(1, j);
        }
    }

    if(ucGBMemoryRead(LCDC_ADDR) & 0x02)
        prvGBPPUDrawLineObjects(ly);
}


