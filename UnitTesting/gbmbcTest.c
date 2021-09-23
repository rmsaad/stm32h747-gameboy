/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   gbmbcTest.c
 * Author: Rami
 *
 * Created on September 21, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

//#include "unity.h"
#include "unity_fixture.h"

// need access to static functions for testing 
#include "gbMBC.c"
#include "gbMBC.h"

#define TESTROMSIZE 0xFFFFFF

unsigned char testROM[TESTROMSIZE];

/* ucGBMBCreadBankX Function Test Group */

TEST_GROUP(ucGBMBCreadBankX);

TEST_SETUP(ucGBMBCreadBankX)
{
	vGBMemorySetROM(testROM);
	memset(testROM, 0x00, TESTROMSIZE);
	memset(mem.ram, 0x00, 0xFFFF);
}

TEST_TEAR_DOWN(ucGBMBCreadBankX)
{
}

TEST(ucGBMBCreadBankX, CartROM0LowestRead)
{ 
	memset(&testROM[CARTROM_BANK0], 0xF0, 0x01);
	TEST_ASSERT_EQUAL_UINT8(0xF0, ucGBMBCreadBankX(CARTROM_BANK0));
}

TEST(ucGBMBCreadBankX, CartROM0TypicalRead)
{ 
	memset(&testROM[CARTROM_BANK0 + 0x0020], 0xF0, 0x01);
	TEST_ASSERT_EQUAL_UINT8(0xF0, ucGBMBCreadBankX(CARTROM_BANK0 + 0x0020));
}

TEST(ucGBMBCreadBankX, CartROM0HighestRead)
{ 
	memset(&testROM[CARTROM_BANKX - 1], 0xF0, 0x01);
	TEST_ASSERT_EQUAL_UINT8(0xF0, ucGBMBCreadBankX(CARTROM_BANKX - 1));
}

TEST(ucGBMBCreadBankX, NoMBCLowestRead)
{ 
	ucgbMBCcontrollerCode = 0;
	memset(&testROM[CARTROM_BANKX], 0xF0, 0x01);
	TEST_ASSERT_EQUAL_UINT8(0xF0, ucGBMBCreadBankX(CARTROM_BANKX));
}

TEST(ucGBMBCreadBankX, NoMBCTypicalRead)
{ 
	ucgbMBCcontrollerCode = 0;
	memset(&testROM[CARTROM_BANKX  + 0x0020], 0xF0, 0x01);
	TEST_ASSERT_EQUAL_UINT8(0xF0, ucGBMBCreadBankX(CARTROM_BANKX  + 0x0020));
}

TEST(ucGBMBCreadBankX, NoMBCHighestRead)
{ 
	ucgbMBCcontrollerCode = 0;
	memset(&testROM[VRAM_BASE - 1], 0xF0, 0x01);
	TEST_ASSERT_EQUAL_UINT8(0xF0, ucGBMBCreadBankX(VRAM_BASE - 1));
}

// -=------------------------------------
TEST(ucGBMBCreadBankX, MBC1LowestRead)
{ 
	ucgbMBCcontrollerCode = 1;
	ucgbMBCoffsetLow = 0x01; 
	ucgbMBCoffsetHigh = 0x00 << 5; 
	
	memset(&testROM[0x4000], 0xF0, 0x01);
	TEST_ASSERT_EQUAL_UINT8(0xF0, ucGBMBCreadBankX(0x4000));
}

TEST(ucGBMBCreadBankX, MBC1LowRead)
{ 
	ucgbMBCcontrollerCode = 1;
	ucgbMBCoffsetLow = 0x08; 
	ucgbMBCoffsetHigh = 0x00 << 5; 
	
	memset(&testROM[0x20000], 0xF0, 0x01);
	TEST_ASSERT_EQUAL_UINT8(0xF0, ucGBMBCreadBankX(0x4000));
}

TEST(ucGBMBCreadBankX, MBC1HighRead)
{ 
	ucgbMBCcontrollerCode = 1;
	ucgbMBCoffsetLow = 0x08; 
	ucgbMBCoffsetHigh = 0x03 << 5; 
	
	memset(&testROM[0x1A0000], 0xF0, 0x01);
	TEST_ASSERT_EQUAL_UINT8(0xF0, ucGBMBCreadBankX(0x4000));
}

