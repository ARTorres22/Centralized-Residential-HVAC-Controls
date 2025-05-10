################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Modules/Serial_ASCII/ASCII_numbers.c \
../Modules/Serial_ASCII/Serial.c \
../Modules/Serial_ASCII/Serial_user.c 

CREF += \
RA6_TokenNet_Master.cref 

C_DEPS += \
./Modules/Serial_ASCII/ASCII_numbers.d \
./Modules/Serial_ASCII/Serial.d \
./Modules/Serial_ASCII/Serial_user.d 

OBJS += \
./Modules/Serial_ASCII/ASCII_numbers.o \
./Modules/Serial_ASCII/Serial.o \
./Modules/Serial_ASCII/Serial_user.o 

MAP += \
RA6_TokenNet_Master.map 


# Each subdirectory must supply rules for building sources it contributes
Modules/Serial_ASCII/%.o: ../Modules/Serial_ASCII/%.c
	@echo 'Building file: $<'
	$(file > $@.in,-mcpu=cortex-m4 -mthumb -mlittle-endian -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -ffunction-sections -fdata-sections -fno-strict-aliasing -fmessage-length=0 -funsigned-char -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Waggregate-return -Wno-parentheses-equality -Wfloat-equal -g3 -std=c99 -fshort-enums -fno-unroll-loops -I"C:\\Users\\artor\\OneDrive\\workspace\\RA6_TokenNet_Master1\\src" -I"." -I"C:\\Users\\artor\\OneDrive\\workspace\\RA6_TokenNet_Master1\\ra\\fsp\\inc" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA6_TokenNet_Master1\\ra\\fsp\\inc\\api" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA6_TokenNet_Master1\\ra\\fsp\\inc\\instances" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA6_TokenNet_Master1\\ra\\arm\\CMSIS_6\\CMSIS\\Core\\Include" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA6_TokenNet_Master1\\ra_gen" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA6_TokenNet_Master1\\Modules\\Scheduler" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA6_TokenNet_Master1\\Modules\\Serial_ASCII" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA6_TokenNet_Master1\\ra_cfg\\fsp_cfg\\bsp" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA6_TokenNet_Master1\\ra_cfg\\fsp_cfg" -D_RENESAS_RA_ -D_RA_CORE=CM4 -D_RA_ORDINAL=1 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -x c "$<" -c -o "$@")
	@clang --target=arm-none-eabi @"$@.in"

