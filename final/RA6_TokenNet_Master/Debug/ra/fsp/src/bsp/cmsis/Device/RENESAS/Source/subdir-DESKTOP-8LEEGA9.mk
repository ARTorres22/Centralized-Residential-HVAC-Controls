################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.c \
../ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.c 

CREF += \
RA6_TokenNet_Master.cref 

C_DEPS += \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.d \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.d 

OBJS += \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.o \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.o 

MAP += \
RA6_TokenNet_Master.map 


# Each subdirectory must supply rules for building sources it contributes
ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/%.o: ../ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/%.c
	@echo 'Building file: $<'
	$(file > $@.in,-mcpu=cortex-m4 -mthumb -mlittle-endian -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -ffunction-sections -fdata-sections -fno-strict-aliasing -fmessage-length=0 -funsigned-char -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Waggregate-return -Wno-parentheses-equality -Wfloat-equal -g3 -std=c99 -fshort-enums -fno-unroll-loops -I"C:\\Users\\artor\\OneDrive\\workspace\\RA6_TokenNet_Master\\src" -I"." -I"C:\\Users\\artor\\OneDrive\\workspace\\RA6_TokenNet_Master\\ra\\fsp\\inc" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA6_TokenNet_Master\\ra\\fsp\\inc\\api" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA6_TokenNet_Master\\ra\\fsp\\inc\\instances" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA6_TokenNet_Master\\ra\\arm\\CMSIS_6\\CMSIS\\Core\\Include" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA6_TokenNet_Master\\ra_gen" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA6_TokenNet_Master\\Modules\\Scheduler" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA6_TokenNet_Master\\Modules\\Serial_ASCII" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA6_TokenNet_Master\\ra_cfg\\fsp_cfg\\bsp" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA6_TokenNet_Master\\ra_cfg\\fsp_cfg" -D_RENESAS_RA_ -D_RA_CORE=CM4 -D_RA_ORDINAL=1 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -x c "$<" -c -o "$@")
	@clang --target=arm-none-eabi @"$@.in"

