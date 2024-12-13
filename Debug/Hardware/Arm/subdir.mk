################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Hardware/Arm/Arm.c 

OBJS += \
./Hardware/Arm/Arm.o 

C_DEPS += \
./Hardware/Arm/Arm.d 


# Each subdirectory must supply rules for building sources it contributes
Hardware/Arm/%.o Hardware/Arm/%.su Hardware/Arm/%.cyclo: ../Hardware/Arm/%.c Hardware/Arm/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"E:/STM32WorkSpace/Gongxun_Taolin/Hardware" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Hardware-2f-Arm

clean-Hardware-2f-Arm:
	-$(RM) ./Hardware/Arm/Arm.cyclo ./Hardware/Arm/Arm.d ./Hardware/Arm/Arm.o ./Hardware/Arm/Arm.su

.PHONY: clean-Hardware-2f-Arm

