################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Robot/WheelDrive.c \
../Middlewares/Robot/WheelDriveSystem.c 

OBJS += \
./Middlewares/Robot/WheelDrive.o \
./Middlewares/Robot/WheelDriveSystem.o 

C_DEPS += \
./Middlewares/Robot/WheelDrive.d \
./Middlewares/Robot/WheelDriveSystem.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Robot/%.o: ../Middlewares/Robot/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -I"C:/Users/Oskar/OneDrive/2018/eRat/Modules/eRatPlatform/software/backup/eRatPlatform2/Inc" -I"C:/Users/Oskar/OneDrive/2018/eRat/Modules/eRatPlatform/software/backup/eRatPlatform2/Drivers/STM32F7xx_HAL_Driver/Inc" -I"C:/Users/Oskar/OneDrive/2018/eRat/Modules/eRatPlatform/software/backup/eRatPlatform2/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Oskar/OneDrive/2018/eRat/Modules/eRatPlatform/software/backup/eRatPlatform2/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1" -I"C:/Users/Oskar/OneDrive/2018/eRat/Modules/eRatPlatform/software/backup/eRatPlatform2/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/Users/Oskar/OneDrive/2018/eRat/Modules/eRatPlatform/software/backup/eRatPlatform2/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/Users/Oskar/OneDrive/2018/eRat/Modules/eRatPlatform/software/backup/eRatPlatform2/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Users/Oskar/OneDrive/2018/eRat/Modules/eRatPlatform/software/backup/eRatPlatform2/Middlewares/Robot" -I"C:/Users/Oskar/OneDrive/2018/eRat/Modules/eRatPlatform/software/backup/eRatPlatform2/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


