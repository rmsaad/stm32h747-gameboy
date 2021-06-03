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

void vGBMemoryLoad(const void* data){
	memcpy(mem.ram , data, 256);
}

uint8_t vGBMemoryRead(uint16_t address){
	return mem.ram[address];
}