TEST(ucGBMBCreadBankX, MBC1HighestRead)
{ 
	ucgbMBCcontrollerCode = 1;
	ucgbMBCoffsetLow = 0x1F; 
	ucgbMBCoffsetHigh = 0x03 << 5; 
	
	memset(&testROM[0x1FC000 + 0x3FFF], 0xF0, 0x01);
	TEST_ASSERT_EQUAL_UINT8(0xF0, ucGBMBCreadBankX(0x7FFF));
}

TEST_GROUP_RUNNER(ucGBMBCreadBankX) {
	RUN_TEST_CASE(ucGBMBCreadBankX, CartROM0LowestRead);
	RUN_TEST_CASE(ucGBMBCreadBankX, CartROM0TypicalRead);
	RUN_TEST_CASE(ucGBMBCreadBankX, CartROM0HighestRead);
	
	RUN_TEST_CASE(ucGBMBCreadBankX, NoMBCLowestRead);
	RUN_TEST_CASE(ucGBMBCreadBankX, NoMBCTypicalRead);
	RUN_TEST_CASE(ucGBMBCreadBankX, NoMBCHighestRead);
	
	RUN_TEST_CASE(ucGBMBCreadBankX, MBC1LowestRead);
	RUN_TEST_CASE(ucGBMBCreadBankX, MBC1LowRead);
	RUN_TEST_CASE(ucGBMBCreadBankX, MBC1HighRead);
	RUN_TEST_CASE(ucGBMBCreadBankX, MBC1HighestRead);
}

/* vGBMBCwrite Function Test Group */

TEST_GROUP(vGBMBCwrite);

TEST_SETUP(vGBMBCwrite)
{
	ucgbMBCcontrollerCode = 1;
	ucgbMBCoffsetLow = 1;
	ucgbMBCoffsetHigh = 0;
	ucgbMBCbankMode = 0;
	memset(mem.ram, 0x00, 0xFFFF);
	ucgbMBCramEN = 0;
}

TEST_TEAR_DOWN(vGBMBCwrite)
{
}

TEST(vGBMBCwrite, EnableRamLowest)
{ 
	vGBMBCwrite(0x0000, 0x0A);
	TEST_ASSERT_EQUAL_UINT8(0x01, ucgbMBCramEN);
}

TEST(vGBMBCwrite, DisableRamLowest)
{ 
	ucgbMBCramEN = 1;
	vGBMBCwrite(0x1234, 0xFF);
	TEST_ASSERT_EQUAL_UINT8(0x00, ucgbMBCramEN);
	
	ucgbMBCramEN = 1;
	vGBMBCwrite(0x1234, 0x11);
	TEST_ASSERT_EQUAL_UINT8(0x00, ucgbMBCramEN);
}

TEST(vGBMBCwrite, EnableRamTypical)
{ 
	vGBMBCwrite(0x1234, 0x0A);
	TEST_ASSERT_EQUAL_UINT8(0x01, ucgbMBCramEN);
}

TEST(vGBMBCwrite, DisableRamTypical)
{ 
	ucgbMBCramEN = 1;
	vGBMBCwrite(0x1234, 0xFF);
	TEST_ASSERT_EQUAL_UINT8(0x00, ucgbMBCramEN);
	
	ucgbMBCramEN = 1;
	vGBMBCwrite(0x1234, 0x11);
	TEST_ASSERT_EQUAL_UINT8(0x00, ucgbMBCramEN);
}

TEST(vGBMBCwrite, EnableRamHighest)
{ 
	vGBMBCwrite(0x2000 - 1, 0x0A);
	TEST_ASSERT_EQUAL_UINT8(0x01, ucgbMBCramEN);
}

