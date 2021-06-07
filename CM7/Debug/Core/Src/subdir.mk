################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/gbcpu.c \
../Core/Src/gbfunctions.c \
../Core/Src/gbmemory.c \
../Core/Src/main.c \
../Core/Src/stm32h7xx_hal_msp.c \
../Core/Src/stm32h7xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c 

OBJS += \
./Core/Src/gbcpu.o \
./Core/Src/gbfunctions.o \
./Core/Src/gbmemory.o \
./Core/Src/main.o \
./Core/Src/stm32h7xx_hal_msp.o \
./Core/Src/stm32h7xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o 

C_DEPS += \
./Core/Src/gbcpu.d \
./Core/Src/gbfunctions.d \
./Core/Src/gbmemory.d \
./Core/Src/main.d \
./Core/Src/stm32h7xx_hal_msp.d \
./Core/Src/stm32h7xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/gbcpu.o: ../Core/Src/gbcpu.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/Components/Common" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/CPU" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/Fonts" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/lcd" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/gbcpu.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/gbfunctions.o: ../Core/Src/gbfunctions.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/Components/Common" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/CPU" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/Fonts" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/lcd" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/gbfunctions.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/gbmemory.o: ../Core/Src/gbmemory.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/Components/Common" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/CPU" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/Fonts" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/lcd" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/gbmemory.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/main.o: ../Core/Src/main.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/Components/Common" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/CPU" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/Fonts" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/lcd" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/stm32h7xx_hal_msp.o: ../Core/Src/stm32h7xx_hal_msp.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/Components/Common" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/CPU" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/Fonts" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/lcd" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/stm32h7xx_hal_msp.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/stm32h7xx_it.o: ../Core/Src/stm32h7xx_it.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/Components/Common" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/CPU" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/Fonts" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/lcd" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/stm32h7xx_it.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/syscalls.o: ../Core/Src/syscalls.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/Components/Common" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/CPU" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/Fonts" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/lcd" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/syscalls.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/sysmem.o: ../Core/Src/sysmem.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/Components/Common" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/CPU" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/Fonts" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/lcd" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/sysmem.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

