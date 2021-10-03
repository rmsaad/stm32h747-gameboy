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

#include "audio.h"
#include <string.h>
#include "gbmemory.h"
#include "gbcpu.h"

uint8_t duties[4][8] = {
    {0,0,0,0,0,0,0,1},          //  00 (0x0)
    {1,0,0,0,0,0,0,1},          //  01 (0x1)
    {1,0,0,0,0,1,1,1},          //  10 (0x2)
    {0,1,1,1,1,1,1,0}           //  11 (0x3)
};

// Audio buffers
uint16_t CH2Buff[1024];

uint16_t CH2BuffPos = 0;

// Channel Enables
uint8_t ucCH1Enable = 0;
uint8_t ucCH2Enable = 0;
uint8_t ucCH3Enable = 0;
uint8_t ucCH4Enable = 0;

// Volume Envelope Enables
uint8_t ucCH1EnvelopeEnable = 0;        // This might refer to sweeps only
uint8_t ucCH2EnvelopeEnable = 0;
uint8_t ucCH3EnvelopeEnable = 0;
uint8_t ucCH4EnvelopeEnable = 0;

// Length Counters
uint8_t ucCH1LengthCounter = 0;
uint8_t ucCH2LengthCounter = 0;
uint8_t ucCH3LengthCounter = 0;
uint8_t ucCH4LengthCounter = 0;

// frequency timers
uint32_t ulCH1Timer = 0;
uint32_t ulCH2Timer = 0;
uint32_t ulCH3Timer = 0;
uint32_t ulCH4Timer = 0;

// Volume Envelopes
uint8_t ucCH1Envelope = 0;
uint8_t ucCH2Envelope = 0;
uint8_t ucCH3Envelope = 0;
uint8_t ucCH4Envelope = 0;

// Inital Volume of Envelope
uint16_t sCH1InitVol = 0;
uint16_t sCH2InitVol = 0; //~~
uint16_t sCH3InitVol = 0;
uint16_t sCH4InitVol = 0;

// 4.194 MHZ --> 44100 HZ Conversion (4.194 / 0.0441 = 95)
uint8_t ucCH1AudioFreqConvertFactor = 95;
uint8_t ucCH2AudioFreqConvertFactor = 95;
uint8_t ucCH3AudioFreqConvertFactor = 95;
uint8_t ucCH4AudioFreqConvertFactor = 95;

// Frame Sequence Step 0 - 7 (Incremented every 8192 T States)
uint8_t ucCH1FrameSequenceStep = 0;
uint8_t ucCH2FrameSequenceStep = 0;
uint8_t ucCH3FrameSequenceStep = 0;
uint8_t ucCH4FrameSequenceStep = 0;

// Frame Sequence TState (Incremented every T State up to 8192)
uint16_t usCH1FrameSequenceTState = 0;
uint16_t usCH2FrameSequenceTState = 0;
uint16_t usCH3FrameSequenceTState = 0;
uint16_t usCH4FrameSequenceTState = 0;

// CH1 & CH2 duty position (0 - 7)
uint8_t ucCH1DutyPos = 0;
uint8_t ucCH2DutyPos = 0;

