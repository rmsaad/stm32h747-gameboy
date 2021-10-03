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

/* prvGetTileLineData function test group */ 

TEST_GROUP(prvGetTileLineData);

TEST_SETUP(prvGetTileLineData)
{
	memset(&mem.ram[VRAM_BASE], 0x00, 0x2000);
}

TEST_TEAR_DOWN(prvGetTileLineData)
{
}

// Basic Case
TEST(prvGetTileLineData, UnsiginedDataAddr_LowTileMap_Basic)
{	
	memset(&mem.ram[VRAM_BASE], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_LOW], 0x00, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x0000, 0x00, TILE_DATA_UNSIGNED_ADDR, TILE_MAP_LOCATION_LOW));
}

TEST(prvGetTileLineData, UnsiginedDataAddr_HighTileMap_Basic)
{	
	memset(&mem.ram[VRAM_BASE], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_HIGH], 0x00, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x0000, 0x00, TILE_DATA_UNSIGNED_ADDR, TILE_MAP_LOCATION_HIGH));
}

TEST(prvGetTileLineData, SignedDataAddr_LowTileMap_Basic)
{
	memset(&mem.ram[VRAM_BASE + 0x1000], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_LOW], 0x00, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x0000, 0x00, TILE_DATA_SIGNED_ADDR, TILE_MAP_LOCATION_LOW));
}

TEST(prvGetTileLineData, SignedDataAddr_HighTileMap_Basic)
{
	memset(&mem.ram[VRAM_BASE + 0x1000], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_HIGH], 0x00, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x0000, 0x00, TILE_DATA_SIGNED_ADDR, TILE_MAP_LOCATION_HIGH));
}

// Smallest VRAM Location
TEST(prvGetTileLineData, UnsiginedDataAddr_LowTileMap_LocTestSmall)
{	
	memset(&mem.ram[VRAM_BASE + (0x10 * 0x01)], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_LOW], 0x01, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x0000, 0x00, TILE_DATA_UNSIGNED_ADDR, TILE_MAP_LOCATION_LOW));
}

TEST(prvGetTileLineData, UnsiginedDataAddr_HighTileMap_LocTestSmall)
{	
	memset(&mem.ram[VRAM_BASE + (0x10 * 0x01)], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_HIGH], 0x01, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x0000, 0x00, TILE_DATA_UNSIGNED_ADDR, TILE_MAP_LOCATION_HIGH));
}

TEST(prvGetTileLineData, SignedDataAddr_LowTileMap_LocTestSmall)
{
	memset(&mem.ram[VRAM_BASE + 0x1000 + (0x10 * 0x01)], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_LOW], 0x01, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x0000, 0x00, TILE_DATA_SIGNED_ADDR, TILE_MAP_LOCATION_LOW));
}

TEST(prvGetTileLineData, SignedDataAddr_HighTileMap_LocTestSmall)
{
	memset(&mem.ram[VRAM_BASE + 0x1000 + (0x10 * 0x01)], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_HIGH], 0x01, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x0000, 0x00, TILE_DATA_SIGNED_ADDR, TILE_MAP_LOCATION_HIGH));
}

// Largest VRAM Location
TEST(prvGetTileLineData, UnsiginedDataAddr_LowTileMap_LocTestLarge)
{	
	memset(&mem.ram[VRAM_BASE + (0x10 * 0xFF)], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_LOW], 0xFF, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x0000, 0x00, TILE_DATA_UNSIGNED_ADDR, TILE_MAP_LOCATION_LOW));
}

TEST(prvGetTileLineData, UnsiginedDataAddr_HighTileMap_LocTestLarge)
{	
	memset(&mem.ram[VRAM_BASE + (0x10 * 0xFF)], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_HIGH], 0xFF, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x0000, 0x00, TILE_DATA_UNSIGNED_ADDR, TILE_MAP_LOCATION_HIGH));
}

TEST(prvGetTileLineData, SiginedDataAddr_LowTileMap_LocTestLarge)
{
	// 0x79 highest value for signed int8_t
	memset(&mem.ram[VRAM_BASE + 0x1000 + (0x10 * 0x79)], 0xF0, 0x02);															
	memset(&mem.ram[TILE_MAP_LOCATION_LOW], 0x79, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x0000, 0x00, TILE_DATA_SIGNED_ADDR, TILE_MAP_LOCATION_LOW));
}

TEST(prvGetTileLineData, SiginedDataAddr_HighTileMap_LocTestLarge)
{
	memset(&mem.ram[VRAM_BASE + 0x1000 + (0x10 * 0x79)], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_HIGH], 0x79, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x0000, 0x00, TILE_DATA_SIGNED_ADDR, TILE_MAP_LOCATION_HIGH));
}

// Smallest Tile Offset
TEST(prvGetTileLineData, UnsiginedDataAddr_LowTileMap_TileOffsetSmall)
{	
	memset(&mem.ram[VRAM_BASE + (0x10 * 0x52)], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_LOW + 0x0001], 0x52, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x0001, 0x00, TILE_DATA_UNSIGNED_ADDR, TILE_MAP_LOCATION_LOW));
}

