################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra_gen/common_data.c \
../ra_gen/elc_data.c \
../ra_gen/hal_data.c \
../ra_gen/main.c \
../ra_gen/pin_data.c \
../ra_gen/vector_data.c 

CREF += \
RA0_TokenNet_Device.cref 

C_DEPS += \
./ra_gen/common_data.d \
./ra_gen/elc_data.d \
./ra_gen/hal_data.d \
./ra_gen/main.d \
./ra_gen/pin_data.d \
./ra_gen/vector_data.d 

OBJS += \
./ra_gen/common_data.o \
./ra_gen/elc_data.o \
./ra_gen/hal_data.o \
./ra_gen/main.o \
./ra_gen/pin_data.o \
./ra_gen/vector_data.o 

MAP += \
RA0_TokenNet_Device.map 


# Each subdirectory must supply rules for building sources it contributes
ra_gen/%.o: ../ra_gen/%.c
	@echo 'Building file: $<'
	$(file > $@.in,-mcpu=cortex-m23 -mthumb -mlittle-endian -Oz -ffunction-sections -fdata-sections -fno-strict-aliasing -fmessage-length=0 -funsigned-char -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Waggregate-return -Wno-parentheses-equality -Wfloat-equal -g3 -std=c99 -fshort-enums -fno-unroll-loops -I"C:\\Users\\artor\\OneDrive\\workspace\\RA0_TokenNet_Device-0\\src" -I"." -I"C:\\Users\\artor\\OneDrive\\workspace\\RA0_TokenNet_Device-0\\ra\\fsp\\inc" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA0_TokenNet_Device-0\\ra\\fsp\\inc\\api" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA0_TokenNet_Device-0\\ra\\fsp\\inc\\instances" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA0_TokenNet_Device-0\\ra\\arm\\CMSIS_6\\CMSIS\\Core\\Include" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA0_TokenNet_Device-0\\ra_gen" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA0_TokenNet_Device-0\\Modules\\Scheduler" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA0_TokenNet_Device-0\\Modules\\Serial_ASCII" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA0_TokenNet_Device-0\\ra_cfg\\fsp_cfg\\bsp" -I"C:\\Users\\artor\\OneDrive\\workspace\\RA0_TokenNet_Device-0\\ra_cfg\\fsp_cfg" -D_RENESAS_RA_ -D_RA_CORE=CM23 -D_RA_ORDINAL=1 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -x c "$<" -c -o "$@")
	@clang --target=arm-none-eabi @"$@.in"

