/**
  ******************************************************************************
  * @file           : gbpapu.c
  * @brief          : Gameboy APU Functionality
  *                   This file emulates all functionality of the Gameboy APU.
  * @author         : Rami
  * @date           : Jul 11, 2021
  *
  ******************************************************************************
  *
  ******************************************************************************
  */

#include "main.h"
#include <string.h>
#include "gbmemory.h"
#include "gbcpu.h"

#define AUDIO_BUFFER_SIZE 10000

typedef enum {
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF,
  BUFFER_OFFSET_FULL,
}BUFFER_StateTypeDef;

BSP_AUDIO_Init_t AudioInit;

extern unsigned char audio_sample_bin[];
uint8_t AudioBuffer[AUDIO_BUFFER_SIZE];
volatile BUFFER_StateTypeDef audstate = BUFFER_OFFSET_NONE;

void gbPAPUstartAudio(){
	AudioInit.BitsPerSample = 16;
	AudioInit.ChannelsNbr = 1;
	AudioInit.SampleRate = AUDIO_FREQUENCY_8K;
	AudioInit.Volume = 70;
	AudioInit.Device = AUDIO_OUT_DEVICE_HEADPHONE;
	if(BSP_AUDIO_OUT_Init(0, &AudioInit)  != HAL_OK){
	    //Error_Handler();
	}
	memset(AudioBuffer, 0, AUDIO_BUFFER_SIZE);

	memmove(&AudioBuffer[0],  &audio_sample_bin[2000], AUDIO_BUFFER_SIZE);

	if(BSP_AUDIO_OUT_Play(0, &AudioBuffer[0], AUDIO_BUFFER_SIZE) != HAL_OK){
		Error_Handler();
	}
}

//void gbPAPUStep(){
//	if(audstate == BUFFER_OFFSET_HALF){
//		//memcpy(&AudioBuffer[0], &audio_sample_bin[0], AUDIO_BUFFER_SIZE/2);
//		audstate = BUFFER_OFFSET_NONE;
//	}else if(audstate == BUFFER_OFFSET_FULL){
//		//memcpy(&AudioBuffer[AUDIO_BUFFER_SIZE/2], &audio_sample_bin[50000], AUDIO_BUFFER_SIZE/2);
//		audstate = BUFFER_OFFSET_NONE;
//	}
//}

void gbPAPUStepCH2(uint8_t cycles){

}

//void gbPAPUStep(){
//    gbPAPUStepCH2(ucGetTstate());
//}

void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t Instance){
	//HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_13);
	audstate = BUFFER_OFFSET_HALF;
}

void BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t Instance){
	//HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_14);
	audstate = BUFFER_OFFSET_FULL;
}


