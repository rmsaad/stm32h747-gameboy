
cmake_minimum_required(VERSION 3.21.2)

project(gameboyUnitTests)

add_subdirectory(Unity-master)

add_library(gameboyLib STATIC 
	../CM7/Core/GameboyLib/Src/gbmemory.c 
	../CM7/Core/GameboyLib/Src/gbcpu.c
	../CM7/Core/GameboyLib/Src/gbppu.c
	../CM7/Core/GameboyLib/Src/gbMBC.c
	../CM7/Core/GameboyLib/Src/gbfunctions.c
								
)

target_include_directories(gameboyLib 
	PUBLIC ../CM7/Core/GameboyLib/Inc
	PUBLIC ../CM7/Core/GameboyLib/Src
)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} 
	PUBLIC unity
	PUBLIC gameboyLib
)