TEST(vGBMBCwrite, DisableRamHighest)
{ 
	ucgbMBCramEN = 1;
	vGBMBCwrite(0x2000 - 1, 0xFF);
	TEST_ASSERT_EQUAL_UINT8(0x00, ucgbMBCramEN);
	
	ucgbMBCramEN = 1;
	vGBMBCwrite(0x2000 - 1, 0x11);
	TEST_ASSERT_EQUAL_UINT8(0x00, ucgbMBCramEN);
}

TEST(vGBMBCwrite, SetSmallROMBANKXRegisterLowest)
{ 
	vGBMBCwrite(0x2000, 0x00);
	TEST_ASSERT_EQUAL_UINT8(0x01, ucgbMBCoffsetLow);
	
	ucgbMBCoffsetLow = 1;
	vGBMBCwrite(0x2555, 0x00);
	TEST_ASSERT_EQUAL_UINT8(0x01, ucgbMBCoffsetLow);
	
	ucgbMBCoffsetLow = 1;
	vGBMBCwrite(0x4000 - 1, 0x00);
	TEST_ASSERT_EQUAL_UINT8(0x01, ucgbMBCoffsetLow);
}

TEST(vGBMBCwrite, SetSmallROMBANKXRegisterTypical)
{ 
	vGBMBCwrite(0x2000, 0x07);
	TEST_ASSERT_EQUAL_UINT8(0x07, ucgbMBCoffsetLow);

	ucgbMBCoffsetLow = 1;
	vGBMBCwrite(0x2555, 0x07);
	TEST_ASSERT_EQUAL_UINT8(0x07, ucgbMBCoffsetLow);
	
	ucgbMBCoffsetLow = 1;
	vGBMBCwrite(0x4000 - 1, 0x07);
	TEST_ASSERT_EQUAL_UINT8(0x07, ucgbMBCoffsetLow);
}

TEST(vGBMBCwrite, SetSmallROMBANKXRegisterHighest)
{ 
	vGBMBCwrite(0x2000, 0x1F);
	TEST_ASSERT_EQUAL_UINT8(0x1F, ucgbMBCoffsetLow);
	
	ucgbMBCoffsetLow = 1;
	vGBMBCwrite(0x2555, 0x1F);
	TEST_ASSERT_EQUAL_UINT8(0x1F, ucgbMBCoffsetLow);
	
	ucgbMBCoffsetLow = 1;
	vGBMBCwrite(0x4000 - 1, 0x1F);
	TEST_ASSERT_EQUAL_UINT8(0x1F, ucgbMBCoffsetLow);
}

TEST(vGBMBCwrite, SetSmallROMBANKXRegisterMasked)
{ 
	vGBMBCwrite(0x2000, 0xFF);
	TEST_ASSERT_EQUAL_UINT8(0x1F, ucgbMBCoffsetLow);
	
	ucgbMBCoffsetLow = 1;
	vGBMBCwrite(0x2555, 0xFF);
	TEST_ASSERT_EQUAL_UINT8(0x1F, ucgbMBCoffsetLow);
	
	ucgbMBCoffsetLow = 1;
	vGBMBCwrite(0x4000 - 1, 0xFF);
	TEST_ASSERT_EQUAL_UINT8(0x1F, ucgbMBCoffsetLow);
}

TEST(vGBMBCwrite, SetLargeROMBANKXRegisterLowest)
{ 
	vGBMBCwrite(0x4000, 0x01);
	TEST_ASSERT_EQUAL_UINT8(0x01 << 5, ucgbMBCoffsetHigh);
	
	ucgbMBCoffsetHigh = 0;
	vGBMBCwrite(0x4555, 0x01);
	TEST_ASSERT_EQUAL_UINT8(0x01 << 5, ucgbMBCoffsetHigh);
	
	ucgbMBCoffsetHigh = 0;
	vGBMBCwrite(0x6000 - 1, 0x01);
	TEST_ASSERT_EQUAL_UINT8(0x01 << 5, ucgbMBCoffsetHigh);
}

