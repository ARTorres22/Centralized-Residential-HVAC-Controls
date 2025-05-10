/*
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "hal_data.h"
#include "Project.h"
#include "Scheduler.h"
#include "Serial_user.h"
#include "..\Modules\SSD1306\OLED\Inc\ssd1306.h"
#include "common_utils.h"
#include "hal_data.h"
#include "bsp_pin_cfg.h"

#define SERVO_PIN1    BSP_IO_PORT_01_PIN_03
#define SERVO_PIN2    BSP_IO_PORT_01_PIN_09
#define MIN_PULSE_US 2000   // 1ms = full left
#define MAX_PULSE_US 1000   // 2ms = full right

volatile uint16_t pulse_width_us = 1000; // Default: center (1.5ms)

// Prototypes
void R_BSP_WarmStart(bsp_warm_start_event_t event);
void i2c_master_callback(i2c_master_callback_args_t *p_args);
void set_servo_angle(float angle_deg);
void send_servo_pulse(uint16_t pulse_us);


volatile uint8_t i2cBusy = false;
fsp_err_t err     = FSP_SUCCESS;
volatile uint8_t x = 0;
#define OLED_DISPLAY_I2C_BUS_ADDRESS 0x3C

// Sensor
static volatile i2c_master_event_t i2c_event = I2C_MASTER_EVENT_ABORTED;
#define SENSOR_I2C_BUS_ADDRESS 0x44
uint8_t sensorRegisters[6];
const uint8_t cmdRead[2] = {0x24,0x16}; // read temp/hum in Low repeatibility and no clock stretching
//const uint8_t cmdRead[2] = {0x24,0x0B}; // read temp/hum in Med repeatibility and no clock stretching
uint8_t getTempHum = false;
uint8_t getTempHumState = 0;
uint8_t currentTempF = 40;
uint8_t currentHum = 10;
uint16_t desiredTempF = 70;
uint16_t tempcheck = 70;
uint8_t fan = 50;
uint8_t five_seconds = 0;

#define BASE_FAN_SPEED 20
#define PROPORTIONAL_GAIN 2

// EEProm
#define EEPROM_I2C_BUS_ADDRESS 0x50
uint8_t readEEPROM = false;
uint8_t writeEEPROM = false;
uint8_t readEEpromValues[8] = {0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA};
uint8_t writeEEpromValues[8];
uint8_t readWriteStartingRegister[2];
uint8_t eepromReadWriteState = 0;
uint8_t noBytestoRW = 4;

//variables to extend scheduler
#define TEN_SEC_SEED 10
uint8_t tenSecCounter = TEN_SEC_SEED;

#define INITIAL_FLASH_RATE 40
volatile uint16_t flashDelaySeed = INITIAL_FLASH_RATE;
uint16_t flashDelay = INITIAL_FLASH_RATE;
uint8_t flashEnable = true;

uint8_t cmdArray[1];

uint8_t zoneNumber = 0;

char str1[16];
char str2[16];
char str3[16];
char str4[16];

uint16_t temp = 70;


float Kp = 10.0f;    // Proportional gain (adjustable)
float Ki = 0.1f;     // Integral gain (adjustable)
float Kd = 5.0f;     // Derivative gain (adjustable)

float zone;
float target;
float seconds;
float vent;

float prev_error = 0.0f;
float integral = 0.0f;


float clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

float update_vent(float T_room, float T_target, float dt_seconds) {
    float error = T_room - T_target;
    integral += error * dt_seconds;
    float derivative = (error - prev_error) / dt_seconds;
    prev_error = error;

    float output = (Kp * error) + (Ki * integral) + (Kd * derivative);

    // Clamp between 0% and 100%
    return clamp(output, 0.0f, 100.0f);
}

uint8_t float_to_uint8(float value) {
    if (value < 0.0f) return 0;
    if (value > 255.0f) return 255;
    return (uint8_t)(value + 0.5f); // optional rounding
}

static uint8_t prevRegisters[6] = {0};  // Holds previous values

uint8_t y = 0;
uint8_t z = 0;


/*******************************************************************************************************************//**
 * @brief  Blinky example application
 *
 * Blinks all leds at a rate of 1 second using the software delay function provided by the BSP.
 *
 **********************************************************************************************************************/
