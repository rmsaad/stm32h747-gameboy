/*
 * gbcpu.h
 *
 *  Created on: Mar 28, 2021
 *      Author: Rami
 */

#ifndef INC_GBCPU_H_
#define INC_GBCPU_H_

#include <stdio.h>

typedef struct{
	void* instr;
	uint8_t bytes;
	uint8_t Tstate;
}Assembly_Instr;

/* Public function prototypes -----------------------------------------------*/

void vGBCPUreset();
void vGBCPUboot();
void vGBCPUinstr(uint8_t opcode);

#endif /* INC_GBCPU_H_ */
