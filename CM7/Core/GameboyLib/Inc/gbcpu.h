/*
 * gbcpu.h
 *
 *  Created on: Mar 28, 2021
 *      Author: Rami
 */

#ifndef INC_GBCPU_H_
#define INC_GBCPU_H_

#include <stdio.h>
#include <stdint.h>

#define Z_FLAG 7
#define N_FLAG 6
#define H_FLAG 5
#define C_FLAG 4

typedef struct{
	void* instr;
	uint8_t bytes;
	uint8_t Tstate;
}Assembly_Instr;

/* Public function prototypes -----------------------------------------------*/

void vGBCPUStep();
uint8_t ucGetTstate();

#endif /* INC_GBCPU_H_ */
