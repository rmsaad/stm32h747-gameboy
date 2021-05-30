################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery.c \
C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_bus.c \
C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_lcd.c \
C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_sdram.c 

OBJS += \
./Drivers/BSP/STM32H747I_DISCO/stm32h747i_discovery.o \
./Drivers/BSP/STM32H747I_DISCO/stm32h747i_discovery_bus.o \
./Drivers/BSP/STM32H747I_DISCO/stm32h747i_discovery_lcd.o \
./Drivers/BSP/STM32H747I_DISCO/stm32h747i_discovery_sdram.o 

C_DEPS += \
./Drivers/BSP/STM32H747I_DISCO/stm32h747i_discovery.d \
./Drivers/BSP/STM32H747I_DISCO/stm32h747i_discovery_bus.d \
./Drivers/BSP/STM32H747I_DISCO/stm32h747i_discovery_lcd.d \
./Drivers/BSP/STM32H747I_DISCO/stm32h747i_discovery_sdram.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32H747I_DISCO/stm32h747i_discovery.o: C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery.c Drivers/BSP/STM32H747I_DISCO/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/Components/Common" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/CPU" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/Fonts" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/lcd" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/STM32H747I_DISCO/stm32h747i_discovery.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/STM32H747I_DISCO/stm32h747i_discovery_bus.o: C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_bus.c Drivers/BSP/STM32H747I_DISCO/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/Components/Common" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/CPU" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/Fonts" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/lcd" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/STM32H747I_DISCO/stm32h747i_discovery_bus.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/STM32H747I_DISCO/stm32h747i_discovery_lcd.o: C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_lcd.c Drivers/BSP/STM32H747I_DISCO/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/Components/Common" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/CPU" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/Fonts" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/lcd" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/STM32H747I_DISCO/stm32h747i_discovery_lcd.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/STM32H747I_DISCO/stm32h747i_discovery_sdram.o: C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO/stm32h747i_discovery_sdram.c Drivers/BSP/STM32H747I_DISCO/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/Components/Common" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/CPU" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/Fonts" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/lcd" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/STM32H747I_DISCO/stm32h747i_discovery_sdram.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

