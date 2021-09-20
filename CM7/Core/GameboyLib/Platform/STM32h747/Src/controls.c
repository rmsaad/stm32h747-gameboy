/**
  ******************************************************************************
  * @file           : controls.c
  * @brief          : Gameboy Joypad Functionality
 *                    This file handles all ADC input polling to detect button presses of the Gameboy joypad
 *                    on the STM32h747 development board and returns this information in the correct format
 *                    such that it can be stored in the Gameboy Joypad register.
  * @author         : Rami
  * @date           : Sept 20, 2021
  *
  ******************************************************************************
  *
  ******************************************************************************
  */

#include <stdint.h>
#include "stm32h7xx_hal.h"
#include "gbmemory.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;

/*Function Prototypes*/
uint8_t prvControlsJoypad(uint8_t *ucJoypadSELdir, uint8_t *ucJoypadSELbut);


/**
 * @brief Allows gbmemory.c access to function prvControlsJoypad() without needing to include controls.h
 * @return None
 * @attention must be called in main before CPU/PPU stepping
 */
void vControlsInit(){
    vGBMemorySetControlFunctionPtr(prvControlsJoypad);
}

/**
 * @brief Handles any writes to the Joypad Register 0xFF00
 * @param data data trying to be written to Joypad Register.
 * @return Joypad Keys pressed
 */
uint8_t prvControlsJoypad(uint8_t *ucJoypadSELdir, uint8_t *ucJoypadSELbut){
    uint32_t value = 0;
    uint8_t mask = 0;

    if(*ucJoypadSELdir == 0x10){
        HAL_ADC_Start(&hadc3);
        HAL_ADC_PollForConversion(&hadc3, HAL_MAX_DELAY);
        value = HAL_ADC_GetValue(&hadc3) >> 12;
    }else if(*ucJoypadSELbut == 0x20){
        HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
        value = HAL_ADC_GetValue(&hadc1) >> 12;
    }

    if(value == 0xf)
        mask = (0x0);
    else{
        if((value >> 3) & 0x1){
            mask = (0x4);
        }else if((value & 0x6) == 0x6){
            mask = (0x8);
        }else if(value > 0x2){
            mask = (0x2);
        }else{
            mask = (0x1);
        }
    }

    return 0xC0 | (0xF^mask) | (*ucJoypadSELbut | *ucJoypadSELdir);
}
