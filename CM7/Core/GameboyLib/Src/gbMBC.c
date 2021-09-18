/**
  ******************************************************************************
  * @file           : gbMBC.c
  * @brief          : Gameboy MBC Functionality
 *                    This file emulates all functionality of the Gameboy Memory Bank Controller embedded into each game cartridge.
  * @author         : Rami
  * @date           : Jun 11, 2021
  *
  ******************************************************************************
  *
  ******************************************************************************
  */

#include <stdint.h>
#include "gbmemory.h"

uint8_t ucgbMBCcontrollerCode = 0;
uint8_t ucgbMBCramEN = 0;
uint8_t ucgbMBCoffsetLow = 0x01;
uint8_t ucgbMBCoffsetHigh = 0x0;
uint8_t ucgbMBCbankMode = 0x0;

extern const unsigned char* rom;

/**
 * @brief sets the cartridge type for use in this file corresponding to data stored at the memory location 0x147
 * @param code data stored at memory location 0x147
 * @returns Nothing
 */
void vGBMBCsetControllerType(uint8_t code){
	ucgbMBCcontrollerCode = code;
}

/**
 * @brief This function will return data from a ROM location in the memory map depending on the MBC type
 * @param address memory map address
 * @returns data stored at specified ROM address
 */
uint8_t ucGBMBCreadBankX(uint16_t address){
	if(address < CARTROM_BANKX){
		return (uint8_t) rom[address];
	}else if(ucgbMBCcontrollerCode == 0){
		return (uint8_t) rom[address];
	}else{
		return (uint8_t) rom[((ucgbMBCoffsetHigh + ucgbMBCoffsetLow - 1) * 0x4000) + (address)];
	}
}

/**
 * @brief Data is written to MBC register when the address falls in range 0x0000 to 0x7FFF.
 * @details When the range 0x0000 - 0x1FFF is written to with value 0x0A, the Gameboy external RAM is enabled. Any other value written to this range
 * will result in the external RAM being disabled. When the range 0x2000 - 0x3FFF is written to, the lower 5 bits correspond to the switch-able ROM bank
 * that read operation are to be read from. When the range 0x4000 - 0x5FFF is written to, depending on the mode it will either specify the top 2 bits of
 * the ROM bank to be read from or it specify the current RAM bank. When the range falls between 0x6000 - 0x7FFF if the value 0 is written then ROM mode
 * is selected and if the value 1 is written then RAM mode is selected.
 * @param address memory map address
 * @param data byte to be written to MBC register
 * @returns Nothing
 */
void vGBMBCwrite(uint16_t address, uint8_t data){
	if(ucgbMBCcontrollerCode > 0){
		if(address < 0x2000){
			if(data  == 0x0A){
				ucgbMBCramEN = 1;
			}else{
				ucgbMBCramEN = 0;
			}
		}else if(address < 0x4000){
			ucgbMBCoffsetLow = (data & 0x1F);
			if(ucgbMBCoffsetLow == 0){
				ucgbMBCoffsetLow = 1;
			}
		}else if(address < 0x6000){
			if(ucgbMBCbankMode == 0){
				ucgbMBCoffsetHigh = ((data & 0x03) << 5);

			}else{

			}
		}else{
			ucgbMBCbankMode = (data & 0x01);
		}
	}
}
