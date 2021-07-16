/**
  ******************************************************************************
  * @file           : gbmemory.c
  * @brief          : Gameboy Memory Functionality
  *                   This file Handles all reads and Writes to Locations in memory map.
  * @author         : Rami
  * @date           : Mar 28, 2021
  *
  ******************************************************************************
  *
  ******************************************************************************
  */

#include <string.h>
#include <stdint.h>
#include "gbfunctions.h"
#include "main.h"
#include "gbmemory.h"
#include "gbMBC.h"

registers reg;
memory    mem;
uint8_t   ucCurrentOP;
uint8_t   ucJoypadSELdir;
uint8_t   ucJoypadSELbut;
uint8_t   ucTimerStopStart;
uint8_t   ucClockMode;
uint8_t   ucDataTransFlag = 0;

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;

/*Function Prototypes*/
uint8_t prvGBMemoryJoypad();

/**
 * @brief Set the current opcode for Debug Printing to LCD screen. See vGBMemoryPrint();
 * @param op current opcode.
 * @return Nothing
 */
void vGBMemorySetOP(uint8_t op){
    ucCurrentOP = op;
}

/**
 * @brief Initialize certain Gameboy registers with their correct information.
 * @details At start up the Joypad Register should read 0xCF to denote that no Joypad buttons are being pressed. The IF register should read
 * 			0xE1 to set the appropriate flags.
 * @return Nothing
 */
void vGBMemoryInit(){
    vGBMBCsetControllerType(ucGBMemoryRead(0x147));
	mem.ram[JOY_ADDR] = 0xCF;
	mem.ram[IF_ADDR] = 0xE1;
	vGBMemoryWrite(TAC_ADDR, 0xF8);
}


/**
 * @brief Loads data of amount bytes into Memory maps from mem.ram[0] to mem.ram[bytes - 1].
 * @param data data to be loaded into memory map.
 * @param size amount of bytes.
 * @return Nothing
 */
void vGBMemoryLoad(const void* data, uint32_t bytes){
	memcpy(mem.ram , data, bytes);
}

/**
 * @brief Handles any writes to the Joypad Register 0xFF00
 * @param data data trying to be written to Joypad Register.
 * @return Joypad Keys pressed
 */
