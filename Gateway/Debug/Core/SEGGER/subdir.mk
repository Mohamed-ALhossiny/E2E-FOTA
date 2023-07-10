################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/SEGGER/SEGGER_RTT.c \
../Core/SEGGER/SEGGER_RTT_printf.c \
../Core/SEGGER/SEGGER_SYSVIEW.c \
../Core/SEGGER/SEGGER_SYSVIEW_Config_embOS.c \
../Core/SEGGER/SEGGER_SYSVIEW_embOS.c 

S_UPPER_SRCS += \
../Core/SEGGER/SEGGER_RTT_ASM_ARMv7M.S 

OBJS += \
./Core/SEGGER/SEGGER_RTT.o \
./Core/SEGGER/SEGGER_RTT_ASM_ARMv7M.o \
./Core/SEGGER/SEGGER_RTT_printf.o \
./Core/SEGGER/SEGGER_SYSVIEW.o \
./Core/SEGGER/SEGGER_SYSVIEW_Config_embOS.o \
./Core/SEGGER/SEGGER_SYSVIEW_embOS.o 

S_UPPER_DEPS += \
./Core/SEGGER/SEGGER_RTT_ASM_ARMv7M.d 

C_DEPS += \
./Core/SEGGER/SEGGER_RTT.d \
./Core/SEGGER/SEGGER_RTT_printf.d \
./Core/SEGGER/SEGGER_SYSVIEW.d \
./Core/SEGGER/SEGGER_SYSVIEW_Config_embOS.d \
./Core/SEGGER/SEGGER_SYSVIEW_embOS.d 


# Each subdirectory must supply rules for building sources it contributes
Core/SEGGER/%.o Core/SEGGER/%.su: ../Core/SEGGER/%.c Core/SEGGER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../embOS -I../Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Include -I../Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Include/GCC -I"C:/Users/moham/STM32CubeIDE/workspace_1.10.1/Gateway/Core/mbedTLS/include/mbedtls" -I"C:/Users/moham/STM32CubeIDE/workspace_1.10.1/Gateway/Core/SEGGER" -I"C:/Users/moham/STM32CubeIDE/workspace_1.10.1/Gateway/Core/mbedTLS/include/psa" -I"C:/Users/moham/STM32CubeIDE/workspace_1.10.1/Gateway/Core/mbedTLS/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/SEGGER/%.o: ../Core/SEGGER/%.S Core/SEGGER/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-SEGGER

clean-Core-2f-SEGGER:
	-$(RM) ./Core/SEGGER/SEGGER_RTT.d ./Core/SEGGER/SEGGER_RTT.o ./Core/SEGGER/SEGGER_RTT.su ./Core/SEGGER/SEGGER_RTT_ASM_ARMv7M.d ./Core/SEGGER/SEGGER_RTT_ASM_ARMv7M.o ./Core/SEGGER/SEGGER_RTT_printf.d ./Core/SEGGER/SEGGER_RTT_printf.o ./Core/SEGGER/SEGGER_RTT_printf.su ./Core/SEGGER/SEGGER_SYSVIEW.d ./Core/SEGGER/SEGGER_SYSVIEW.o ./Core/SEGGER/SEGGER_SYSVIEW.su ./Core/SEGGER/SEGGER_SYSVIEW_Config_embOS.d ./Core/SEGGER/SEGGER_SYSVIEW_Config_embOS.o ./Core/SEGGER/SEGGER_SYSVIEW_Config_embOS.su ./Core/SEGGER/SEGGER_SYSVIEW_embOS.d ./Core/SEGGER/SEGGER_SYSVIEW_embOS.o ./Core/SEGGER/SEGGER_SYSVIEW_embOS.su

.PHONY: clean-Core-2f-SEGGER

