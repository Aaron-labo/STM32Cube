################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/nRF24L01/nrf24l01.c 

OBJS += \
./Drivers/nRF24L01/nrf24l01.o 

C_DEPS += \
./Drivers/nRF24L01/nrf24l01.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/nRF24L01/nrf24l01.o: ../Drivers/nRF24L01/nrf24l01.c Drivers/nRF24L01/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/HC_SR04 -I../Drivers/LCD -I../Drivers/DS18B20 -I../Drivers/KEY -I../Drivers/nRF24L01 -I../Drivers/BEEP -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/nRF24L01/nrf24l01.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

