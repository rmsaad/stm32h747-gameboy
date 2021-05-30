################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/Components/is42s32800j/is42s32800j.c \
C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/Components/otm8009a/otm8009a.c \
C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/Components/otm8009a/otm8009a_reg.c 

OBJS += \
./Drivers/BSP/Components/is42s32800j.o \
./Drivers/BSP/Components/otm8009a.o \
./Drivers/BSP/Components/otm8009a_reg.o 

C_DEPS += \
./Drivers/BSP/Components/is42s32800j.d \
./Drivers/BSP/Components/otm8009a.d \
./Drivers/BSP/Components/otm8009a_reg.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/is42s32800j.o: C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/Components/is42s32800j/is42s32800j.c Drivers/BSP/Components/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/Components/Common" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/CPU" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/Fonts" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/lcd" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/Components/is42s32800j.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/Components/otm8009a.o: C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/Components/otm8009a/otm8009a.c Drivers/BSP/Components/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/Components/Common" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/CPU" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/Fonts" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/lcd" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/Components/otm8009a.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/Components/otm8009a_reg.o: C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/Components/otm8009a/otm8009a_reg.c Drivers/BSP/Components/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/Components/Common" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/BSP/STM32H747I-DISCO" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/CMSIS/Include" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/CPU" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/Fonts" -I"C:/Users/Rami/STM32Cube/Repository/STM32Cube_FW_H7_V1.9.0/Utilities/lcd" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/Components/otm8009a_reg.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

