
cmake_minimum_required(VERSION 3.21.2)

project(gameboyUnitTests)

add_subdirectory(Unity-master)

add_library(gameboyLib STATIC ../CM7/Core/GameboyLib/Src/gbmemory.c 					
)

target_include_directories(gameboyLib 
	PUBLIC ../CM7/Core/GameboyLib/Inc
)

add_executable(${PROJECT_NAME} main.c )

target_link_libraries(${PROJECT_NAME} 
	PUBLIC unity
	PUBLIC gameboyLib
)
