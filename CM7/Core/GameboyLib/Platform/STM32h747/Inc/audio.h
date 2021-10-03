/*
 * audio.h
 *
 *  Created on: Oct 1, 2021
 *      Author: Rami
 */

#ifndef GAMEBOYLIB_PLATFORM_STM32H747_INC_AUDIO_H_
#define GAMEBOYLIB_PLATFORM_STM32H747_INC_AUDIO_H_

#include <stdint.h>

void vAudioInit();
void vAudioPlayBuffer(uint16_t CH2Buff[], uint16_t *CH2BuffPos);

#endif /* GAMEBOYLIB_PLATFORM_STM32H747_INC_AUDIO_H_ */
