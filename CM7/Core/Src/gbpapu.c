/*
 * gbpapu.c
 *
 *  Created on: Jul 11, 2021
 *      Author: Rami
 */

#include "main.h"
#include "string.h"
#include "gbmemory.h"

#define AUDIO_BUFFER_SIZE 5000


typedef enum {
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF,
  BUFFER_OFFSET_FULL,
}BUFFER_StateTypeDef;

BSP_AUDIO_Init_t AudioInit;
extern unsigned char audio_sample_bin[];
uint8_t AudioBuffer[AUDIO_BUFFER_SIZE];
uint8_t Audindex = 1;
BUFFER_StateTypeDef audstate = BUFFER_OFFSET_NONE;

void gbPAPUstartAudio(){
	AudioInit.BitsPerSample = 16;
	AudioInit.ChannelsNbr = 2;
	AudioInit.SampleRate = AUDIO_FREQUENCY_44K;
	AudioInit.Volume = 50;
	AudioInit.Device = AUDIO_OUT_DEVICE_HEADPHONE;
	BSP_AUDIO_OUT_Init(0, &AudioInit);
	memmove(&AudioBuffer[0],  &audio_sample_bin[10000], AUDIO_BUFFER_SIZE);

	if(BSP_AUDIO_OUT_Play(0, &AudioBuffer[0], AUDIO_BUFFER_SIZE) != HAL_OK){
		Error_Handler();
	}
}

void gbPAPUStep(){
	if(audstate == BUFFER_OFFSET_HALF){
		audstate = BUFFER_OFFSET_NONE;
		memmove(&AudioBuffer[0], &audio_sample_bin[Audindex * AUDIO_BUFFER_SIZE], AUDIO_BUFFER_SIZE/2);

	}else if(audstate == BUFFER_OFFSET_FULL){
		audstate = BUFFER_OFFSET_NONE;
		memmove(&AudioBuffer[AUDIO_BUFFER_SIZE/2], &audio_sample_bin[(Audindex * AUDIO_BUFFER_SIZE) + AUDIO_BUFFER_SIZE/2], AUDIO_BUFFER_SIZE/2);
		Audindex++;
		if(Audindex == 50){
			Audindex = 1;
		}
	}
}

void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t Instance){
	HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_13);
	audstate = BUFFER_OFFSET_HALF;
}

void BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t Instance){
	HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_14);
	audstate = BUFFER_OFFSET_FULL;
}


