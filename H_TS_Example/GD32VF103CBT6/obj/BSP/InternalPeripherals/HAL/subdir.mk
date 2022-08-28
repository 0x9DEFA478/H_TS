################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../BSP/InternalPeripherals/HAL/dma.c \
../BSP/InternalPeripherals/HAL/gpio.c \
../BSP/InternalPeripherals/HAL/usart.c 

OBJS += \
./BSP/InternalPeripherals/HAL/dma.o \
./BSP/InternalPeripherals/HAL/gpio.o \
./BSP/InternalPeripherals/HAL/usart.o 

C_DEPS += \
./BSP/InternalPeripherals/HAL/dma.d \
./BSP/InternalPeripherals/HAL/gpio.d \
./BSP/InternalPeripherals/HAL/usart.d 


# Each subdirectory must supply rules for building sources it contributes
BSP/InternalPeripherals/HAL/%.o: ../BSP/InternalPeripherals/HAL/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medlow -msmall-data-limit=8 -mno-save-restore -fno-strict-aliasing -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -DGD32VF103C_START -DUSE_STDPERIPH_DRIVER -D__stack_size=1K -I"E:\MCU\GD32VF1\GD32VF103CBT6" -I"E:\MCU\GD32VF1\GD32VF103CBT6\H_ThreadScheduler" -I"E:\MCU\GD32VF1\GD32VF103CBT6\Application" -I"E:\MCU\GD32VF1\GD32VF103CBT6\GD32VF103_Peripherals\Include" -I"E:\MCU\GD32VF1\GD32VF103CBT6\GD32VF103_Peripherals" -I"E:\MCU\GD32VF1\GD32VF103CBT6\RISCV\drivers" -I"E:\MCU\GD32VF1\GD32VF103CBT6\RISCV\env_Eclipse" -I"E:\MCU\GD32VF1\GD32VF103CBT6\RISCV\stubs" -I"E:\MCU\GD32VF1\GD32VF103CBT6\H_Lib" -I"E:\MCU\GD32VF1\GD32VF103CBT6\Package" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\InternalPeripherals\HAL" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\InternalPeripherals\IP" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\MemoryMalloc" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\Instance" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

