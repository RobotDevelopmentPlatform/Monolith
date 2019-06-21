################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.c 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.o 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/%.o: ../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -I"E:/STM32/workspace/Monolith/monolitowyKod/Inc" -I"E:/STM32/workspace/Monolith/monolitowyKod/Drivers/STM32F7xx_HAL_Driver/Inc" -I"E:/STM32/workspace/Monolith/monolitowyKod/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy" -I"E:/STM32/workspace/Monolith/monolitowyKod/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1" -I"E:/STM32/workspace/Monolith/monolitowyKod/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"E:/STM32/workspace/Monolith/monolitowyKod/Middlewares/Third_Party/FreeRTOS/Source/include" -I"E:/STM32/workspace/Monolith/monolitowyKod/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"E:/STM32/workspace/Monolith/monolitowyKod/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


