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
uint32_t tempState = 0;

void LYC_check(uint8_t ly);
void setMode(uint8_t mode);

void gbPPUStep(){

	if (tStates > 456){
		ly++;

		if(ly > 153){
			setMode(MODE_2);
			ly = 0;
		}

		vGBMemoryWrite(0xFF44, ly);
		tStates -= 456;
		tempState += 456; // debugging purposes

	}

	LYC_check(ly);

	if (ly > 143){
		setMode(MODE_1);
	}else{
		if (tStates <= 80)
			setMode(MODE_2);
		else if(tStates <= 252)
			setMode(MODE_3);
		else if(tStates <= 456)
			setMode(MODE_0);
	}
}

void LYC_check(uint8_t ly){
	if(ly == ucGBMemoryRead(0xFF45)){
		vGBMemorySetBit(0xFF41, 2);
	}else{
		vGBMemoryResetBit(0xFF41, 2);
	}
}

void setMode(uint8_t mode){
	switch (mode) {
		case MODE_0: vGBMemoryResetBit(0xFF41, 1); vGBMemoryResetBit(0xFF41, 0); break;		// 00
		case MODE_1: vGBMemoryResetBit(0xFF41, 1);   vGBMemorySetBit(0xFF41, 0); break;		// 01
		case MODE_2:   vGBMemorySetBit(0xFF41, 1); vGBMemoryResetBit(0xFF41, 0); break;		// 10
		case MODE_3:   vGBMemorySetBit(0xFF41, 1);   vGBMemorySetBit(0xFF41, 0); break;		// 11
		default:                                                                 break;
	}
}
