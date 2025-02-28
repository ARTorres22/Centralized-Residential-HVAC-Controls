################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Modules/Serial_ASCII/ASCII_numbers.c \
../Modules/Serial_ASCII/Serial.c \
../Modules/Serial_ASCII/Serial_user.c 

C_DEPS += \
./Modules/Serial_ASCII/ASCII_numbers.d \
./Modules/Serial_ASCII/Serial.d \
./Modules/Serial_ASCII/Serial_user.d 

OBJS += \
./Modules/Serial_ASCII/ASCII_numbers.o \
./Modules/Serial_ASCII/Serial.o \
./Modules/Serial_ASCII/Serial_user.o 

SREC += \
RA0E1_Class2.srec 

MAP += \
RA0E1_Class2.map 


# Each subdirectory must supply rules for building sources it contributes
Modules/Serial_ASCII/%.o: ../Modules/Serial_ASCII/%.c
	$(file > $@.in,-mcpu=cortex-m23 -mthumb -Oz -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_CORE=CM23 -D_RA_ORDINAL=1 -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/src" -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/Modules/SSD1306/OLED/Inc" -I"." -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/ra/fsp/inc" -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/ra/fsp/inc/api" -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/ra/fsp/inc/instances" -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/ra/arm/CMSIS_6/CMSIS/Core/Include" -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/ra_gen" -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/Modules/Scheduler" -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/Modules/Serial_ASCII" -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/Modules/UX" -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/ra_cfg/fsp_cfg/bsp" -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/ra_cfg/fsp_cfg" -std=c99 -Wno-stringop-overflow -Wno-format-truncation --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

