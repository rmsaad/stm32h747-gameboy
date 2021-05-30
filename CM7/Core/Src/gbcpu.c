/*
 * gbcpu.c
 *
 *  Created on: Mar 28, 2021
 *      Author: Rami
 */

#include "gbmemory.h"

/* Private variables ---------------------------------------------------------*/
registers reg;

/**
  * @brief  init gameboy on powerup
  * @param  None
  * @retval None
  */
void vGBCPUpowerup(){
	// TEMP until OPCODES implemented
	reg.PC = 0x100;
	reg.SP = 0xFFFE;
}
