################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Hardware/Emm_V5/Emm_V5.c 

OBJS += \
./Hardware/Emm_V5/Emm_V5.o 

C_DEPS += \
./Hardware/Emm_V5/Emm_V5.d 


# Each subdirectory must supply rules for building sources it contributes
Hardware/Emm_V5/%.o Hardware/Emm_V5/%.su Hardware/Emm_V5/%.cyclo: ../Hardware/Emm_V5/%.c Hardware/Emm_V5/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"E:/STM32WorkSpace/Gongxun_Taolin/Hardware" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Hardware-2f-Emm_V5

clean-Hardware-2f-Emm_V5:
	-$(RM) ./Hardware/Emm_V5/Emm_V5.cyclo ./Hardware/Emm_V5/Emm_V5.d ./Hardware/Emm_V5/Emm_V5.o ./Hardware/Emm_V5/Emm_V5.su

.PHONY: clean-Hardware-2f-Emm_V5

