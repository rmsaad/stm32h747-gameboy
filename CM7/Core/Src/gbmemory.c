/*
 * gbmemory.c
 *
 *  Created on: Mar 28, 2021
 *      Author: Rami
 */

#include <string.h>
#include "gbmemory.h"

registers reg;
memory mem;

// loads data into memory map
void vGBMemoryLoad(const void* data){
	memcpy(mem.ram , data, 256);
}

// reads a location from memory map
uint8_t vGBMemoryRead(uint16_t address){
	return mem.ram[address];
}

