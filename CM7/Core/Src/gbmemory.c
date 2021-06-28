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

//#define SRAM2		   0xD2000000UL
// use this to access d2_sram
//memory *mem = (uint32_t*)SRAM2;

registers reg;
memory mem;
uint8_t current_op;
uint8_t joypadSELdir;
uint8_t joypadSELbut;
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
	mem.ram[0xFF00] = 0xCF;
	mem.ram[0xFF0F] = 0xE1;
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

	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) != 0){
		if(joypadSELbut == 0x20){
			return 0xC0 | (0xF^0x8) | (joypadSELbut | joypadSELdir);
		}
	}

//	if(joypadSELdir == 0x10){
//		HAL_ADC_Start(&hadc3);
//		HAL_ADC_PollForConversion(&hadc3, HAL_MAX_DELAY);
//	    value = HAL_ADC_GetValue(&hadc3) >> 12;
//	}else if(joypadSELbut == 0x20){
//		HAL_ADC_Start(&hadc1);
//		HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
//		value = HAL_ADC_GetValue(&hadc1) >> 12;
//	}

//	if((value >> 3) & 0x1){
//		mask = (0x4);
//	}else if((value & 0x6) == 0x6){
//		mask = (0x8);
//	}else if(value > 0x2){
//	    mask = (0x2);
//	}else{
//		mask = (0x1);
//	}

	return 0xC0 | (0xF^mask) | (joypadSELbut | joypadSELdir);
}

/**
 * @brief
 * @details
 * @param address
 * @param data
 */
void vGBMemoryWrite(uint16_t address, uint8_t data){
	if(address == JOY_ADDR){
		if((data >> 4) & 0x1)
			joypadSELdir = data & 0x10;
		else if((data >> 4) & 0x2)
			joypadSELbut = data & 0x20;
		return;
	}
	if(((ucGBMemoryRead(STAT_ADDR) & MODE_3)  == MODE_3) && (address >= VRAM_BASE && address < CARTRAM_BASE))
		return;

	if(address >= 0xE000 && address < 0xFE00)
		mem.ram[address - 0x2000] = data;

	if((address >= 0x0000 && address < 0x8000))
		return;
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
	if(((ucGBMemoryRead(STAT_ADDR) & MODE_3)  == MODE_3) && (address >= VRAM_BASE && address < CARTRAM_BASE))
		return;

	if(address >= 0xE000 && address < 0xFE00)
		mem.ram[address - 0x2000] |= (0x1 << bit);

	mem.ram[address] |= (0x1 << bit);
}

/**
 *
 * @param address
 * @param bit
 */
void vGBMemoryResetBit(uint16_t address, uint8_t bit){
	if(((ucGBMemoryRead(STAT_ADDR) & MODE_3)  == MODE_3) && (address >= VRAM_BASE && address < CARTRAM_BASE))
		return;

	if(address >= 0xE000 && address < 0xFE00)
		mem.ram[address - 0x2000] &= ~(0x1 << bit);

	mem.ram[address] &= ~(0x1 << bit);
}

/**
 *
 * @param address
 * @return
 */
uint8_t ucGBMemoryRead(uint16_t address){
	if(address == JOY_ADDR){
		return vGBMemoryJoypad();
	}

	if(address >= 0xE000 && address < 0xFE00)
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
