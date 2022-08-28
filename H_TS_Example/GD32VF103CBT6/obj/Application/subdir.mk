################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Application/gd32vf103_it_ASM.s 

C_SRCS += \
../Application/H_Printf.c \
../Application/Test.c \
../Application/gd32vf103_it.c \
../Application/gd32xxx_Time.c \
../Application/gd32xxx_it.c \
../Application/main.c \
../Application/systick.c 

OBJS += \
./Application/H_Printf.o \
./Application/Test.o \
./Application/gd32vf103_it.o \
./Application/gd32vf103_it_ASM.o \
./Application/gd32xxx_Time.o \
./Application/gd32xxx_it.o \
./Application/main.o \
./Application/systick.o 

S_DEPS += \
./Application/gd32vf103_it_ASM.d 

C_DEPS += \
./Application/H_Printf.d \
./Application/Test.d \
./Application/gd32vf103_it.d \
./Application/gd32xxx_Time.d \
./Application/gd32xxx_it.d \
./Application/main.d \
./Application/systick.d 


# Each subdirectory must supply rules for building sources it contributes
Application/%.o: ../Application/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medlow -msmall-data-limit=8 -mno-save-restore -fno-strict-aliasing -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -DGD32VF103C_START -DUSE_STDPERIPH_DRIVER -D__stack_size=1K -I"E:\MCU\GD32VF1\GD32VF103CBT6" -I"E:\MCU\GD32VF1\GD32VF103CBT6\H_ThreadScheduler" -I"E:\MCU\GD32VF1\GD32VF103CBT6\Application" -I"E:\MCU\GD32VF1\GD32VF103CBT6\GD32VF103_Peripherals\Include" -I"E:\MCU\GD32VF1\GD32VF103CBT6\GD32VF103_Peripherals" -I"E:\MCU\GD32VF1\GD32VF103CBT6\RISCV\drivers" -I"E:\MCU\GD32VF1\GD32VF103CBT6\RISCV\env_Eclipse" -I"E:\MCU\GD32VF1\GD32VF103CBT6\RISCV\stubs" -I"E:\MCU\GD32VF1\GD32VF103CBT6\H_Lib" -I"E:\MCU\GD32VF1\GD32VF103CBT6\Package" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\InternalPeripherals\HAL" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\InternalPeripherals\IP" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\MemoryMalloc" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\Instance" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
Application/%.o: ../Application/%.s
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medlow -msmall-data-limit=8 -mno-save-restore -fno-strict-aliasing -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -x assembler-with-cpp -I"E:\MCU\GD32VF1\GD32VF103CBT6\RISCV\drivers" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

