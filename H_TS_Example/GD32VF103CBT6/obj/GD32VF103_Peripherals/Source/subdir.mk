################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../GD32VF103_Peripherals/Source/gd32vf103_adc.c \
../GD32VF103_Peripherals/Source/gd32vf103_bkp.c \
../GD32VF103_Peripherals/Source/gd32vf103_can.c \
../GD32VF103_Peripherals/Source/gd32vf103_crc.c \
../GD32VF103_Peripherals/Source/gd32vf103_dac.c \
../GD32VF103_Peripherals/Source/gd32vf103_dbg.c \
../GD32VF103_Peripherals/Source/gd32vf103_dma.c \
../GD32VF103_Peripherals/Source/gd32vf103_eclic.c \
../GD32VF103_Peripherals/Source/gd32vf103_exmc.c \
../GD32VF103_Peripherals/Source/gd32vf103_exti.c \
../GD32VF103_Peripherals/Source/gd32vf103_fmc.c \
../GD32VF103_Peripherals/Source/gd32vf103_fwdgt.c \
../GD32VF103_Peripherals/Source/gd32vf103_gpio.c \
../GD32VF103_Peripherals/Source/gd32vf103_i2c.c \
../GD32VF103_Peripherals/Source/gd32vf103_pmu.c \
../GD32VF103_Peripherals/Source/gd32vf103_rcu.c \
../GD32VF103_Peripherals/Source/gd32vf103_rtc.c \
../GD32VF103_Peripherals/Source/gd32vf103_spi.c \
../GD32VF103_Peripherals/Source/gd32vf103_timer.c \
../GD32VF103_Peripherals/Source/gd32vf103_usart.c \
../GD32VF103_Peripherals/Source/gd32vf103_wwdgt.c 

OBJS += \
./GD32VF103_Peripherals/Source/gd32vf103_adc.o \
./GD32VF103_Peripherals/Source/gd32vf103_bkp.o \
./GD32VF103_Peripherals/Source/gd32vf103_can.o \
./GD32VF103_Peripherals/Source/gd32vf103_crc.o \
./GD32VF103_Peripherals/Source/gd32vf103_dac.o \
./GD32VF103_Peripherals/Source/gd32vf103_dbg.o \
./GD32VF103_Peripherals/Source/gd32vf103_dma.o \
./GD32VF103_Peripherals/Source/gd32vf103_eclic.o \
./GD32VF103_Peripherals/Source/gd32vf103_exmc.o \
./GD32VF103_Peripherals/Source/gd32vf103_exti.o \
./GD32VF103_Peripherals/Source/gd32vf103_fmc.o \
./GD32VF103_Peripherals/Source/gd32vf103_fwdgt.o \
./GD32VF103_Peripherals/Source/gd32vf103_gpio.o \
./GD32VF103_Peripherals/Source/gd32vf103_i2c.o \
./GD32VF103_Peripherals/Source/gd32vf103_pmu.o \
./GD32VF103_Peripherals/Source/gd32vf103_rcu.o \
./GD32VF103_Peripherals/Source/gd32vf103_rtc.o \
./GD32VF103_Peripherals/Source/gd32vf103_spi.o \
./GD32VF103_Peripherals/Source/gd32vf103_timer.o \
./GD32VF103_Peripherals/Source/gd32vf103_usart.o \
./GD32VF103_Peripherals/Source/gd32vf103_wwdgt.o 

C_DEPS += \
./GD32VF103_Peripherals/Source/gd32vf103_adc.d \
./GD32VF103_Peripherals/Source/gd32vf103_bkp.d \
./GD32VF103_Peripherals/Source/gd32vf103_can.d \
./GD32VF103_Peripherals/Source/gd32vf103_crc.d \
./GD32VF103_Peripherals/Source/gd32vf103_dac.d \
./GD32VF103_Peripherals/Source/gd32vf103_dbg.d \
./GD32VF103_Peripherals/Source/gd32vf103_dma.d \
./GD32VF103_Peripherals/Source/gd32vf103_eclic.d \
./GD32VF103_Peripherals/Source/gd32vf103_exmc.d \
./GD32VF103_Peripherals/Source/gd32vf103_exti.d \
./GD32VF103_Peripherals/Source/gd32vf103_fmc.d \
./GD32VF103_Peripherals/Source/gd32vf103_fwdgt.d \
./GD32VF103_Peripherals/Source/gd32vf103_gpio.d \
./GD32VF103_Peripherals/Source/gd32vf103_i2c.d \
./GD32VF103_Peripherals/Source/gd32vf103_pmu.d \
./GD32VF103_Peripherals/Source/gd32vf103_rcu.d \
./GD32VF103_Peripherals/Source/gd32vf103_rtc.d \
./GD32VF103_Peripherals/Source/gd32vf103_spi.d \
./GD32VF103_Peripherals/Source/gd32vf103_timer.d \
./GD32VF103_Peripherals/Source/gd32vf103_usart.d \
./GD32VF103_Peripherals/Source/gd32vf103_wwdgt.d 


# Each subdirectory must supply rules for building sources it contributes
GD32VF103_Peripherals/Source/%.o: ../GD32VF103_Peripherals/Source/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medlow -msmall-data-limit=8 -mno-save-restore -fno-strict-aliasing -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -DGD32VF103C_START -DUSE_STDPERIPH_DRIVER -D__stack_size=1K -I"E:\MCU\GD32VF1\GD32VF103CBT6" -I"E:\MCU\GD32VF1\GD32VF103CBT6\H_ThreadScheduler" -I"E:\MCU\GD32VF1\GD32VF103CBT6\Application" -I"E:\MCU\GD32VF1\GD32VF103CBT6\GD32VF103_Peripherals\Include" -I"E:\MCU\GD32VF1\GD32VF103CBT6\GD32VF103_Peripherals" -I"E:\MCU\GD32VF1\GD32VF103CBT6\RISCV\drivers" -I"E:\MCU\GD32VF1\GD32VF103CBT6\RISCV\env_Eclipse" -I"E:\MCU\GD32VF1\GD32VF103CBT6\RISCV\stubs" -I"E:\MCU\GD32VF1\GD32VF103CBT6\H_Lib" -I"E:\MCU\GD32VF1\GD32VF103CBT6\Package" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\InternalPeripherals\HAL" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\InternalPeripherals\IP" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\MemoryMalloc" -I"E:\MCU\GD32VF1\GD32VF103CBT6\BSP\Instance" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

