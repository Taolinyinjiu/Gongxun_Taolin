################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Hardware/Time/subdir.mk
-include Hardware/TX/subdir.mk
-include Hardware/Stability/subdir.mk
-include Hardware/Screen/subdir.mk
-include Hardware/SCSLib/subdir.mk
-include Hardware/Run/subdir.mk
-include Hardware/Location/subdir.mk
-include Hardware/Joy_Interface/subdir.mk
-include Hardware/HWT101CT/subdir.mk
-include Hardware/Emm_V5/subdir.mk
-include Hardware/Arm/subdir.mk
-include Hardware/Action/subdir.mk
-include Drivers/STM32H7xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Gongxun_Taolin
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Gongxun_Taolin.elf \

MAP_FILES += \
Gongxun_Taolin.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Gongxun_Taolin.list \


# All Target
all: main-build

# Main-build Target
main-build: Gongxun_Taolin.elf secondary-outputs

# Tool invocations
Gongxun_Taolin.elf Gongxun_Taolin.map: $(OBJS) $(USER_OBJS) E:\STM32WorkSpace\Gongxun_Taolin\STM32H723VGTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Gongxun_Taolin.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"E:\STM32WorkSpace\Gongxun_Taolin\STM32H723VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Gongxun_Taolin.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Gongxun_Taolin.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Gongxun_Taolin.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Gongxun_Taolin.elf Gongxun_Taolin.list Gongxun_Taolin.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
