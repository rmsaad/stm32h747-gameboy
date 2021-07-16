/*
 * gbMBC.h
 *
 *  Created on: Jul 8, 2021
 *      Author: Rami
 */

#ifndef INC_GBMBC_H_
#define INC_GBMBC_H_


void vGBMBCsetControllerType(uint8_t code);
uint8_t ucGBMBCreadBankX(uint16_t address);
void vGBMBCwrite(uint16_t address, uint8_t data);

#endif /* INC_GBMBC_H_ */
