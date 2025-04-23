################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.c \
../ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.c 

CREF += \
RA0_TokenNet_Device-0.cref 

C_DEPS += \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.d \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.d 

OBJS += \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.o \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.o 

MAP += \
RA0_TokenNet_Device-0.map 


# Each subdirectory must supply rules for building sources it contributes
ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/%.o: ../ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/%.c
	@echo 'Building file: $<'
	$(file > $@.in,-mcpu=cortex-m23 -mthumb -mlittle-endian -Oz -ffunction-sections -fdata-sections -fno-strict-aliasing -fmessage-length=0 -funsigned-char -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Waggregate-return -Wno-parentheses-equality -Wfloat-equal -g3 -std=c99 -fshort-enums -fno-unroll-loops -I"C:\\Users\\ddmga\\OneDrive\\workspace\\RA0_TokenNet_Device-0\\src" -I"." -I"C:\\Users\\ddmga\\OneDrive\\workspace\\RA0_TokenNet_Device-0\\ra\\fsp\\inc" -I"C:\\Users\\ddmga\\OneDrive\\workspace\\RA0_TokenNet_Device-0\\ra\\fsp\\inc\\api" -I"C:\\Users\\ddmga\\OneDrive\\workspace\\RA0_TokenNet_Device-0\\ra\\fsp\\inc\\instances" -I"C:\\Users\\ddmga\\OneDrive\\workspace\\RA0_TokenNet_Device-0\\ra\\arm\\CMSIS_6\\CMSIS\\Core\\Include" -I"C:\\Users\\ddmga\\OneDrive\\workspace\\RA0_TokenNet_Device-0\\ra_gen" -I"C:\\Users\\ddmga\\OneDrive\\workspace\\RA0_TokenNet_Device-0\\Modules\\Scheduler" -I"C:\\Users\\ddmga\\OneDrive\\workspace\\RA0_TokenNet_Device-0\\Modules\\Serial_ASCII" -I"C:\\Users\\ddmga\\OneDrive\\workspace\\RA0_TokenNet_Device-0\\ra_cfg\\fsp_cfg\\bsp" -I"C:\\Users\\ddmga\\OneDrive\\workspace\\RA0_TokenNet_Device-0\\ra_cfg\\fsp_cfg" -D_RENESAS_RA_ -D_RA_CORE=CM23 -D_RA_ORDINAL=1 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -x c "$<" -c -o "$@")
	@clang --target=arm-none-eabi @"$@.in"