TEST(vGBMBCwrite, SetLargeROMBANKXRegisterHighest)
{ 
	vGBMBCwrite(0x4000, 0x03);
	TEST_ASSERT_EQUAL_UINT8(0x03 << 5, ucgbMBCoffsetHigh);
	
	ucgbMBCoffsetHigh = 0;
	vGBMBCwrite(0x4555, 0x03);
	TEST_ASSERT_EQUAL_UINT8(0x03 << 5, ucgbMBCoffsetHigh);
	
	ucgbMBCoffsetHigh = 0;
	vGBMBCwrite(0x6000 - 1, 0x03);
	TEST_ASSERT_EQUAL_UINT8(0x03 << 5, ucgbMBCoffsetHigh);
}

TEST(vGBMBCwrite, SetLargeROMBANKXRegisterMasked)
{ 
	vGBMBCwrite(0x4000, 0xFF);
	TEST_ASSERT_EQUAL_UINT8(0x03 << 5, ucgbMBCoffsetHigh);
	
	ucgbMBCoffsetHigh = 0;
	vGBMBCwrite(0x4555, 0xFF);
	TEST_ASSERT_EQUAL_UINT8(0x03 << 5, ucgbMBCoffsetHigh);
	
	ucgbMBCoffsetHigh = 0;
	vGBMBCwrite(0x6000 - 1, 0xFF);
	TEST_ASSERT_EQUAL_UINT8(0x03 << 5, ucgbMBCoffsetHigh);
}

TEST(vGBMBCwrite, SelectROM)
{ 
	ucgbMBCbankMode = 1;
	vGBMBCwrite(0x6000, 0x00);
	TEST_ASSERT_EQUAL_UINT8(0x00, ucgbMBCbankMode);
	
	ucgbMBCbankMode = 1;
	vGBMBCwrite(0x6555, 0x00);
	TEST_ASSERT_EQUAL_UINT8(0x00, ucgbMBCbankMode);
	
	ucgbMBCbankMode = 1;
	vGBMBCwrite(0x8000 - 1, 0x00);
	TEST_ASSERT_EQUAL_UINT8(0x00, ucgbMBCbankMode);
}

TEST(vGBMBCwrite, SelectRAM)
{ 
	ucgbMBCbankMode = 0;
	vGBMBCwrite(0x6000, 0x01);
	TEST_ASSERT_EQUAL_UINT8(0x01, ucgbMBCbankMode);
	
	ucgbMBCbankMode = 0;
	vGBMBCwrite(0x6555, 0x01);
	TEST_ASSERT_EQUAL_UINT8(0x01, ucgbMBCbankMode);
	
	ucgbMBCbankMode = 0;
	vGBMBCwrite(0x8000 - 1, 0x01);
	TEST_ASSERT_EQUAL_UINT8(0x01, ucgbMBCbankMode);
}

TEST_GROUP_RUNNER(vGBMBCwrite) {
	RUN_TEST_CASE(vGBMBCwrite, EnableRamLowest);
	RUN_TEST_CASE(vGBMBCwrite, DisableRamLowest);
	RUN_TEST_CASE(vGBMBCwrite, EnableRamTypical);
	RUN_TEST_CASE(vGBMBCwrite, DisableRamTypical);
	RUN_TEST_CASE(vGBMBCwrite, EnableRamHighest);
	RUN_TEST_CASE(vGBMBCwrite, DisableRamHighest);
	
	RUN_TEST_CASE(vGBMBCwrite, SetSmallROMBANKXRegisterLowest);
	RUN_TEST_CASE(vGBMBCwrite, SetSmallROMBANKXRegisterTypical);
	RUN_TEST_CASE(vGBMBCwrite, SetSmallROMBANKXRegisterHighest);
	RUN_TEST_CASE(vGBMBCwrite, SetSmallROMBANKXRegisterMasked);
	
	RUN_TEST_CASE(vGBMBCwrite, SetLargeROMBANKXRegisterLowest);
	RUN_TEST_CASE(vGBMBCwrite, SetLargeROMBANKXRegisterHighest);
	RUN_TEST_CASE(vGBMBCwrite, SetLargeROMBANKXRegisterMasked);

	RUN_TEST_CASE(vGBMBCwrite, SelectROM);
	RUN_TEST_CASE(vGBMBCwrite, SelectRAM);
	
}