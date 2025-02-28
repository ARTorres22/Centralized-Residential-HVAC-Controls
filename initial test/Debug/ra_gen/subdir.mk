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

SREC += \
RA0E1_Class2.srec 

MAP += \
RA0E1_Class2.map 


# Each subdirectory must supply rules for building sources it contributes
ra_gen/%.o: ../ra_gen/%.c
	$(file > $@.in,-mcpu=cortex-m23 -mthumb -Oz -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_CORE=CM23 -D_RA_ORDINAL=1 -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/src" -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/Modules/SSD1306/OLED/Inc" -I"." -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/ra/fsp/inc" -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/ra/fsp/inc/api" -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/ra/fsp/inc/instances" -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/ra/arm/CMSIS_6/CMSIS/Core/Include" -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/ra_gen" -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/Modules/Scheduler" -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/Modules/Serial_ASCII" -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/Modules/UX" -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/ra_cfg/fsp_cfg/bsp" -I"C:/Users/ddmga/OneDrive/workspace/RA0E1_Class2/ra_cfg/fsp_cfg" -std=c99 -Wno-stringop-overflow -Wno-format-truncation --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

