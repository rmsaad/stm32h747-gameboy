/*
 * audio.c
 *
 *  Created on: Oct 1, 2021
 *      Author: Rami
 */

#include "main.h"
#include <memory.h>

#define AUDIO_BUFFER_SIZE (8192 * 4)

typedef enum {
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF,
  BUFFER_OFFSET_FULL,
}BUFFER_StateTypeDef;

BSP_AUDIO_Init_t AudioInit;

//extern unsigned char audio_sample_bin[];
uint8_t AudioBuffer[AUDIO_BUFFER_SIZE];

volatile BUFFER_StateTypeDef xAudioState = BUFFER_OFFSET_NONE;

void vAudioInit(){
    AudioInit.BitsPerSample = 16;
    AudioInit.ChannelsNbr = 2;
    AudioInit.SampleRate = AUDIO_FREQUENCY_44K;
    AudioInit.Volume = 70;
    AudioInit.Device = AUDIO_OUT_DEVICE_HEADPHONE;
    if(BSP_AUDIO_OUT_Init(0, &AudioInit)  != HAL_OK){
        //Error_Handler();
    }
    memset(AudioBuffer, 0, AUDIO_BUFFER_SIZE);

    //memmove(&AudioBuffer[0],  &audio_sample_bin[2000], AUDIO_BUFFER_SIZE);

    if(BSP_AUDIO_OUT_Play(0, &AudioBuffer[0], AUDIO_BUFFER_SIZE) != HAL_OK){
        Error_Handler();
    }
}


void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t Instance){
    //HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_13);
    xAudioState = BUFFER_OFFSET_HALF;
}

void BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t Instance){
    //HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_14);
    xAudioState = BUFFER_OFFSET_FULL;
}

void vAudioPlayBuffer(uint16_t CH2Buff[], uint16_t *CH2BuffPos){

    while(xAudioState == BUFFER_OFFSET_NONE) {}                  // sync emulation to audio

    if (xAudioState == BUFFER_OFFSET_HALF){
        xAudioState = BUFFER_OFFSET_NONE;
        memcpy(&AudioBuffer[0],  &CH2Buff[0], 8192 * 2);

    }else if(xAudioState == BUFFER_OFFSET_FULL){
        xAudioState = BUFFER_OFFSET_NONE;
        memcpy(&AudioBuffer[AUDIO_BUFFER_SIZE/2],  &CH2Buff[0], 8192 * 2);
    }


}
