################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/ds18b20/ds18b20.c \
../Drivers/ds18b20/onewire.c 

OBJS += \
./Drivers/ds18b20/ds18b20.o \
./Drivers/ds18b20/onewire.o 

C_DEPS += \
./Drivers/ds18b20/ds18b20.d \
./Drivers/ds18b20/onewire.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/ds18b20/ds18b20.o: ../Drivers/ds18b20/ds18b20.c Drivers/ds18b20/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/ds18b20 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/ds18b20/ds18b20.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Drivers/ds18b20/onewire.o: ../Drivers/ds18b20/onewire.c Drivers/ds18b20/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/ds18b20 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/ds18b20/onewire.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