void prvGBPAPUStepCH2(uint8_t TStates) {
    while (TStates--) {

        if (ulCH2Timer <= 0x00) {
            uint16_t freq_x = ((ucGBMemoryRead(0xFF19) & 0x07) << 8 | ucGBMemoryRead(0xFF18));              // reload freq timer when it hits zero
            ulCH2Timer = (2048 - freq_x) * 4;                                                               // commonly used formula amoung emulators

            //
            //ucCH2DutyPos = (ucCH2DutyPos == 7) ? 0 : ucCH2DutyPos++;                                      // increment duty pos
            ucCH2DutyPos++;
            ucCH2DutyPos %= 8;
        }
        else {
            ulCH2Timer--;
        }

        usCH2FrameSequenceTState++;
        if (usCH2FrameSequenceTState == 8192) {
            usCH2FrameSequenceTState = 0;                                                                   // Set Frame Sequence Tstates back to zero
            //ucCH2FrameSequenceStep = (ucCH2FrameSequenceStep == 7) ? 0 : ucCH2FrameSequenceStep++;            // update Frame Sequence Step
            ucCH2FrameSequenceStep++;
            ucCH2FrameSequenceStep %= 8;

            // length
            if (ucCH2FrameSequenceStep % 2 == 0 && ((ucGBMemoryRead(0xFF19) >> 6) & 0x01) && ucCH2LengthCounter) {
                ucCH2LengthCounter--;
                if (ucCH2LengthCounter <= 0) {
                    ucCH2Enable = 0;
                }
            }

            // envelope
            if (ucCH2FrameSequenceStep == 7 && ucCH2EnvelopeEnable && ucGBMemoryRead(0xFF17) & 0x07) {
                ucCH2Envelope--;

                if (ucCH2Envelope <= 0) {
                    ucCH2Envelope = ucGBMemoryRead(0xFF17) & 0x07;

                    // dont understand here!!
                    int8_t vol = sCH2InitVol + (((ucGBMemoryRead(0xFF17) >> 3) & 0x01) ? 1 : -1);

                    if (vol >= 0 && vol <= 15) {
                        sCH2InitVol = vol;
                    }
                    else {
                        ucCH2EnvelopeEnable = 0;
                    }

                }
            }
        }

        if (!--ucCH2AudioFreqConvertFactor) {
            ucCH2AudioFreqConvertFactor = 95;

                if (ucCH2Enable && (ucGBMemoryRead(0xFF25) & 0x22) && (ucGBMemoryRead(0xFF26) & 0x02)) {

                    int duty = ucGBMemoryRead(0xFF16) >> 6;
                    CH2Buff[CH2BuffPos++] = ((duties[duty][ucCH2DutyPos] == 1) ? (sCH2InitVol << 10) : 0);          // Left  Channel
                    CH2Buff[CH2BuffPos++] = ((duties[duty][ucCH2DutyPos] == 1) ? (sCH2InitVol << 10) : 0);          // Right Channel
                }
                else {
                    CH2Buff[CH2BuffPos++] = 0;
                    CH2Buff[CH2BuffPos++] = 0;
                }

        }

    }
}

void vGBPAPUStep(){
    uint8_t currentTstaes = ucGetTstate();

    prvGBPAPUStepCH2(currentTstaes);

    if (CH2BuffPos >= 128){
        vAudioPlayBuffer(CH2Buff, &CH2BuffPos);
    }


//	if(audstate == BUFFER_OFFSET_HALF){
//		//memcpy(&AudioBuffer[0], &audio_sample_bin[0], AUDIO_BUFFER_SIZE/2);
//		audstate = BUFFER_OFFSET_NONE;
//	}else if(audstate == BUFFER_OFFSET_FULL){
//		//memcpy(&AudioBuffer[AUDIO_BUFFER_SIZE/2], &audio_sample_bin[50000], AUDIO_BUFFER_SIZE/2);
//		audstate = BUFFER_OFFSET_NONE;
//	}
}

// TRIGGER EVENTS
void vGBPAPUTriggerCH1(uint8_t data) {
    ucCH1Enable = 1;                                                                        // Enable Channel

    if (ucCH1LengthCounter == 0)                                                            // Set Length Channel to 64 - t1(data)
        ucCH1LengthCounter = 64 - data;

    uint16_t freq_x = ((ucGBMemoryRead(0xFF14) & 0x07) << 8 | ucGBMemoryRead(0xFF13));      // reload freq timer
    ulCH1Timer = (2048 - freq_x) * 4;                                                       // commonly used formula amoung emulators

}

void vGBPAPUTriggerCH2(uint8_t data) {
    ucCH2Enable = 1;                                                                        // Enable Channel
    vGBMemoryWrite(0xFF26, 0x82);
    if (ucCH2LengthCounter == 0)                                                            // Set length Channel to 64 - t1(data)
        ucCH2LengthCounter = 64 - data;

    uint16_t freq_x = ((ucGBMemoryRead(0xFF19) & 0x07) << 8 | ucGBMemoryRead(0xFF18));      // reload freq timer
    ulCH2Timer = (2048 - freq_x) * 4;                                                       // commonly used formula amoung emulators

    ucCH2Envelope = ucGBMemoryRead(0xFF17) & 0x07;                                          // reload volume envelope
    ucCH2EnvelopeEnable = 1;

    sCH2InitVol = ucGBMemoryRead(0xFF17) >> 4;

    if (ucGBMemoryRead(0xFF17) >> 3 == 0x00) {
        ucCH2Enable = 0;
    }


}

void vGBPAPUTriggerCH3(uint8_t data) {
    ucCH3Enable = 1;

    if (ucCH3LengthCounter == 0)
        ucCH3LengthCounter = 256 - data;
}

void vGBPAPUTriggerCH4(uint8_t data) {
    ucCH4Enable = 1;

    if (ucCH4LengthCounter == 0)
        ucCH4LengthCounter = 64 - data;
}


