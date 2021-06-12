/*
 * gbppu.c
 *
 *  Created on: Jun 11, 2021
 *      Author: Rami
 */

#include "gbcpu.h"
#include "gbmemory.h"

#define MODE_0 0
#define MODE_1 1
#define MODE_2 2
#define MODE_3 3

extern uint32_t tStates;
uint8_t ly = 0;
uint8_t mode = 0;
uint32_t tempState = 0;

void LYC_check(uint8_t ly);

void gbPPUStep(){

	if (tStates > 456){
		ly++;

		if(ly > 153){
			mode = MODE_2;
			ly = 0;
		}

		vGBMemoryWrite(0xFF44, ly);
		tStates -= 456;
		tempState += 456; // debugging purposes

	}

	LYC_check(ly);

	if (ly > 143){
		mode = MODE_1;
	}else{
		if (tStates <= 80)
			mode = MODE_2;
		else if(tStates <= 252)
			mode = MODE_3;
		else if(tStates <= 456)
			mode = MODE_0;
	}
}

void LYC_check(uint8_t ly){
	if(ly == ucGBMemoryRead(0xFF45)){
		vGBMemorySetBit(0xFF41, 2);
	}else{
		vGBMemoryResetBit(0xFF41, 2);
	}
}