TEST(prvGetTileLineData, UnsiginedDataAddr_HighTileMap_TileOffsetSmall)
{	
	memset(&mem.ram[VRAM_BASE + (0x10 * 0x52)], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_HIGH + 0x0001], 0x52, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x0001, 0x00, TILE_DATA_UNSIGNED_ADDR, TILE_MAP_LOCATION_HIGH));
}

TEST(prvGetTileLineData, SiginedDataAddr_LowTileMap_TileOffsetSmall)
{
	memset(&mem.ram[VRAM_BASE + 0x1000 + (0x10 * 0x52)], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_LOW + 0x0001], 0x52, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x0001, 0x00, TILE_DATA_SIGNED_ADDR, TILE_MAP_LOCATION_LOW));
}

TEST(prvGetTileLineData, SiginedDataAddr_HighTileMap_TileOffsetSmall)
{
	memset(&mem.ram[VRAM_BASE + 0x1000 + (0x10 * 0x52)], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_HIGH + 0x0001], 0x52, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x0001, 0x00, TILE_DATA_SIGNED_ADDR, TILE_MAP_LOCATION_HIGH));
}

// Largest Tile Offset
TEST(prvGetTileLineData, UnsiginedDataAddr_LowTileMap_TileOffsetLarge)
{	
	memset(&mem.ram[VRAM_BASE + (0x10 * 0x52)], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_LOW + 0x03FF], 0x52, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x03FF, 0x00, TILE_DATA_UNSIGNED_ADDR, TILE_MAP_LOCATION_LOW));
}

TEST(prvGetTileLineData, UnsiginedDataAddr_HighTileMap_TileOffsetLarge)
{	
	memset(&mem.ram[VRAM_BASE + (0x10 * 0x52)], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_HIGH + 0x03FF], 0x52, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x03FF, 0x00, TILE_DATA_UNSIGNED_ADDR, TILE_MAP_LOCATION_HIGH));
}

TEST(prvGetTileLineData, SiginedDataAddr_LowTileMap_TileOffsetLarge)
{
	memset(&mem.ram[VRAM_BASE + 0x1000 + (0x10 * 0x52)], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_LOW + 0x03FF], 0x52, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x03FF, 0x00, TILE_DATA_SIGNED_ADDR, TILE_MAP_LOCATION_LOW));
}

TEST(prvGetTileLineData, SiginedDataAddr_HighTileMap_TileOffsetLarge)
{
	memset(&mem.ram[VRAM_BASE + 0x1000 + (0x10 * 0x52)], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_HIGH + 0x03FF], 0x52, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x03FF, 0x00, TILE_DATA_SIGNED_ADDR, TILE_MAP_LOCATION_HIGH));
}

// Small Line Offset
TEST(prvGetTileLineData, UnsiginedDataAddr_LowTileMap_LineOffsetSmall)
{	
	memset(&mem.ram[VRAM_BASE + (0x10 * 0x52) + 0x01], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_LOW + 0x03FF], 0x52, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x03FF, 0x01, TILE_DATA_UNSIGNED_ADDR, TILE_MAP_LOCATION_LOW));
}

TEST(prvGetTileLineData, UnsiginedDataAddr_HighTileMap_LineOffsetSmall)
{	
	memset(&mem.ram[VRAM_BASE + (0x10 * 0x52) + 0x01], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_HIGH + 0x03FF], 0x52, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x03FF, 0x01, TILE_DATA_UNSIGNED_ADDR, TILE_MAP_LOCATION_HIGH));
}

TEST(prvGetTileLineData, SiginedDataAddr_LowTileMap_LineOffsetSmall)
{
	memset(&mem.ram[VRAM_BASE + 0x1000 + (0x10 * 0x52) + 0x01], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_LOW + 0x03FF], 0x52, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x03FF, 0x01, TILE_DATA_SIGNED_ADDR, TILE_MAP_LOCATION_LOW));
}

TEST(prvGetTileLineData, SiginedDataAddr_HighTileMap_LineOffsetSmall)
{
	memset(&mem.ram[VRAM_BASE + 0x1000 + (0x10 * 0x52) + 0x01], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_HIGH + 0x03FF], 0x52, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x03FF, 0x01, TILE_DATA_SIGNED_ADDR, TILE_MAP_LOCATION_HIGH));
}

// Largest Line Offset
TEST(prvGetTileLineData, UnsiginedDataAddr_LowTileMap_LineOffsetLarge)
{	
	memset(&mem.ram[VRAM_BASE + (0x10 * 0x52) + 0x07], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_LOW + 0x03FF], 0x52, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x03FF, 0x07, TILE_DATA_UNSIGNED_ADDR, TILE_MAP_LOCATION_LOW));
}

