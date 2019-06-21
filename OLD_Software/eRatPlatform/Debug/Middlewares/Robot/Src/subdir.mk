################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Robot/Src/WheelDrive.c \
../Middlewares/Robot/Src/WheelDriveSystem.c 

OBJS += \
./Middlewares/Robot/Src/WheelDrive.o \
./Middlewares/Robot/Src/WheelDriveSystem.o 

C_DEPS += \
./Middlewares/Robot/Src/WheelDrive.d \
./Middlewares/Robot/Src/WheelDriveSystem.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Robot/Src/%.o: ../Middlewares/Robot/Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -I"C:/Users/osk30/OneDrive/2018/eRat/Modules/eRatPlatform/software/eRatPlatform/Inc" -I"C:/Users/osk30/OneDrive/2018/eRat/Modules/eRatPlatform/software/eRatPlatform/Drivers/STM32F7xx_HAL_Driver/Inc" -I"C:/Users/osk30/OneDrive/2018/eRat/Modules/eRatPlatform/software/eRatPlatform/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy" -I"C:/Users/osk30/OneDrive/2018/eRat/Modules/eRatPlatform/software/eRatPlatform/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1" -I"C:/Users/osk30/OneDrive/2018/eRat/Modules/eRatPlatform/software/eRatPlatform/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/Users/osk30/OneDrive/2018/eRat/Modules/eRatPlatform/software/eRatPlatform/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/Users/osk30/OneDrive/2018/eRat/Modules/eRatPlatform/software/eRatPlatform/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Users/osk30/OneDrive/2018/eRat/Modules/eRatPlatform/software/eRatPlatform/Drivers/CMSIS/Include" -I"C:/Users/osk30/OneDrive/2018/eRat/Modules/eRatPlatform/software/eRatPlatform/Middlewares/Robot/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


