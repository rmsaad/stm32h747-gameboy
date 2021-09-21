/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   gbmemoryTest.c
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
#include "gbppu.c"
#include "gbppu.h"

TEST_GROUP(getTileLineData);

TEST_SETUP(getTileLineData)
{
}

TEST_TEAR_DOWN(getTileLineData)
{
}

TEST(getTileLineData, HighAddress)
{
	TEST_ASSERT_EQUAL_UINT8(0x00, 0x00);
}

TEST_GROUP_RUNNER(getTileLineData) {
	RUN_TEST_CASE(getTileLineData, HighAddress);
}

