################################################################################
# 自动生成的文件。不要编辑！
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include RISCV/stubs/subdir.mk
-include RISCV/env_Eclipse/subdir.mk
-include RISCV/drivers/subdir.mk
-include Package/NET_Interface/subdir.mk
-include Package/H_Stream/subdir.mk
-include Package/H_FIFO_Builder/subdir.mk
-include H_ThreadScheduler/LL/subdir.mk
-include H_ThreadScheduler/API/subdir.mk
-include H_Lib/subdir.mk
-include GD32VF103_Peripherals/Source/subdir.mk
-include GD32VF103_Peripherals/subdir.mk
-include BSP/MemoryMalloc/subdir.mk
-include BSP/InternalPeripherals/IP/subdir.mk
-include BSP/InternalPeripherals/HAL/subdir.mk
-include BSP/Instance/subdir.mk
-include BSP/subdir.mk
-include Application/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
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

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
GD32VF103CBT6.hex \

SECONDARY_LIST += \
GD32VF103CBT6.lst \

SECONDARY_SIZE += \
GD32VF103CBT6.siz \


# 所有目标
all: GD32VF103CBT6.elf secondary-outputs

# 工具调用
GD32VF103CBT6.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medlow -msmall-data-limit=8 -mno-save-restore -fno-strict-aliasing -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -T "E:\MCU\GD32VF1\GD32VF103CBT6\RISCV\env_Eclipse\GD32VF103xB.lds" -nostartfiles -Xlinker --gc-sections -L"../" -Xlinker --print-memory-usage -Wl,-Map,"GD32VF103CBT6.map" --specs=nano.specs -o "GD32VF103CBT6.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
GD32VF103CBT6.hex: GD32VF103CBT6.elf
	@	riscv-none-embed-objcopy -O ihex "GD32VF103CBT6.elf"  "GD32VF103CBT6.hex"
	@	@
GD32VF103CBT6.lst: GD32VF103CBT6.elf
	@	riscv-none-embed-objdump --source --all-headers --demangle --line-numbers --wide "GD32VF103CBT6.elf" > "GD32VF103CBT6.lst"
	@	@
GD32VF103CBT6.siz: GD32VF103CBT6.elf
	@	riscv-none-embed-size --format=berkeley "GD32VF103CBT6.elf"
	@	@
# 其他目标
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) GD32VF103CBT6.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
