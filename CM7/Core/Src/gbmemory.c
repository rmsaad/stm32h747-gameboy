/*
 * gbmemory.c
 *
 *  Created on: Mar 28, 2021
 *      Author: Rami
 */

#include <string.h>
#include "main.h"
#include "gbmemory.h"

registers reg;
memory mem;
uint8_t current_op;

void vGBMemorySetOP(uint8_t op){
	current_op = op;
}

// loads data into memory map
void vGBMemoryLoad(const void* data){
	memcpy(mem.ram , data, 256);
}

void vGBMemoryWrite(uint16_t address, uint8_t data){
	mem.ram[address] = data;
}

// reads a location from memory map
uint8_t ucGBMemoryRead(uint16_t address){
	return mem.ram[address];
}

void vGBMemoryPrint(){
	char temp[15];
	sprintf(temp,"Opcode: 0x%.2x", current_op);
	UTIL_LCD_DisplayStringAt(500, LINE(6), (uint8_t *) temp, LEFT_MODE);
	sprintf(temp,"AF: 0x%.4x",reg.AF);
	UTIL_LCD_DisplayStringAt(500, LINE(7), (uint8_t *) temp, LEFT_MODE);
	sprintf(temp,"BC: 0x%.4x",reg.BC);
	UTIL_LCD_DisplayStringAt(500, LINE(8), (uint8_t *) temp, LEFT_MODE);
	sprintf(temp,"DE: 0x%.4x",reg.DE);
	UTIL_LCD_DisplayStringAt(500, LINE(9), (uint8_t *) temp, LEFT_MODE);
	sprintf(temp,"HL: 0x%.4x",reg.HL);
	UTIL_LCD_DisplayStringAt(500, LINE(10), (uint8_t *) temp, LEFT_MODE);
	sprintf(temp,"SP: 0x%.4x",reg.SP);
	UTIL_LCD_DisplayStringAt(500, LINE(11), (uint8_t *) temp, LEFT_MODE);
	sprintf(temp,"PC: 0x%.4x",reg.PC);
	UTIL_LCD_DisplayStringAt(500, LINE(12), (uint8_t *) temp, LEFT_MODE);
}
