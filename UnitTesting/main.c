/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Rami
 *
 * Created on August 19, 2018, 11:34 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "unity_fixture.h"

// include all test groups
#include "gbmemoryTest.c"
#include "gbppuTest.c"
#include "gbmbcTest.c"

static void RunAllTests(void)
{
	RUN_TEST_GROUP(vGBMemoryWrite);
	RUN_TEST_GROUP(prvGetTileLineData);
	RUN_TEST_GROUP(ucGBMBCreadBankX);
	RUN_TEST_GROUP(vGBMBCwrite);
}
/*
 * 
 */
int main(int argc, const char** argv) {

    return UnityMain(argc, argv, RunAllTests);

}

