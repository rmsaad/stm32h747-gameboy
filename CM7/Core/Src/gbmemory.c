/*
 * gbmemory.c
 *
 *  Created on: Mar 28, 2021
 *      Author: Rami
 */

#include <string.h>
#include <stdint.h>
#include "gbfunctions.h"
#include "main.h"
#include "gbmemory.h"
#include "gbMBC.h"

//#define SRAM2		   0xD2000000UL
// use this to access d2_sram
//memory *mem = (uint32_t*)SRAM2;

registers reg;
memory mem;
uint8_t current_op;
uint8_t joypadSELdir;
uint8_t joypadSELbut;
uint8_t timerStopStart;
uint8_t clockMode;
uint8_t dataTransFlag = 0;
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;

/**
 * @brief Set the current opcode for Debug Printing to LCD screen. See vGBMemoryPrint();
 * @param op current opcode.
 * @return Nothing
 */
void vGBMemorySetOP(uint8_t op){
	current_op = op;
}

/**
 * @brief Initialize certain Gameboy registers with their correct information.
 * @details At start up the Joypad Register should read 0xCF to denote that no Joypad buttons are being pressed. The IF register should read
 * 			0xE1 to set the appropriate flags.
 * @return Nothing
 */
void vGBMemoryInit(){
	gbMBCsetControllerType(ucGBMemoryRead(0x147));
	mem.ram[JOY_ADDR] = 0xCF;
	mem.ram[IF_ADDR] = 0xE1;
	vGBMemoryWrite(TAC_ADDR, 0xF8);
}


/**
 * @brief Loads data of amount bytes into Memory maps from mem.ram[0] to mem.ram[bytes - 1].
 * @param data data to be loaded into memory map.
 * @param size amount of bytes.
 * @return Nothing
 */
void vGBMemoryLoad(const void* data, uint32_t bytes){
	memcpy(mem.ram , data, bytes);
}

/**
 * @brief Handles any writes to the Joypad Register 0xFF00
 * @details
 * @param data data trying to be written to Joypad Register.
 * @return Nothing
 */
uint8_t vGBMemoryJoypad(){
	uint32_t value = 0;
	uint8_t mask = 0;

	if(joypadSELdir == 0x10){
		HAL_ADC_Start(&hadc3);
		HAL_ADC_PollForConversion(&hadc3, HAL_MAX_DELAY);
	    value = HAL_ADC_GetValue(&hadc3) >> 12;
	}else if(joypadSELbut == 0x20){
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
		value = HAL_ADC_GetValue(&hadc1) >> 12;
	}

	if(value == 0xf)
		mask = (0x0);
	else{
		if((value >> 3) & 0x1){
			mask = (0x4);
		}else if((value & 0x6) == 0x6){
			mask = (0x8);
		}else if(value > 0x2){
			mask = (0x2);
		}else{
			mask = (0x1);
		}
	}

	return 0xC0 | (0xF^mask) | (joypadSELbut | joypadSELdir);
}

/**
 * @brief
 * @details
 * @param address
 * @param data
 */
void vGBMemoryWrite(uint16_t address, uint8_t data){

	if(address >= 0xFF00){

		if(address == JOY_ADDR){
			joypadSELdir = data & 0x10;
			joypadSELbut = data & 0x20;
			return;
		}

		else if(address == DMA_ADDR){
			for(uint16_t i = 0; i < 40*4; i++) vGBMemoryWrite(OAM_BASE + i, ucGBMemoryRead((data << 8) + i));
			return;
		}

		else if(address == DIV_ADDR){
			mem.ram[DIV_ADDR] = 0;
			return;
		}

		else if(address == TAC_ADDR){
			timerStopStart = checkbit(data, 2);
			clockMode = (checkbit(data, 1) * 2) + checkbit(data, 0);
			mem.ram[address] = data;
			return;
		}

		else if(address == STC_ADDR){
			if(checkbit(data, 7))
				dataTransFlag = 1;
			mem.ram[address] = data;
			return;
		}

	}

	if((address >= CARTROM_BANK0 && address < VRAM_BASE)){
		gbMBCwrite(address, data);
		return;
	}


	if(address >= ECHORAM_BASE && address < OAM_BASE){
		mem.ram[address - 0x2000] = data;
		return;
	}

	mem.ram[address] = data;
}

void vGBMemoryWriteShort(uint16_t address, uint16_t data){
	vGBMemoryWrite(address,   data & 0xFF);
	vGBMemoryWrite(address+1, data >> 8);
}
/**
 *
 * @param address
 * @param bit
 */
void vGBMemorySetBit(uint16_t address, uint8_t bit){
	if(address >= ECHORAM_BASE && address < OAM_BASE)
		mem.ram[address - 0x2000] |= (0x1 << bit);

	if((address >= CARTROM_BANK0 && address < VRAM_BASE))
		return;

	mem.ram[address] |= (0x1 << bit);
}

/**
 *
 * @param address
 * @param bit
 */
