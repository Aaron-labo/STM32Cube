################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/LED/led.c 

OBJS += \
./Drivers/LED/led.o 

C_DEPS += \
./Drivers/LED/led.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/LED/led.o: ../Drivers/LED/led.c Drivers/LED/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Driver/LED -I../Drivers/KEY -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/LED/led.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