void hal_entry (void)
{
#if BSP_TZ_SECURE_BUILD

    /* Enter non-secure code */
    R_BSP_NonSecureEnter();
#endif


    // In case we need a software delay...
    /* Define the units to be used with the software delay function */
    const bsp_delay_units_t bsp_delay_units = BSP_DELAY_UNITS_MILLISECONDS;

    /* Set the blink frequency (must be <= bsp_delay_units */
    const uint32_t freq_in_hz = 2;

    /* Calculate the delay in terms of bsp_delay_units */
    const uint32_t delay = bsp_delay_units / freq_in_hz;


    //-----------------------------------------
    // System Variables

    /* Holds level to set for pins */
    bsp_io_level_t pin_level = BSP_IO_LEVEL_LOW;


    // System Init
    // Give me a SysTick every 1mS
     SysTick_Config (SystemCoreClock / 1000);

     // Start the UART
     R_UARTA_Open(&g_uart0_ctrl, &g_uart0_cfg);



     err = R_IICA_MASTER_Open(&g_iica_master0_ctrl, &g_iica_master0_cfg);




     // preset some things for read and write EEPROM
     readWriteStartingRegister[0] = 0x00;
     readWriteStartingRegister[1] = 0x04;
     writeEEpromValues[0] =  readWriteStartingRegister[0];
     writeEEpromValues[1] =  readWriteStartingRegister[1];
     writeEEpromValues[2] = 1;
     writeEEpromValues[3] = 2;
     writeEEpromValues[4] = 3;
     writeEEpromValues[5] = 4;

     g_iica_master0_ctrl.slave = OLED_DISPLAY_I2C_BUS_ADDRESS;

     SSD1306_Init();
     SSD1306_Clear ();
     SSD1306_GotoXY(10,20);
     SSD1306_Puts("Booting...", &Font_11x18, SSD1306_COLOR_WHITE);
     SSD1306_UpdateScreen();

     /* Open IOPORT (GPIO) */
     //R_IOPORT_Open(&g_ioport_ctrl, &g_bsp_pin_cfg);

     /* Start the timer */
     R_TAU_Open(&g_timer0_ctrl, &g_timer0_cfg);
     R_TAU_Start(&g_timer0_ctrl);

     set_servo_angle(90.0f);

// Main Loop Start
    while (1)
    {



        //---------------------------------
        // 10mS Tasks
        if (ten_mS_Flag) {
          ten_mS_Flag = false;

          if (readEEPROM == true) {
              switch(eepromReadWriteState++) {
                  case 0:
                      g_iica_master0_ctrl.slave = EEPROM_I2C_BUS_ADDRESS;
                      err = R_IICA_MASTER_Write(&g_iica_master0_ctrl, (uint8_t *)&readWriteStartingRegister[0], 2, true);
                      break;
                  case 1:
                      err = R_IICA_MASTER_Read(&g_iica_master0_ctrl, &readEEpromValues[0], noBytestoRW, false);
                      break;
                  case 2:
                      readEEPROM = false;
                      eepromReadWriteState = 0;
                      break;
              }
          }

          if (writeEEPROM == true) {
              g_iica_master0_ctrl.slave = EEPROM_I2C_BUS_ADDRESS;
              err = R_IICA_MASTER_Write(&g_iica_master0_ctrl, (uint8_t *)&writeEEpromValues[0], noBytestoRW + 2, false);
              writeEEPROM = false;
          }

          if(z == 1){
              send_servo_pulse(pulse_width_us);
              z=0;
          }
          else{
              z++;
          }

        }  // end of 10mS Tasks
        //---------------------------------


        //---------------------------------
        // 25mS Tasks
        if (twentyfive_mS_Flag) {
          twentyfive_mS_Flag = false;

          getTempHum = true;

          if (flashEnable == true) {
              if (--flashDelay == 0) {
                  flashDelay = flashDelaySeed;
//                  R_BSP_PinAccessEnable();
//
//                  /* Update all board LEDs */
//                  for (uint32_t i = 0; i < leds.led_count; i++)
//                  {
//                      /* Get pin to toggle */
//                      uint32_t pin = leds.p_leds[i];
//
//                      /* Write to this pin */
//                      R_BSP_PinWrite((bsp_io_port_pin_t) pin, pin_level);
//                  }
//
//                  /* Protect PFS registers */
//                  R_BSP_PinAccessDisable();

                  R_PORT0->PODR_b.PODR8 = pin_level;
                  R_PORT0->PODR_b.PODR9 = pin_level;


                  /* Toggle level for next write */
                  if (BSP_IO_LEVEL_LOW == pin_level)
                  {
                      pin_level = BSP_IO_LEVEL_HIGH;
                  }
                  else
                  {
                      pin_level = BSP_IO_LEVEL_LOW;
                  }
              }
          }

          if (getTempHum == true) {
              switch (getTempHumState++) {
                  case 0:
                      g_iica_master0_ctrl.slave = SENSOR_I2C_BUS_ADDRESS;
                      err = R_IICA_MASTER_Write(&g_iica_master0_ctrl, (uint8_t *)&cmdRead[0], 2, false);
                      break;
                  case 1:
                      err = R_IICA_MASTER_Read(&g_iica_master0_ctrl, &sensorRegisters[0], 6, false);
                      break;
                  case 2:
                      currentTempF = (uint8_t)(
                              (uint32_t)(
                              (uint32_t)(
                              (uint32_t)(((uint32_t)sensorRegisters[0] << 8) + sensorRegisters[1])
                              * 315)
                              / 0xFFFF)
                              - 49);

                      currentHum = (uint8_t)(
                              (uint32_t)(
                              (uint32_t)(
                              (uint32_t)(((uint32_t)sensorRegisters[3] << 8) + sensorRegisters[4])
                              *100)
                              /0xFFFF));

                      getTempHum = false;
                      getTempHumState = 0;

                      g_iica_master0_ctrl.slave = OLED_DISPLAY_I2C_BUS_ADDRESS;
                      err = R_IICA_MASTER_Write(&g_iica_master0_ctrl, cmdArray, 1, false);


//                      uint8_t temp_diff = desiredTempF - currentTempF;
//                      fan = BASE_FAN_SPEED + (PROPORTIONAL_GAIN * temp_diff);
//
//                      // Ensure fan percentage is within bounds (0 - 100)
//                      if (fan > 100) fan = 100;
//                      if (fan < 0) fan = 0;

                      desiredTempF = temp;



                      fan = float_to_uint8(vent);


                      if(desiredTempF != tempcheck){
                          tempcheck = desiredTempF;
                      }

                      sprintf(str1, "%d", currentTempF);  // Convert to string
                      sprintf(str2, "%d", currentHum);
                      sprintf(str4, "%d", tempcheck);
                      sprintf(str3, "%u%%", fan);




                      bool allMatch = true;


                      for (int i = 0; i < 6; ++i) {
                          if (sensorRegisters[i] != prevRegisters[i]) {
                              allMatch = false;
                              y = 0;
                              break;
                          }
                      }

                      if (allMatch) {
                          y++;
                          if(y == 10){

                              SSD1306_Fill(SSD1306_COLOR_BLACK);
                              SSD1306_GotoXY(0,5);
                              SSD1306_Puts("Desired Temp", &Font_7x10, SSD1306_COLOR_WHITE);
                              SSD1306_GotoXY(10,20);
                              SSD1306_Puts(str4, &Font_16x26, SSD1306_COLOR_WHITE);
                              SSD1306_UpdateScreen();
                          }
                      }
                      else{

                          SSD1306_Fill(SSD1306_COLOR_BLACK);
                          SSD1306_GotoXY(0,5);
                          SSD1306_Puts("Temp", &Font_7x10, SSD1306_COLOR_WHITE);
                          SSD1306_GotoXY(10,20);
                          SSD1306_Puts(str1, &Font_11x18, SSD1306_COLOR_WHITE);
                          SSD1306_GotoXY(70,5);
                          SSD1306_Puts("Hum", &Font_7x10, SSD1306_COLOR_WHITE);
                          SSD1306_GotoXY(80,20);
                          SSD1306_Puts(str2, &Font_11x18, SSD1306_COLOR_WHITE);
                          SSD1306_GotoXY(0,40);
                          SSD1306_Puts("Fan", &Font_7x10, SSD1306_COLOR_WHITE);
                          SSD1306_GotoXY(10,50);
                          SSD1306_Puts(str3, &Font_7x10, SSD1306_COLOR_WHITE);
                          SSD1306_GotoXY(70,40);
                          SSD1306_Puts("D Temp", &Font_7x10, SSD1306_COLOR_WHITE);
                          SSD1306_GotoXY(80,50);
                          SSD1306_Puts(str4, &Font_7x10, SSD1306_COLOR_WHITE);
                          SSD1306_UpdateScreen();
                      }

                      // Update prevRegisters for next check
                      for (int i = 0; i < 6; ++i) {
                          prevRegisters[i] = sensorRegisters[i];
                      }








                      break;
              }
          }



        }  // end of 25mS Tasks
        //---------------------------------


        //---------------------------------
        // 100mS Tasks
        if (hundred_mS_Flag) {
            hundred_mS_Flag = false;

        }  // end of 100mS Tasks
        //---------------------------------


        //---------------------------------
        // 1 Sec Tasks
        if (one_S_Flag) {
            one_S_Flag = false;


            R_PORT0->PODR_b.PODR8 = pin_level;
            //R_PORT0->PODR_b.PODR9 = pin_level;

            /* Toggle level for next write */
            if (BSP_IO_LEVEL_LOW == pin_level)
            {
              pin_level = BSP_IO_LEVEL_HIGH;

    }
            else
            {
              pin_level = BSP_IO_LEVEL_LOW;

            }

            if (--tenSecCounter == 0) {
                tenSecCounter = TEN_SEC_SEED;



  //              g_iica_master0_ctrl.slave = SENSOR_I2C_BUS_ADDRESS;
  //
  //              err = R_IICA_MASTER_Write(&g_iica_master0_ctrl, &cmdRead[0], 2, false);
  //
  //              for (index1 = 0; index1 < 65535; index1++) {
  //                  delay2++;
  //              }
  //
  //              err = R_IICA_MASTER_Read(&g_iica_master0_ctrl, &sensorRegisters[0], 6, false);
            }

            if (five_seconds == 4){
                zone = (float)currentTempF;
                target = (float)desiredTempF;
                seconds = 5.0f;

                vent = update_vent(zone, target, seconds);
                set_servo_angle(vent);
                five_seconds = 0;
            }
            else{
                five_seconds++;
            }
        } // end of 1Sec Tasks
        //---------------------------------


        //---------------------------------
        // Every time through the loop

        // Check to see if a packet has been received
        if (processPacket == true) {
            processPacket = false;
            ProcessPacket();
        }

        // If the buffer is not empty, process a byte
        if (!RxBufferEmpty()) {
            ProcessReceiveBuffer();



        }
        else{
            x=0;
        }


    }
}

