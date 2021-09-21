/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   gbmemoryTest.c
 * Author: Rami
 *
 * Created on September 20, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

//#include "unity.h"
#include "unity_fixture.h"

// need access to static functions for testing 
#include "gbmemory.c"
#include "gbmemory.h"

TEST_GROUP(vGBMemoryWrite);

TEST_SETUP(vGBMemoryWrite)
{
	memset(mem.ram, 0, 0xFFFF);
}

TEST_TEAR_DOWN(vGBMemoryWrite)
{
}

TEST(vGBMemoryWrite, NotWritableROM)
{
	vGBMemoryWrite(CARTROM_BANK0, 0xFF);
	vGBMemoryWrite(CARTROM_BANKX, 0xFF);
	vGBMemoryWrite(VRAM_BASE - 1, 0xFF);
	vGBMemoryWrite(VRAM_BASE    , 0xFF);
	TEST_ASSERT_EQUAL_UINT8(0x00, mem.ram[CARTROM_BANK0]);
	TEST_ASSERT_EQUAL_UINT8(0x00, mem.ram[CARTROM_BANKX]);
	TEST_ASSERT_EQUAL_UINT8(0x00, mem.ram[VRAM_BASE - 1]);
	TEST_ASSERT_EQUAL_UINT8(0xFF, mem.ram[VRAM_BASE]);
}

TEST(vGBMemoryWrite, NotWritableEchoRAM)
{
	vGBMemoryWrite(ECHORAM_BASE, 0xFF);
	vGBMemoryWrite(OAM_BASE - 1, 0xFF);
	vGBMemoryWrite(OAM_BASE, 0xFF);
	TEST_ASSERT_EQUAL_UINT8(0x00, mem.ram[ECHORAM_BASE]);
	TEST_ASSERT_EQUAL_UINT8(0x00, mem.ram[OAM_BASE - 1]);
	TEST_ASSERT_EQUAL_UINT8(0xFF, mem.ram[ECHORAM_BASE - 0x2000]);
	TEST_ASSERT_EQUAL_UINT8(0xFF, mem.ram[OAM_BASE - 1 - 0x2000]);
	TEST_ASSERT_EQUAL_UINT8(0xFF, mem.ram[OAM_BASE]);
}

TEST(vGBMemoryWrite, JoyPadSelectHigh)
{
	ucJoypadSELdir = 0x00;
	ucJoypadSELbut = 0x00;
	vGBMemoryWrite(JOY_ADDR, 0xFF);
	TEST_ASSERT_EQUAL_UINT8(0x10, ucJoypadSELdir);
	TEST_ASSERT_EQUAL_UINT8(0x20, ucJoypadSELbut);
}

TEST(vGBMemoryWrite, JoyPadSelectLow)
{
	ucJoypadSELdir = 0xFF;
	ucJoypadSELbut = 0xFF;
	vGBMemoryWrite(JOY_ADDR, 0x00);
	TEST_ASSERT_EQUAL_UINT8(0x00, ucJoypadSELdir);
	TEST_ASSERT_EQUAL_UINT8(0x00, ucJoypadSELbut);
}

TEST(vGBMemoryWrite, GameboyDMATransfer)
{
	uint8_t testArr[40];
	
	// data to be transfered
	memset(&mem.ram[VRAM_BASE], 0xFF, 40);
	
	// array of correct result
	memset(&testArr[0], 0xFF, 40);
	
	vGBMemoryWrite(DMA_ADDR, (uint8_t)(VRAM_BASE >> 8));
	TEST_ASSERT_EQUAL_UINT8_ARRAY(&testArr[0], &mem.ram[OAM_BASE], 40);
}

TEST(vGBMemoryWrite, TimerDiv)
{
	mem.ram[DIV_ADDR] = 0xFF;
	vGBMemoryWrite(DIV_ADDR, 0xFF);
	TEST_ASSERT_EQUAL_UINT8(0x00, mem.ram[DIV_ADDR]);
}

TEST(vGBMemoryWrite, TimerControl)
{
	ucTimerStopStart = 0x00;
	ucClockMode = 0x00;
	vGBMemoryWrite(TAC_ADDR, 0xFF);
	
	TEST_ASSERT_EQUAL_UINT8(0x01, ucTimerStopStart);
	TEST_ASSERT_EQUAL_UINT8(0x03, ucClockMode);
	TEST_ASSERT_BITS(0x07, 0x07, mem.ram[TAC_ADDR]);
}

TEST(vGBMemoryWrite, TimerCompleteHigh)
{
	ucDataTransFlag = 0x00;
	vGBMemoryWrite(STC_ADDR, 0xFF);
	TEST_ASSERT_EQUAL_UINT8(0x01, ucDataTransFlag);
	TEST_ASSERT_EQUAL_UINT8(0xFF, mem.ram[STC_ADDR]);
}

TEST(vGBMemoryWrite, TimerCompleteLow)
{
	ucDataTransFlag = 0x00;
	vGBMemoryWrite(STC_ADDR, 0x00);
	TEST_ASSERT_EQUAL_UINT8(0x00, ucDataTransFlag);
	TEST_ASSERT_EQUAL_UINT8(0x00, mem.ram[STC_ADDR]);
}

TEST(vGBMemoryWrite, DisableBootRom)
{
	ucBootRomEn = 0x01;
	vGBMemoryWrite(0xFF50, 0x01);
	TEST_ASSERT_EQUAL_UINT8(0x00, ucBootRomEn);
	
	ucBootRomEn = 0x01;
	vGBMemoryWrite(0xFF50, 0x00);
	TEST_ASSERT_EQUAL_UINT8(0x01, ucBootRomEn);
}

TEST_GROUP_RUNNER(vGBMemoryWrite) {
	RUN_TEST_CASE(vGBMemoryWrite, NotWritableROM);
	RUN_TEST_CASE(vGBMemoryWrite, NotWritableEchoRAM);
	RUN_TEST_CASE(vGBMemoryWrite, JoyPadSelectHigh);
	RUN_TEST_CASE(vGBMemoryWrite, JoyPadSelectLow);
	RUN_TEST_CASE(vGBMemoryWrite, GameboyDMATransfer);
	RUN_TEST_CASE(vGBMemoryWrite, TimerDiv);
	RUN_TEST_CASE(vGBMemoryWrite, TimerControl);
	RUN_TEST_CASE(vGBMemoryWrite, TimerCompleteHigh);
	RUN_TEST_CASE(vGBMemoryWrite, TimerCompleteLow);
	RUN_TEST_CASE(vGBMemoryWrite, DisableBootRom);
}


