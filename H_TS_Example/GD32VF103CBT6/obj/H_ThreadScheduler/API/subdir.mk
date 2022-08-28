################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../H_ThreadScheduler/API/H_ThreadScheduler.c \
../H_ThreadScheduler/API/H_ThreadScheduler_BinarySemaphore.c \
../H_ThreadScheduler/API/H_ThreadScheduler_Core.c \
../H_ThreadScheduler/API/H_ThreadScheduler_Core_Ext.c \
../H_ThreadScheduler/API/H_ThreadScheduler_Ext.c \
../H_ThreadScheduler/API/H_ThreadScheduler_Lock.c \
../H_ThreadScheduler/API/H_ThreadScheduler_Mailbox.c \
../H_ThreadScheduler/API/H_ThreadScheduler_MessageQueue.c \
../H_ThreadScheduler/API/H_ThreadScheduler_Semaphore.c \
../H_ThreadScheduler/API/H_ThreadScheduler_Thread.c \
../H_ThreadScheduler/API/H_ThreadScheduler_Thread_Ext.c 

OBJS += \
./H_ThreadScheduler/API/H_ThreadScheduler.o \
./H_ThreadScheduler/API/H_ThreadScheduler_BinarySemaphore.o \
./H_ThreadScheduler/API/H_ThreadScheduler_Core.o \
./H_ThreadScheduler/API/H_ThreadScheduler_Core_Ext.o \
./H_ThreadScheduler/API/H_ThreadScheduler_Ext.o \
./H_ThreadScheduler/API/H_ThreadScheduler_Lock.o \
./H_ThreadScheduler/API/H_ThreadScheduler_Mailbox.o \
./H_ThreadScheduler/API/H_ThreadScheduler_MessageQueue.o \
./H_ThreadScheduler/API/H_ThreadScheduler_Semaphore.o \
./H_ThreadScheduler/API/H_ThreadScheduler_Thread.o \
./H_ThreadScheduler/API/H_ThreadScheduler_Thread_Ext.o 

C_DEPS += \
./H_ThreadScheduler/API/H_ThreadScheduler.d \
./H_ThreadScheduler/API/H_ThreadScheduler_BinarySemaphore.d \
./H_ThreadScheduler/API/H_ThreadScheduler_Core.d \
./H_ThreadScheduler/API/H_ThreadScheduler_Core_Ext.d \
./H_ThreadScheduler/API/H_ThreadScheduler_Ext.d \
./H_ThreadScheduler/API/H_ThreadScheduler_Lock.d \
./H_ThreadScheduler/API/H_ThreadScheduler_Mailbox.d \
./H_ThreadScheduler/API/H_ThreadScheduler_MessageQueue.d \
./H_ThreadScheduler/API/H_ThreadScheduler_Semaphore.d \
./H_ThreadScheduler/API/H_ThreadScheduler_Thread.d \
./H_ThreadScheduler/API/H_ThreadScheduler_Thread_Ext.d 


# Each subdirectory must supply rules for building sources it contributes
H_ThreadScheduler/API/%.o: ../H_ThreadScheduler/API/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medlow -msmall-data-limit=8 -mno-save-restore -fno-strict-aliasing -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -DGD32VF103C_START -DUSE_STDPERIPH_DRIVER -D__stack_size=1K -I"E:\MCU\GD32VF1\GD32VF103CBT6" -I"E:\MCU\GD32VF1\GD32VF103CBT6\H_ThreadScheduler" -I"E:\MCU\GD32VF1\GD32VF103CBT6\Application" -I"E:\MCU\GD32VF1\GD32VF103CBT6\GD32VF103_Peripherals\Include" -I"E:\MCU\GD32VF1\GD32VF103CBT6\GD32VF103_Peripherals" -I"E:\MCU\GD32VF1\GD32VF103CBT6\RISCV\drivers" -I"E:\MCU\GD32VF1\GD32VF103CBT6\RISCV\env_Eclipse" -I"E:\MCU\GD32VF1\GD32VF103CBT6\RISCV\stubs" -I"E:\MCU\GD32VF1\GD32VF103CBT6\H_Lib" -I"E:\MCU\GD32VF1\GD32VF103CBT6\Package" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\InternalPeripherals\HAL" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\InternalPeripherals\IP" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\MemoryMalloc" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\Instance" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

