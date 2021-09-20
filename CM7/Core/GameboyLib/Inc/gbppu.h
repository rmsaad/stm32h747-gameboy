/*
 * gbppu.h
 *
 *  Created on: Jun 11, 2021
 *      Author: Rami
 */

#ifndef INC_GBPPU_H_
#define INC_GBPPU_H_

void vGBPPUSetDisplayLineBufferFunctionPtr(void (*displayLineBufferFunctionPtr)(uint8_t* , uint8_t , uint8_t));
void vGBPPUStep();
void vSetLineBuffer();

#endif /* INC_GBPPU_H_ */
