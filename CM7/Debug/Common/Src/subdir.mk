################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Rami/STM32CubeIDE/workspace_1.4.0/stm32h747-gameboy/Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.c 

OBJS += \
./Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.o 

C_DEPS += \
./Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.d 


# Each subdirectory must supply rules for building sources it contributes
Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.o: C:/Users/Rami/STM32CubeIDE/workspace_1.4.0/stm32h747-gameboy/Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.c Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/Components/Common" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/CPU" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/Fonts" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/lcd" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

