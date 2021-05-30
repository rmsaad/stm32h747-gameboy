################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/lcd/stm32_lcd.c 

OBJS += \
./Utilities/stm32_lcd.o 

C_DEPS += \
./Utilities/stm32_lcd.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/stm32_lcd.o: C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/lcd/stm32_lcd.c Utilities/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/Components/Common" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/CPU" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/Fonts" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/lcd" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Utilities/stm32_lcd.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

