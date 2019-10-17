################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/drive.c \
../Src/freertos.c \
../Src/gpio.c \
../Src/main.c \
../Src/spi.c \
../Src/stm32f7xx_hal_msp.c \
../Src/stm32f7xx_hal_timebase_TIM.c \
../Src/stm32f7xx_it.c \
../Src/system_stm32f7xx.c \
../Src/tim.c \
../Src/usart.c 

OBJS += \
./Src/drive.o \
./Src/freertos.o \
./Src/gpio.o \
./Src/main.o \
./Src/spi.o \
./Src/stm32f7xx_hal_msp.o \
./Src/stm32f7xx_hal_timebase_TIM.o \
./Src/stm32f7xx_it.o \
./Src/system_stm32f7xx.o \
./Src/tim.o \
./Src/usart.o 

C_DEPS += \
./Src/drive.d \
./Src/freertos.d \
./Src/gpio.d \
./Src/main.d \
./Src/spi.d \
./Src/stm32f7xx_hal_msp.d \
./Src/stm32f7xx_hal_timebase_TIM.d \
./Src/stm32f7xx_it.d \
./Src/system_stm32f7xx.d \
./Src/tim.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -I"D:/Users/Micha³/GitHub/Monolith/monolitowyKod/Inc" -I"D:/Users/Micha³/GitHub/Monolith/monolitowyKod/Drivers/STM32F7xx_HAL_Driver/Inc" -I"D:/Users/Micha³/GitHub/Monolith/monolitowyKod/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy" -I"D:/Users/Micha³/GitHub/Monolith/monolitowyKod/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1" -I"D:/Users/Micha³/GitHub/Monolith/monolitowyKod/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"D:/Users/Micha³/GitHub/Monolith/monolitowyKod/Middlewares/Third_Party/FreeRTOS/Source/include" -I"D:/Users/Micha³/GitHub/Monolith/monolitowyKod/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"D:/Users/Micha³/GitHub/Monolith/monolitowyKod/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