TEST(prvGetTileLineData, UnsiginedDataAddr_HighTileMap_LineOffsetLarge)
{	
	memset(&mem.ram[VRAM_BASE + (0x10 * 0x52) + 0x07], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_HIGH + 0x03FF], 0x52, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x03FF, 0x07, TILE_DATA_UNSIGNED_ADDR, TILE_MAP_LOCATION_HIGH));
}

TEST(prvGetTileLineData, SiginedDataAddr_LowTileMap_LineOffsetLarge)
{
	memset(&mem.ram[VRAM_BASE + 0x1000 + (0x10 * 0x52) + 0x07], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_LOW + 0x03FF], 0x52, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x03FF, 0x07, TILE_DATA_SIGNED_ADDR, TILE_MAP_LOCATION_LOW));
}

TEST(prvGetTileLineData, SiginedDataAddr_HighTileMap_LineOffsetLarge)
{
	memset(&mem.ram[VRAM_BASE + 0x1000 + (0x10 * 0x52) + 0x07], 0xF0, 0x02);
	memset(&mem.ram[TILE_MAP_LOCATION_HIGH + 0x03FF], 0x52, 0x01);
	TEST_ASSERT_EQUAL_UINT16(0xF0F0, prvGetTileLineData(0x03FF, 0x07, TILE_DATA_SIGNED_ADDR, TILE_MAP_LOCATION_HIGH));
}


TEST_GROUP_RUNNER(prvGetTileLineData) {
	
	RUN_TEST_CASE(prvGetTileLineData, UnsiginedDataAddr_LowTileMap_Basic);
	RUN_TEST_CASE(prvGetTileLineData, UnsiginedDataAddr_HighTileMap_Basic);
	RUN_TEST_CASE(prvGetTileLineData, SignedDataAddr_LowTileMap_Basic);
	RUN_TEST_CASE(prvGetTileLineData, SignedDataAddr_HighTileMap_Basic);
	
	RUN_TEST_CASE(prvGetTileLineData, UnsiginedDataAddr_LowTileMap_LocTestSmall);
	RUN_TEST_CASE(prvGetTileLineData, UnsiginedDataAddr_HighTileMap_LocTestSmall);
	RUN_TEST_CASE(prvGetTileLineData, SignedDataAddr_LowTileMap_LocTestSmall);
	RUN_TEST_CASE(prvGetTileLineData, SignedDataAddr_HighTileMap_LocTestSmall);
	
	RUN_TEST_CASE(prvGetTileLineData, UnsiginedDataAddr_LowTileMap_LocTestLarge);
	RUN_TEST_CASE(prvGetTileLineData, UnsiginedDataAddr_HighTileMap_LocTestLarge);
	RUN_TEST_CASE(prvGetTileLineData, SiginedDataAddr_LowTileMap_LocTestLarge);
	RUN_TEST_CASE(prvGetTileLineData, SiginedDataAddr_HighTileMap_LocTestLarge);
	
	RUN_TEST_CASE(prvGetTileLineData, UnsiginedDataAddr_LowTileMap_TileOffsetSmall);
	RUN_TEST_CASE(prvGetTileLineData, UnsiginedDataAddr_HighTileMap_TileOffsetSmall);
	RUN_TEST_CASE(prvGetTileLineData, SiginedDataAddr_LowTileMap_TileOffsetSmall);
	RUN_TEST_CASE(prvGetTileLineData, SiginedDataAddr_HighTileMap_TileOffsetSmall);
	
	RUN_TEST_CASE(prvGetTileLineData, UnsiginedDataAddr_LowTileMap_TileOffsetLarge);
	RUN_TEST_CASE(prvGetTileLineData, UnsiginedDataAddr_HighTileMap_TileOffsetLarge);
	RUN_TEST_CASE(prvGetTileLineData, SiginedDataAddr_LowTileMap_TileOffsetLarge);
	RUN_TEST_CASE(prvGetTileLineData, SiginedDataAddr_HighTileMap_TileOffsetLarge);
	
	RUN_TEST_CASE(prvGetTileLineData, UnsiginedDataAddr_LowTileMap_LineOffsetSmall);
	RUN_TEST_CASE(prvGetTileLineData, UnsiginedDataAddr_HighTileMap_LineOffsetSmall);
	RUN_TEST_CASE(prvGetTileLineData, SiginedDataAddr_LowTileMap_LineOffsetSmall);
	RUN_TEST_CASE(prvGetTileLineData, SiginedDataAddr_HighTileMap_LineOffsetSmall);
	
	RUN_TEST_CASE(prvGetTileLineData, UnsiginedDataAddr_LowTileMap_LineOffsetLarge);
	RUN_TEST_CASE(prvGetTileLineData, UnsiginedDataAddr_HighTileMap_LineOffsetLarge);
	RUN_TEST_CASE(prvGetTileLineData, SiginedDataAddr_LowTileMap_LineOffsetLarge);
	RUN_TEST_CASE(prvGetTileLineData, SiginedDataAddr_HighTileMap_LineOffsetLarge);
}

