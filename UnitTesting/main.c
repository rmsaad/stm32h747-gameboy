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

//#include "unity.h"
#include "unity_fixture.h"

static char output[100];
static const char* expected;

TEST_GROUP(lazy);

TEST_SETUP(lazy)
{
	memset(output, 0xaa, sizeof output);
	expected = "";
}

TEST_TEAR_DOWN(lazy)
{
}

TEST(lazy, NoFormatOperations)
{
	char output[5] = "";
	TEST_ASSERT_EQUAL(3, sprintf(output, "hey"));
	TEST_ASSERT_EQUAL_STRING("hey", output);
}

TEST(lazy, InsertString)
{
	char output[20] = "";

	TEST_ASSERT_EQUAL(12, sprintf(output, "Hello %s\n", "World"));
	TEST_ASSERT_EQUAL_STRING("Hello World\n", output);
}

TEST_GROUP_RUNNER(lazy) {
	RUN_TEST_CASE(lazy, NoFormatOperations);
	RUN_TEST_CASE(lazy, InsertString);
}

static void RunAllTests(void)
{
	RUN_TEST_GROUP(lazy);
}
/*
 * 
 */
int main(int argc, const char** argv) {

    return UnityMain(argc, argv, RunAllTests);

}

