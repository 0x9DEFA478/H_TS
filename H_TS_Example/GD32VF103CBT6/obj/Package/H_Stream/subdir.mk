################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Package/H_Stream/H_Reader.c \
../Package/H_Stream/H_Reader_Ext.c \
../Package/H_Stream/H_Stream.c \
../Package/H_Stream/H_Stream_Ext.c \
../Package/H_Stream/H_Writer.c \
../Package/H_Stream/H_Writer_Ext.c 

OBJS += \
./Package/H_Stream/H_Reader.o \
./Package/H_Stream/H_Reader_Ext.o \
./Package/H_Stream/H_Stream.o \
./Package/H_Stream/H_Stream_Ext.o \
./Package/H_Stream/H_Writer.o \
./Package/H_Stream/H_Writer_Ext.o 

C_DEPS += \
./Package/H_Stream/H_Reader.d \
./Package/H_Stream/H_Reader_Ext.d \
./Package/H_Stream/H_Stream.d \
./Package/H_Stream/H_Stream_Ext.d \
./Package/H_Stream/H_Writer.d \
./Package/H_Stream/H_Writer_Ext.d 


# Each subdirectory must supply rules for building sources it contributes
Package/H_Stream/%.o: ../Package/H_Stream/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medlow -msmall-data-limit=8 -mno-save-restore -fno-strict-aliasing -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -DGD32VF103C_START -DUSE_STDPERIPH_DRIVER -D__stack_size=1K -I"E:\MCU\GD32VF1\GD32VF103CBT6" -I"E:\MCU\GD32VF1\GD32VF103CBT6\H_ThreadScheduler" -I"E:\MCU\GD32VF1\GD32VF103CBT6\Application" -I"E:\MCU\GD32VF1\GD32VF103CBT6\GD32VF103_Peripherals\Include" -I"E:\MCU\GD32VF1\GD32VF103CBT6\GD32VF103_Peripherals" -I"E:\MCU\GD32VF1\GD32VF103CBT6\RISCV\drivers" -I"E:\MCU\GD32VF1\GD32VF103CBT6\RISCV\env_Eclipse" -I"E:\MCU\GD32VF1\GD32VF103CBT6\RISCV\stubs" -I"E:\MCU\GD32VF1\GD32VF103CBT6\H_Lib" -I"E:\MCU\GD32VF1\GD32VF103CBT6\Package" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\InternalPeripherals\HAL" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\InternalPeripherals\IP" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\MemoryMalloc" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\Instance" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

