################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/AFIO.c \
../Core/Src/BOOTLOADER.c \
../Core/Src/CAN.c \
../Core/Src/CRC.c \
../Core/Src/DMA.c \
../Core/Src/FLASH.c \
../Core/Src/GPIO.c \
../Core/Src/NVIC.c \
../Core/Src/RCC.c \
../Core/Src/SCB.c \
../Core/Src/SysTick.c \
../Core/Src/UART.c \
../Core/Src/bspatch.c \
../Core/Src/cmox_low_level_template.c \
../Core/Src/fastlz.c \
../Core/Src/lz.c \
../Core/Src/main.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syscalls.c 

OBJS += \
./Core/Src/AFIO.o \
./Core/Src/BOOTLOADER.o \
./Core/Src/CAN.o \
./Core/Src/CRC.o \
./Core/Src/DMA.o \
./Core/Src/FLASH.o \
./Core/Src/GPIO.o \
./Core/Src/NVIC.o \
./Core/Src/RCC.o \
./Core/Src/SCB.o \
./Core/Src/SysTick.o \
./Core/Src/UART.o \
./Core/Src/bspatch.o \
./Core/Src/cmox_low_level_template.o \
./Core/Src/fastlz.o \
./Core/Src/lz.o \
./Core/Src/main.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syscalls.o 

C_DEPS += \
./Core/Src/AFIO.d \
./Core/Src/BOOTLOADER.d \
./Core/Src/CAN.d \
./Core/Src/CRC.d \
./Core/Src/DMA.d \
./Core/Src/FLASH.d \
./Core/Src/GPIO.d \
./Core/Src/NVIC.d \
./Core/Src/RCC.d \
./Core/Src/SCB.d \
./Core/Src/SysTick.d \
./Core/Src/UART.d \
./Core/Src/bspatch.d \
./Core/Src/cmox_low_level_template.d \
./Core/Src/fastlz.d \
./Core/Src/lz.d \
./Core/Src/main.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syscalls.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/AFIO.d ./Core/Src/AFIO.o ./Core/Src/AFIO.su ./Core/Src/BOOTLOADER.d ./Core/Src/BOOTLOADER.o ./Core/Src/BOOTLOADER.su ./Core/Src/CAN.d ./Core/Src/CAN.o ./Core/Src/CAN.su ./Core/Src/CRC.d ./Core/Src/CRC.o ./Core/Src/CRC.su ./Core/Src/DMA.d ./Core/Src/DMA.o ./Core/Src/DMA.su ./Core/Src/FLASH.d ./Core/Src/FLASH.o ./Core/Src/FLASH.su ./Core/Src/GPIO.d ./Core/Src/GPIO.o ./Core/Src/GPIO.su ./Core/Src/NVIC.d ./Core/Src/NVIC.o ./Core/Src/NVIC.su ./Core/Src/RCC.d ./Core/Src/RCC.o ./Core/Src/RCC.su ./Core/Src/SCB.d ./Core/Src/SCB.o ./Core/Src/SCB.su ./Core/Src/SysTick.d ./Core/Src/SysTick.o ./Core/Src/SysTick.su ./Core/Src/UART.d ./Core/Src/UART.o ./Core/Src/UART.su ./Core/Src/bspatch.d ./Core/Src/bspatch.o ./Core/Src/bspatch.su ./Core/Src/cmox_low_level_template.d ./Core/Src/cmox_low_level_template.o ./Core/Src/cmox_low_level_template.su ./Core/Src/fastlz.d ./Core/Src/fastlz.o ./Core/Src/fastlz.su ./Core/Src/lz.d ./Core/Src/lz.o ./Core/Src/lz.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/stm32f1xx_it.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su

.PHONY: clean-Core-2f-Src