void vGBMemoryResetBit(uint16_t address, uint8_t bit){

	if(address >= ECHORAM_BASE && address < OAM_BASE)
		mem.ram[address - 0x2000] &= ~(0x1 << bit);

	if((address >= CARTROM_BANK0 && address < VRAM_BASE))
		return;

	mem.ram[address] &= ~(0x1 << bit);
}

/**
 *
 * @param address
 * @return
 */
uint8_t ucGBMemoryRead(uint16_t address){

	if(address >= 0xFF00){
		if(address == JOY_ADDR){
			return vGBMemoryJoypad();
		}

		if(address == SB_ADDR){
			return 0xFF;
		}
	}

	if((address >= CARTROM_BANK0 && address < VRAM_BASE) && mem.ram[0xFF50] != 0){
		return gbMBCreadBankX(address);
	}

	if(address >= ECHORAM_BASE && address < OAM_BASE)
		return mem.ram[address - 0x2000];

	return mem.ram[address];
}


/**
 *
 * @param address
 * @return
 */
uint16_t usGBMemoryReadShort(uint16_t address){
	return concat_16bit_bigEndian(mem.ram[address], mem.ram[address+1]);
}


void vGBMemoryIncTimers(uint8_t durationMcycle){
	static uint8_t timerDIV  = 0;
	static uint8_t timerTIMA = 0;
	static uint8_t oldTIMA   = 0;
	static uint8_t timerDiv8k = 0;

	if((timerDIV + (durationMcycle << 2)) > 0xFF){
		mem.ram[DIV_ADDR]++;

		if(dataTransFlag){
			timerDiv8k++;
			if(timerDiv8k == 0x10){
				timerDiv8k = 0;
				vGBMemoryResetBit(STC_ADDR, 7);
				dataTransFlag = 0;
				vGBMemorySetBit(IF_ADDR, 3);
			}
		}
	}

	timerDIV += (durationMcycle << 2);

	if(timerStopStart){
		uint16_t curDuration = 0;

		switch (clockMode) {
			case 0x0: curDuration = (durationMcycle << 0);  break;
			case 0x1: curDuration = (durationMcycle << 6);  break;
			case 0x2: curDuration = (durationMcycle << 4);  break;
			case 0x3: curDuration = (durationMcycle << 2);  break;
			default:  break;
		}

		if(timerTIMA + curDuration > 0xFF){
			mem.ram[TIMA_ADDR]++;
		}

		if(timerTIMA + curDuration > 0x1FE){
			mem.ram[TIMA_ADDR]++;
		}

		timerTIMA += curDuration;


		if(mem.ram[TIMA_ADDR] < 5 && oldTIMA == 0xFF){
			mem.ram[TIMA_ADDR] = mem.ram[TMA_ADDR];
			vGBMemorySetBit(IF_ADDR, 2);
		}

		oldTIMA = mem.ram[TIMA_ADDR];
	}

}

/**
 *
 */
void vGBMemoryPrint(){
	char temp[15];
	sprintf(temp,"Opcode: 0x%.2x", current_op);
	UTIL_LCD_DisplayStringAt(500, LINE(2), (uint8_t *) temp, LEFT_MODE);
	sprintf(temp,"AF: 0x%.4x",reg.AF);
	UTIL_LCD_DisplayStringAt(500, LINE(3), (uint8_t *) temp, LEFT_MODE);
	sprintf(temp,"BC: 0x%.4x",reg.BC);
	UTIL_LCD_DisplayStringAt(500, LINE(4), (uint8_t *) temp, LEFT_MODE);
	sprintf(temp,"DE: 0x%.4x",reg.DE);
	UTIL_LCD_DisplayStringAt(500, LINE(5), (uint8_t *) temp, LEFT_MODE);
	sprintf(temp,"HL: 0x%.4x",reg.HL);
	UTIL_LCD_DisplayStringAt(500, LINE(6), (uint8_t *) temp, LEFT_MODE);
	sprintf(temp,"SP: 0x%.4x",reg.SP);
	UTIL_LCD_DisplayStringAt(500, LINE(7), (uint8_t *) temp, LEFT_MODE);
	sprintf(temp,"PC: 0x%.4x",reg.PC);
	UTIL_LCD_DisplayStringAt(500, LINE(8), (uint8_t *) temp, LEFT_MODE);

	sprintf(temp," Z: 0x%.1x",checkbit(reg.F, 7));
	UTIL_LCD_DisplayStringAt(500, LINE(10), (uint8_t *) temp, LEFT_MODE);
	sprintf(temp," N: 0x%.1x",checkbit(reg.F, 6));
	UTIL_LCD_DisplayStringAt(500, LINE(11), (uint8_t *) temp, LEFT_MODE);
	sprintf(temp," H: 0x%.1x",checkbit(reg.F, 5));
	UTIL_LCD_DisplayStringAt(500, LINE(12), (uint8_t *) temp, LEFT_MODE);
	sprintf(temp," C: 0x%.1x",checkbit(reg.F, 4));
	UTIL_LCD_DisplayStringAt(500, LINE(13), (uint8_t *) temp, LEFT_MODE);
}
