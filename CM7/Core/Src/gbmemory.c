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

registers reg;
memory mem;
uint8_t current_op;

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;

void vGBMemorySetOP(uint8_t op){
	current_op = op;
}

void vGBMemoryInit(){
	mem.ram[0xFF00] = 0xCF;
	mem.ram[0xFF0F] = 0xE1;
}

// loads data into memory map
void vGBMemoryLoad(const void* data, uint32_t size){
	memcpy(mem.ram , data, size);
}

void vGBMemoryJoypad(uint8_t data){
	uint32_t value;

	if((data >> 4) & 0x1){
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	    value = HAL_ADC_GetValue(&hadc1) >> 12;
	}else if((data >> 4) & 0x2){
		HAL_ADC_Start(&hadc3);
		HAL_ADC_PollForConversion(&hadc3, HAL_MAX_DELAY);
		value = HAL_ADC_GetValue(&hadc3) >> 12;
	}else{
		return;
	}

	if(value == 0xf){
		return;
	}

	if((value >> 3) & 0x1){
		mem.ram[JOY_ADDR] &= ~(0x4);
	}else if((value & 0x6) == 0x6){
		mem.ram[JOY_ADDR] &= ~(0x8);
	}else if(value > 0x2){
	    mem.ram[JOY_ADDR] &= ~(0x2);
	}else{
		mem.ram[JOY_ADDR] &= ~(0x1);
	}
}

void vGBMemoryWrite(uint16_t address, uint8_t data){
	if(address == JOY_ADDR){
		vGBMemoryJoypad(data);
		return;
	}
	if(((ucGBMemoryRead(STAT_ADDR) & MODE_3)  == MODE_3) && (address >= VRAM_BASE && address < CARTRAM_BASE))
		return;
	mem.ram[address] = data;
}

void vGBMemorySetBit(uint16_t address, uint8_t bit){
	if(((ucGBMemoryRead(STAT_ADDR) & MODE_3)  == MODE_3) && (address >= VRAM_BASE && address < CARTRAM_BASE))
		return;
	mem.ram[address] |= (0x1 << bit);
}

void vGBMemoryResetBit(uint16_t address, uint8_t bit){
	if(((ucGBMemoryRead(STAT_ADDR) & MODE_3)  == MODE_3) && (address >= VRAM_BASE && address < CARTRAM_BASE))
		return;
	mem.ram[address] &= ~(0x1 << bit);
}

// reads a location from memory map
uint8_t ucGBMemoryRead(uint16_t address){
	return mem.ram[address];
}

uint16_t usGBMemoryReadShort(uint16_t address){
	return *((uint16_t*) &mem.ram[address]);
}

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
