################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../H_Lib/H_FIFO.c \
../H_Lib/H_FIFO_byte.c \
../H_Lib/H_FIFO_uint16.c \
../H_Lib/H_FIFO_uint32.c \
../H_Lib/H_FIFO_uint64.c \
../H_Lib/H_Graphics.c \
../H_Lib/H_Icon.c \
../H_Lib/H_Malloc.c \
../H_Lib/H_Memory.c \
../H_Lib/H_MemoryMalloc.c \
../H_Lib/H_String.c 

OBJS += \
./H_Lib/H_FIFO.o \
./H_Lib/H_FIFO_byte.o \
./H_Lib/H_FIFO_uint16.o \
./H_Lib/H_FIFO_uint32.o \
./H_Lib/H_FIFO_uint64.o \
./H_Lib/H_Graphics.o \
./H_Lib/H_Icon.o \
./H_Lib/H_Malloc.o \
./H_Lib/H_Memory.o \
./H_Lib/H_MemoryMalloc.o \
./H_Lib/H_String.o 

C_DEPS += \
./H_Lib/H_FIFO.d \
./H_Lib/H_FIFO_byte.d \
./H_Lib/H_FIFO_uint16.d \
./H_Lib/H_FIFO_uint32.d \
./H_Lib/H_FIFO_uint64.d \
./H_Lib/H_Graphics.d \
./H_Lib/H_Icon.d \
./H_Lib/H_Malloc.d \
./H_Lib/H_Memory.d \
./H_Lib/H_MemoryMalloc.d \
./H_Lib/H_String.d 


# Each subdirectory must supply rules for building sources it contributes
H_Lib/%.o: ../H_Lib/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medlow -msmall-data-limit=8 -mno-save-restore -fno-strict-aliasing -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -DGD32VF103C_START -DUSE_STDPERIPH_DRIVER -D__stack_size=1K -I"E:\MCU\GD32VF1\GD32VF103CBT6" -I"E:\MCU\GD32VF1\GD32VF103CBT6\H_ThreadScheduler" -I"E:\MCU\GD32VF1\GD32VF103CBT6\Application" -I"E:\MCU\GD32VF1\GD32VF103CBT6\GD32VF103_Peripherals\Include" -I"E:\MCU\GD32VF1\GD32VF103CBT6\GD32VF103_Peripherals" -I"E:\MCU\GD32VF1\GD32VF103CBT6\RISCV\drivers" -I"E:\MCU\GD32VF1\GD32VF103CBT6\RISCV\env_Eclipse" -I"E:\MCU\GD32VF1\GD32VF103CBT6\RISCV\stubs" -I"E:\MCU\GD32VF1\GD32VF103CBT6\H_Lib" -I"E:\MCU\GD32VF1\GD32VF103CBT6\Package" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\InternalPeripherals\HAL" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\InternalPeripherals\IP" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\MemoryMalloc" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\Instance" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

