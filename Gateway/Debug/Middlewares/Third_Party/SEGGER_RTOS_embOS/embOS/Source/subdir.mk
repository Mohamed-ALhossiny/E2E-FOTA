################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Source/JLINKMEM_Process.c \
../Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Source/OS_Error.c \
../Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Source/RTOSInit.c 

OBJS += \
./Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Source/JLINKMEM_Process.o \
./Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Source/OS_Error.o \
./Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Source/RTOSInit.o 

C_DEPS += \
./Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Source/JLINKMEM_Process.d \
./Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Source/OS_Error.d \
./Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Source/RTOSInit.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Source/%.o Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Source/%.su: ../Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Source/%.c Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../embOS -I../Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Include -I../Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Include/GCC -I"C:/Users/moham/STM32CubeIDE/workspace_1.10.1/Gateway/Core/mbedTLS/include/mbedtls" -I"C:/Users/moham/STM32CubeIDE/workspace_1.10.1/Gateway/Core/SEGGER" -I"C:/Users/moham/STM32CubeIDE/workspace_1.10.1/Gateway/Core/mbedTLS/include/psa" -I"C:/Users/moham/STM32CubeIDE/workspace_1.10.1/Gateway/Core/mbedTLS/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-SEGGER_RTOS_embOS-2f-embOS-2f-Source

clean-Middlewares-2f-Third_Party-2f-SEGGER_RTOS_embOS-2f-embOS-2f-Source:
	-$(RM) ./Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Source/JLINKMEM_Process.d ./Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Source/JLINKMEM_Process.o ./Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Source/JLINKMEM_Process.su ./Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Source/OS_Error.d ./Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Source/OS_Error.o ./Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Source/OS_Error.su ./Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Source/RTOSInit.d ./Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Source/RTOSInit.o ./Middlewares/Third_Party/SEGGER_RTOS_embOS/embOS/Source/RTOSInit.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-SEGGER_RTOS_embOS-2f-embOS-2f-Source

