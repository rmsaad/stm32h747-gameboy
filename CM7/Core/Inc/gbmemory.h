/*
 * gbmemory.h
 *
 *  Created on: Mar 28, 2021
 *      Author: Rami
 */

#ifndef INC_GBMEMORY_H_
#define INC_GBMEMORY_H_

#include <stdio.h>

typedef struct{
	union{
	  struct{
		uint8_t F;
		uint8_t A;
	  };
	  uint16_t AF;
	};

	union{
	  struct{
		uint8_t C;
		uint8_t B;
	  };
	  uint16_t BC;
	};

	union{
	  struct{
		uint8_t E;
		uint8_t D;
	  };
	  uint16_t DE;
	};

	union{
	  struct{
		uint8_t L;
		uint8_t H;
	  };
	  uint16_t HL;
	};

	uint16_t SP;

	uint16_t PC;
}registers;

typedef struct{
	uint8_t ram[65536];
}memory;

/* Public function prototypes -----------------------------------------------*/

void vGBMemoryLoad(const void* data);
void vGBMemoryWrite(uint16_t address, uint8_t data);
uint8_t ucGBMemoryRead(uint16_t address);
void vGBMemoryPrint();
void vGBMemorySetOP(uint8_t op);

#endif /* INC_GBMEMORY_H_ */