/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/
void R_BSP_WarmStart (bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_RESET == event)
    {
#if BSP_FEATURE_FLASH_LP_VERSION != 0

        /* Enable reading from data flash. */
        R_FACI_LP->DFLCTL = 1U;

        /* Would normally have to wait tDSTOP(6us) for data flash recovery. Placing the enable here, before clock and
         * C runtime initialization, should negate the need for a delay since the initialization will typically take more than 6us. */
#endif
    }

    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        R_IOPORT_Open(&IOPORT_CFG_CTRL, &IOPORT_CFG_NAME);
    }
}


void i2c_master_callback(i2c_master_callback_args_t *p_args)
{
    if (NULL != p_args)
    {
        /* capture callback event for validating the i2c transfer event*/
        i2c_event = p_args->event;
        if (i2c_event == I2C_MASTER_EVENT_TX_COMPLETE) {
            i2cBusy = false;
        }
    }
}

void timer0_callback(timer_callback_args_t *p_args)
{
    /* Start the PWM pulse */
    R_IOPORT_PinWrite(&g_ioport_ctrl, SERVO_PIN1, BSP_IO_LEVEL_HIGH);
    R_IOPORT_PinWrite(&g_ioport_ctrl, SERVO_PIN2, BSP_IO_LEVEL_HIGH);

    /* Delay for pulse width (in microseconds) */
    R_BSP_SoftwareDelay(pulse_width_us, BSP_DELAY_UNITS_MICROSECONDS);

    /* End the PWM pulse */
    R_IOPORT_PinWrite(&g_ioport_ctrl, SERVO_PIN1, BSP_IO_LEVEL_LOW);
    R_IOPORT_PinWrite(&g_ioport_ctrl, SERVO_PIN2, BSP_IO_LEVEL_LOW);
}

void set_servo_angle(float angle_deg)
{
    if (angle_deg < 0.0f) angle_deg = 0.0f;
    if (angle_deg > 180.0f) angle_deg = 180.0f;

    pulse_width_us = (uint16_t)(MIN_PULSE_US + (angle_deg / 180.0f) * (MAX_PULSE_US - MIN_PULSE_US));
}

void send_servo_pulse(uint16_t pulse_us)
{
    R_IOPORT_PinWrite(&g_ioport_ctrl, SERVO_PIN1, BSP_IO_LEVEL_HIGH);
    R_IOPORT_PinWrite(&g_ioport_ctrl, SERVO_PIN2, BSP_IO_LEVEL_HIGH);
    R_BSP_SoftwareDelay(pulse_us, BSP_DELAY_UNITS_MICROSECONDS);
    R_IOPORT_PinWrite(&g_ioport_ctrl, SERVO_PIN1, BSP_IO_LEVEL_LOW);
    R_IOPORT_PinWrite(&g_ioport_ctrl, SERVO_PIN2, BSP_IO_LEVEL_LOW);
}
