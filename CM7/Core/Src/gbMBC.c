/*
 * gbMBC.c
 *
 *  Created on: Jul 8, 2021
 *      Author: Rami
 */

#include <stdint.h>
#include "gbmemory.h"

#define CARTROM_BANK0 0x0000
#define CARTROM_BANKX 0x4000
#define VRAM_BASE     0x8000

uint8_t ucgbMBCcontrollerCode = 0;
uint8_t ucgbMBCramEN = 0;
uint8_t ucgbMBCoffsetLow = 0x01;
uint8_t ucgbMBCoffsetHigh = 0x0;
uint8_t ucgbMBCbankMode = 0x0;

extern const unsigned char* rom;

void gbMBCsetControllerType(uint8_t code){
	ucgbMBCcontrollerCode = code;
}

uint8_t gbMBCreadBankX(uint16_t address){
	if(address < CARTROM_BANKX){
		return (uint8_t) rom[address];
	}else if(ucgbMBCcontrollerCode == 0){
		return (uint8_t) rom[address];
	}else{
		return (uint8_t) rom[((ucgbMBCoffsetHigh + ucgbMBCoffsetLow - 1) * 0x4000) + (address)];
	}
}

void gbMBCwrite(uint16_t address, uint8_t data){
	if(ucgbMBCcontrollerCode > 0){
		if(address < 0x2000){
			if((data & 0x0A) == 0x0A){
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