uint8_t prvGBMemoryJoypad(){
    uint32_t value = 0;
	uint8_t mask = 0;

	if(ucJoypadSELdir == 0x10){
	    HAL_ADC_Start(&hadc3);
		HAL_ADC_PollForConversion(&hadc3, HAL_MAX_DELAY);
		value = HAL_ADC_GetValue(&hadc3) >> 12;
	}else if(ucJoypadSELbut == 0x20){
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

	return 0xC0 | (0xF^mask) | (ucJoypadSELbut | ucJoypadSELdir);
}

/**
 * @brief Write data to location in memory map specified by address variable
 * @param address memory map address
 * @param data byte of data
 * @returns Nothing
 */
void vGBMemoryWrite(uint16_t address, uint8_t data){

    if(address >= 0xFF00){

	    if(address == JOY_ADDR){
	        ucJoypadSELdir = data & 0x10;
	        ucJoypadSELbut = data & 0x20;
			return;
		}

		else if(address == DMA_ADDR){
		    for(uint16_t i = 0; i < 40*4; i++) vGBMemoryWrite(OAM_BASE + i, ucGBMemoryRead((data << 8) + i));
			return;
		}

		else if(address == DIV_ADDR){
		    mem.ram[DIV_ADDR] = 0;
			return;
		}

		else if(address == TAC_ADDR){
		    ucTimerStopStart = ucCheckBit(data, 2);
		    ucClockMode = (ucCheckBit(data, 1) * 2) + ucCheckBit(data, 0);
			mem.ram[address] = data;
			return;
		}

		else if(address == STC_ADDR){
		    if(ucCheckBit(data, 7)){
		        ucDataTransFlag = 1;
		    }
			mem.ram[address] = data;
			return;
		}
	}

	if((address >= CARTROM_BANK0 && address < VRAM_BASE)){
	    vGBMBCwrite(address, data);
		return;
	}


	if(address >= ECHORAM_BASE && address < OAM_BASE){
	    mem.ram[address - 0x2000] = data;
		return;
	}

	mem.ram[address] = data;
}

/**
 * @brief Write a short value to location in the memory map
 * @param address memory map address
 * @param data 2 bytes of data
 * @returns Nothing
 */
void vGBMemoryWriteShort(uint16_t address, uint16_t data){
    vGBMemoryWrite(address,   data & 0xFF);
	vGBMemoryWrite(address+1, data >> 8);
}
/**
 * @brief Set a bit in location specified by memory map address
 * @param address memory map address
 * @param bit which bit to set (0 - 7)
 * @returns Nothing
 */
void vGBMemorySetBit(uint16_t address, uint8_t bit){
    if(address >= ECHORAM_BASE && address < OAM_BASE)
	    mem.ram[address - 0x2000] |= (0x1 << bit);

	if((address >= CARTROM_BANK0 && address < VRAM_BASE))
	    return;

	mem.ram[address] |= (0x1 << bit);
}

/**
 * @brief Reset a bit in location specified by memory map address
 * @param address memory map address
 * @param bit which bit to reset (0 - 7)
 * @returns Nothing
 */
void vGBMemoryResetBit(uint16_t address, uint8_t bit){

    if(address >= ECHORAM_BASE && address < OAM_BASE)
	    mem.ram[address - 0x2000] &= ~(0x1 << bit);

	if((address >= CARTROM_BANK0 && address < VRAM_BASE))
	    return;

	mem.ram[address] &= ~(0x1 << bit);
}

/**
 * @brief Read from location in memory map
 * @param address memory map address
 * @return data byte of data located in memory map
 */
uint8_t ucGBMemoryRead(uint16_t address){

    if(address >= 0xFF00){
	    if(address == JOY_ADDR){
		    return prvGBMemoryJoypad();
		}

		if(address == SB_ADDR){
		    return 0xFF;
		}
	}

	if((address >= CARTROM_BANK0 && address < VRAM_BASE) && mem.ram[0xFF50] != 0){
	    return ucGBMBCreadBankX(address);
	}

	if(address >= ECHORAM_BASE && address < OAM_BASE)
	    return mem.ram[address - 0x2000];

	return mem.ram[address];
}


/**
 * @brief Read from 2 sequential locations in memory map
 * @param address memory map address
 * @return Short information of data in memory map
 */
uint16_t usGBMemoryReadShort(uint16_t address){
    return usConcat16BitBigEndian(mem.ram[address], mem.ram[address+1]);
}

/**
 * @brief Ticks all timers currently in use
 * @details Ticks the DIV and possibly the TIMA timers at their correct Hertz
 * @param durationMcycle Amount of T states that have passed executing the current instruction.
 * @returns Nothing
 */
void vGBMemoryIncTimers(uint8_t durationMcycle){
    static uint8_t timerDIV  = 0;
    static uint8_t timerTIMA = 0;
    static uint8_t oldTIMA   = 0;
    static uint8_t timerDiv8k = 0;

    if((timerDIV + (durationMcycle << 2)) > 0xFF){
        mem.ram[DIV_ADDR]++;

        if(ucDataTransFlag){
            timerDiv8k++;
            if(timerDiv8k == 0x10){
                timerDiv8k = 0;
                vGBMemoryResetBit(STC_ADDR, 7);
                ucDataTransFlag = 0;
                vGBMemorySetBit(IF_ADDR, 3);
            }
        }
    }

    timerDIV += (durationMcycle << 2);

    if(ucTimerStopStart){
        uint16_t curDuration = 0;

        switch (ucClockMode) {
            case 0x0: curDuration = (durationMcycle << 0);  break;
            case 0x1: curDuration = (durationMcycle << 6);  break;
            case 0x2: curDuration = (durationMcycle << 4);  break;
            case 0x3: curDuration = (durationMcycle << 2);  break;
            default:                                        break;
        }

        if(timerTIMA + curDuration > 0xFF){
            mem.ram[TIMA_ADDR]++;
        }

        if(timerTIMA + curDuration > 0x1FE){
            mem.ram[TIMA_ADDR]++;
        }

        timerTIMA += curDuration;

        if(mem.ram[TIMA_ADDR] < 5 && oldTIMA == 0xFF){
            mem.ram[TIMA_ADDR] = mem.ram[TMA_ADDR];
            vGBMemorySetBit(IF_ADDR, 2);
        }

        oldTIMA = mem.ram[TIMA_ADDR];
    }
}

/**
 * @brief Print out all the Register value information to the Screen for debugging purposes.
 * @returns Nothing
 */
void vGBMemoryPrint(){
    char temp[15];
    sprintf(temp,"Opcode: 0x%.2x", ucCurrentOP);
    UTIL_LCD_DisplayStringAt(500, LINE(2), (uint8_t *) temp, LEFT_MODE);
    sprintf(temp,"AF: 0x%.4x",reg.AF);
    UTIL_LCD_DisplayStringAt(500, LINE(3), (uint8_t *) temp, LEFT_MODE);
    sprintf(temp,"BC: 0x%.4x",reg.BC);
    UTIL_LCD_DisplayStringAt(500, LINE(4), (uint8_t *) temp, LEFT_MODE);
    sprintf(temp,"DE: 0x%.4x",reg.DE);
    UTIL_LCD_DisplayStringAt(500, LINE(5), (uint8_t *) temp, LEFT_MODE);
    sprintf(temp,"HL: 0x%.4x",reg.HL);
    UTIL_LCD_DisplayStringAt(500, LINE(6), (uint8_t *) temp, LEFT_MODE);
    sprintf(temp,"SP: 0x%.4x",reg.SP);
    UTIL_LCD_DisplayStringAt(500, LINE(7), (uint8_t *) temp, LEFT_MODE);
    sprintf(temp,"PC: 0x%.4x",reg.PC);
    UTIL_LCD_DisplayStringAt(500, LINE(8), (uint8_t *) temp, LEFT_MODE);

	sprintf(temp," Z: 0x%.1x",ucCheckBit(reg.F, 7));
	UTIL_LCD_DisplayStringAt(500, LINE(10), (uint8_t *) temp, LEFT_MODE);
	sprintf(temp," N: 0x%.1x",ucCheckBit(reg.F, 6));
	UTIL_LCD_DisplayStringAt(500, LINE(11), (uint8_t *) temp, LEFT_MODE);
	sprintf(temp," H: 0x%.1x",ucCheckBit(reg.F, 5));
	UTIL_LCD_DisplayStringAt(500, LINE(12), (uint8_t *) temp, LEFT_MODE);
	sprintf(temp," C: 0x%.1x",ucCheckBit(reg.F, 4));
	UTIL_LCD_DisplayStringAt(500, LINE(13), (uint8_t *) temp, LEFT_MODE);
}
