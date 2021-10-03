/*
 * gbpapu.h
 *
 *  Created on: Jul 11, 2021
 *      Author: Rami
 */

#ifndef INC_GBPAPU_H_
#define INC_GBPAPU_H_


void vGBPAPUStep();
void vGBPAPUTriggerCH1(uint8_t data);
void vGBPAPUTriggerCH2(uint8_t data);
void vGBPAPUTriggerCH3(uint8_t data);
void vGBPAPUTriggerCH4(uint8_t data);

#endif /* INC_GBPAPU_H_ */
