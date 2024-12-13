################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Hardware/SCSLib/SCS.c \
../Hardware/SCSLib/SCS_Uart.c \
../Hardware/SCSLib/SCSerial.c \
../Hardware/SCSLib/SMS_STS.c \
../Hardware/SCSLib/SyncRead.c 

OBJS += \
./Hardware/SCSLib/SCS.o \
./Hardware/SCSLib/SCS_Uart.o \
./Hardware/SCSLib/SCSerial.o \
./Hardware/SCSLib/SMS_STS.o \
./Hardware/SCSLib/SyncRead.o 

C_DEPS += \
./Hardware/SCSLib/SCS.d \
./Hardware/SCSLib/SCS_Uart.d \
./Hardware/SCSLib/SCSerial.d \
./Hardware/SCSLib/SMS_STS.d \
./Hardware/SCSLib/SyncRead.d 


# Each subdirectory must supply rules for building sources it contributes
Hardware/SCSLib/%.o Hardware/SCSLib/%.su Hardware/SCSLib/%.cyclo: ../Hardware/SCSLib/%.c Hardware/SCSLib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"E:/STM32WorkSpace/Gongxun_Taolin/Hardware" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Hardware-2f-SCSLib

clean-Hardware-2f-SCSLib:
	-$(RM) ./Hardware/SCSLib/SCS.cyclo ./Hardware/SCSLib/SCS.d ./Hardware/SCSLib/SCS.o ./Hardware/SCSLib/SCS.su ./Hardware/SCSLib/SCS_Uart.cyclo ./Hardware/SCSLib/SCS_Uart.d ./Hardware/SCSLib/SCS_Uart.o ./Hardware/SCSLib/SCS_Uart.su ./Hardware/SCSLib/SCSerial.cyclo ./Hardware/SCSLib/SCSerial.d ./Hardware/SCSLib/SCSerial.o ./Hardware/SCSLib/SCSerial.su ./Hardware/SCSLib/SMS_STS.cyclo ./Hardware/SCSLib/SMS_STS.d ./Hardware/SCSLib/SMS_STS.o ./Hardware/SCSLib/SMS_STS.su ./Hardware/SCSLib/SyncRead.cyclo ./Hardware/SCSLib/SyncRead.d ./Hardware/SCSLib/SyncRead.o ./Hardware/SCSLib/SyncRead.su

.PHONY: clean-Hardware-2f-SCSLib

