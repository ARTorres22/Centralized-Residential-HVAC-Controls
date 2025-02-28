/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "hal_data.h"
#include "Project.h"
#include "Scheduler.h"
#include "Serial_user.h"
//#include "common_utils.h"
#include "hal_data.h"
#include "common_utils.h"
#include "dtc_hal.h"
#include "adc_interval_scan.h"
#include "adc_hal.h"
#include "tau_hal.h"
#include "elc_hal.h"
#include "..\Modules\SSD1306\OLED\Inc\ssd1306.h"
#include "ux_manager.h"
#include "bsp_pin_cfg.h"



// prototypes
void R_BSP_WarmStart(bsp_warm_start_event_t event);
void i2c_master_callback(i2c_master_callback_args_t *p_args);

static void general_signal_acquisition_init (void);
static void handle_error(fsp_err_t err, char *err_str, module_name_t module);

/*FSP Delay loop definition */
/* Define the units to be used with the software delay function */
const bsp_delay_units_t bsp_delay_units = BSP_DELAY_UNITS_MILLISECONDS;

///* Set the blink frequency (must be <= bsp_delay_units */
//const uint32_t freq_in_hz = 2;
//
///* Calculate the delay in terms of bsp_delay_units */
//const uint32_t delay = bsp_delay_units / freq_in_hz;



/*extern variables */
extern uint16_t g_buffer_adc[ADC_UNIT_1][ADC_GROUP_SCAN_NUM][NUM_ADC_CHANNELS][NUM_SAMPLE_BUFFER][NUM_SAMPLES_PER_CHANNEL];
extern transfer_info_t g_transfer_adc_group[];
extern volatile bool g_adc0_group_flag;
extern volatile bool g_err_flag_adc0;


extern bsp_leds_t g_bsp_leds;

//----------------------
// system variables
// Variables for LED flashing
#define INITIAL_FLASH_RATE 40
volatile uint16_t flashDelaySeed = INITIAL_FLASH_RATE;
uint16_t flashDelay = INITIAL_FLASH_RATE;
uint8_t flashEnable = true;

// variables for I2C
fsp_err_t err     = FSP_SUCCESS;
// Sensor
static volatile i2c_master_event_t i2c_event = I2C_MASTER_EVENT_ABORTED;
#define SENSOR_I2C_BUS_ADDRESS 0x44
uint8_t sensorRegisters[6];
const uint8_t cmdRead[2] = {0x24,0x16}; // read temp/hum in Low repeatibility and no clock stretching
//const uint8_t cmdRead[2] = {0x24,0x0B}; // read temp/hum in Med repeatibility and no clock stretching
uint8_t getTempHum = false;
uint8_t getTempHumState = 0;
uint8_t currentTempF = 40;
uint8_t desiredTempF = 70;
uint8_t currentHum = 10;
uint8_t fan = 50;

// EEProm
#define EEPROM_I2C_BUS_ADDRESS 0x50
uint8_t readEEPROM = false;
uint8_t writeEEPROM = false;
uint8_t readEEpromValues[8] = {0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA};
uint8_t writeEEpromValues[8];
uint8_t readWriteStartingRegister[2];
uint8_t eepromReadWriteState = 0;
uint8_t noBytestoRW = 4;
volatile uint8_t i2cBusy = false;
// OLED Display
#define OLED_DISPLAY_I2C_BUS_ADDRESS 0x3C
uint8_t cmdArray[1];


//variables to extend scheduler
#define TEN_SEC_SEED 10
uint8_t tenSecCounter = TEN_SEC_SEED;
uint8_t toggle_200mS = false;

#define BASE_FAN_SPEED 20
#define PROPORTIONAL_GAIN 2

#define BUTTON_PRESSED BSP_IO_LEVEL_LOW
#define BUTTON_DEBOUNCE_DELAY 200


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

   /* LED type structure */
    bsp_leds_t leds = g_bsp_leds;

    /* If this board has no LEDs then trap here */
    if (0 == leds.led_count)
    {
        while (1)
        {
            ;                          // There are no LEDs on this board
        }
    }

    // Give me a SysTick every 1mS
     SysTick_Config (SystemCoreClock / 1000);

     // Start the UART
     R_UARTA_Open(&g_uart0_ctrl, &g_uart0_cfg);

     // Start I2C
     err = R_IICA_MASTER_Open(&g_iica_master0_ctrl, &g_iica_master0_cfg);

    /* Holds level to set for pins */
    bsp_io_level_t pin_level = BSP_IO_LEVEL_LOW;

    // preset some things for read and write EEPROM
    readWriteStartingRegister[0] = 0x00;
    readWriteStartingRegister[1] = 0x04;
    writeEEpromValues[0] =  readWriteStartingRegister[0];
    writeEEpromValues[1] =  readWriteStartingRegister[1];
    writeEEpromValues[2] = 1;
    writeEEpromValues[3] = 2;
    writeEEpromValues[4] = 3;
    writeEEpromValues[5] = 4;


    // *** Analog capture setup ***
    /* Initialize the hal driver's for signal acquisition */
    general_signal_acquisition_init();

    /* start ADC*/
    err = scan_start_adc(&g_adc0_ctrl);
    handle_error(err,"\r\n** start_adc FAILED ** \r\n", ALL);

    /*start tau timer*/
    err = tau_timer_start(&g_timer0_ctrl);
    handle_error(err,"\r\n** start TAU timer FAILED ** \r\n", ALL);

    // Display related init
    g_iica_master0_ctrl.slave = OLED_DISPLAY_I2C_BUS_ADDRESS;

    char buffer[4];  // Enough to store "255\0" (max uint8_t value)
    char buffer2[4];
    char buffer3[4];
    char buffer4[4];

    sprintf(buffer, "%u", currentTempF);  // Convert to string
    sprintf(buffer2, "%u", currentHum);
    sprintf(buffer3, "%u", desiredTempF);
    sprintf(buffer4, "%u%%", fan);

    SSD1306_Init();
    SSD1306_Clear ();
    SSD1306_GotoXY(0,5);
    SSD1306_Puts("Temp", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(10,20);
    SSD1306_Puts(buffer, &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(70,5);
    SSD1306_Puts("Hum", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(80,20);
    SSD1306_Puts(buffer2, &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(0,40);
    SSD1306_Puts("Fan", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(10,50);
    SSD1306_Puts(buffer4, &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(70,40);
    SSD1306_Puts("D Temp", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(80,50);
    SSD1306_Puts(buffer3, &Font_7x10, SSD1306_COLOR_WHITE);

    SSD1306_UpdateScreen();

//    R_BSP_SoftwareDelay(2000, bsp_delay_units);
//    SwitchScreens(MAIN);


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

        }  // end of 10mS Tasks
        //---------------------------------


        //---------------------------------
        // 25mS Tasks
        if (twentyfive_mS_Flag) {
          twentyfive_mS_Flag = false;

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

          if (R_BSP_PinRead(SW) == BUTTON_PRESSED)
          {
              // increment desired temperature
              desiredTempF++;

              while (R_BSP_PinRead(SW) == BUTTON_PRESSED){;}

              // limit the desired temperature to a maximum of 90
              if (desiredTempF > 90)
              {
                  desiredTempF = 60;
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

                      int temp_diff = desiredTempF - currentTempF;
                      fan = BASE_FAN_SPEED + (PROPORTIONAL_GAIN * temp_diff);

                      // Ensure fan percentage is within bounds (0 - 100)
                      if (fan > 100) fan = 100;
                      if (fan < 0) fan = 0;



                      sprintf(buffer, "%d", currentTempF);  // Convert to string
                      sprintf(buffer2, "%d", currentHum);
                      sprintf(buffer3, "%u", desiredTempF);
                      sprintf(buffer4, "%u%%", fan);

                      SSD1306_Fill(SSD1306_COLOR_BLACK);
                      SSD1306_GotoXY(0,5);
                      SSD1306_Puts("Temp", &Font_7x10, SSD1306_COLOR_WHITE);
                      SSD1306_GotoXY(10,20);
                      SSD1306_Puts(buffer, &Font_11x18, SSD1306_COLOR_WHITE);
                      SSD1306_GotoXY(70,5);
                      SSD1306_Puts("Hum", &Font_7x10, SSD1306_COLOR_WHITE);
                      SSD1306_GotoXY(80,20);
                      SSD1306_Puts(buffer2, &Font_11x18, SSD1306_COLOR_WHITE);
                      SSD1306_GotoXY(0,40);
                      SSD1306_Puts("Fan", &Font_7x10, SSD1306_COLOR_WHITE);
                      SSD1306_GotoXY(10,50);
                      SSD1306_Puts(buffer4, &Font_7x10, SSD1306_COLOR_WHITE);
                      SSD1306_GotoXY(70,40);
                      SSD1306_Puts("D Temp", &Font_7x10, SSD1306_COLOR_WHITE);
                      SSD1306_GotoXY(80,50);
                      SSD1306_Puts(buffer3, &Font_7x10, SSD1306_COLOR_WHITE);
                      SSD1306_UpdateScreen();




                      break;
              }
          }




        }  // end of 25mS Tasks
        //---------------------------------


        //---------------------------------
        // 100mS Tasks
        if (hundred_mS_Flag) {
          hundred_mS_Flag = false;



          toggle_200mS ^= true;
          if (toggle_200mS) {
              // update the screen


          }

        }  // end of 100mS Tasks
        //---------------------------------


        //---------------------------------
        // 1 Sec Tasks
        if (one_S_Flag) {
          one_S_Flag = false;

          if (--tenSecCounter == 0) {
              tenSecCounter = TEN_SEC_SEED;

// Disable the next line if you are working on LCD until you have a working mutex
              getTempHum = true;



//              g_iica_master0_ctrl.slave = SENSOR_I2C_BUS_ADDRESS;
//
//              err = R_IICA_MASTER_Write(&g_iica_master0_ctrl, &cmdRead[0], 2, false);
//
//              for (index1 = 0; index1 < 65535; index1++) {
//                  delay2++;
//              }
//
              err = R_IICA_MASTER_Read(&g_iica_master0_ctrl, &sensorRegisters[0], 6, false);
          }

        } // end of 1Sec Tasks
        //---------------------------------


        //---------------------------------
        // Every time through the loop

        // Check to see if a packet has been received
A        if (processPacket == true) {
            processPacket = false;
            ProcessPacket();
        }

        // If the buffer is not empty, process a byte
        if (!RxBufferEmpty()) {
            ProcessReceiveBuffer();
        }

        /* Check if adc 0 scan complete event for ADC group is received */
        if(true == g_adc0_group_flag)
        {
            /*buffer select for ping pong buffer*/
            static uint8_t buffer_select = ZERO;
            buffer_select = (buffer_select == ZERO) ? ONE : ZERO;

            /*update destination address and length for transfer*/
            g_transfer_adc_group[ZERO].p_dest
            = (void*) &g_buffer_adc[ADC_UNIT_0][ADC_GROUP_SCAN][ZERO][buffer_select][ZERO];
//            g_transfer_adc_group[ONE].p_dest
//            = (void*) &g_buffer_adc[ADC_UNIT_0][ADC_GROUP_SCAN][ONE][buffer_select][ZERO];
            g_transfer_adc_group[ZERO].length = NUM_SAMPLES_PER_CHANNEL;
//            g_transfer_adc_group[ONE].length = NUM_SAMPLES_PER_CHANNEL;

            /*Enable transfer*/
            err = dtc_enable(&g_transfer_adc0_group_ctrl);
            handle_error(err,"\r\n** dtc enable failed for adc group  ** \r\n", ALL);

            /*reset the variable */
            g_adc0_group_flag = false;
        }
        /* check if adc scan complete event is not received */
        else if (true == g_err_flag_adc0)
        {
            handle_error(FSP_ERR_ABORTED,"\r\n** adc scan complete event for ADC group is not received  ** \r\n", ALL);
        }
        else
        {
            /* do nothing */
        }
        // end Every time through the loop
        //---------------------------------



    }
}

/*******************************************************************************************************************//**
 * @brief       This functions initializes and enables adc, tau, dtc and elc modules to be used as signal acquisition module.
 * @param[IN]   None
 * @retval      None
 * @retval      Any Other Error code apart from FSP_SUCCESS  Unsuccessful open.
 **********************************************************************************************************************/
static void general_signal_acquisition_init (void)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Initialize all the links in the Event Link Controller */
    err = init_hal_elc(&g_elc_ctrl,&g_elc_cfg);
    if(FSP_SUCCESS != err)
    {
//        APP_ERR_PRINT("\r\n** init_hal_elc FAILED ** \r\n");
//        APP_ERR_TRAP(err);
    }

    /* Initialize DTC instance and reconfigure in chain mode for instance unit, ADC group */
    err = init_hal_dtc(&g_transfer_adc0_group_ctrl, &g_transfer_adc0_group_cfg);
    handle_error(err,"\r\n** dtc_init for unit 0, group a failed ** \r\n", ELC_MODULE);

    err = dtc_hal_reconfigure(&g_transfer_adc0_group_ctrl,  &g_transfer_adc_group[ZERO]);
    handle_error(err,"\r\n** dtc reconfiguration for unit 0, group a failed ** \r\n", ELC_DTC1);

    /* Initialize ADC Unit and configure channels for it*/
    err = init_hal_adc(&g_adc0_ctrl,&g_adc0_cfg);
    handle_error(err,"\r\n** adc_init for unit 0 failed ** \r\n", ELC_DTC_MODULE_ALL);

    err = adc_channel_config(&g_adc0_ctrl, &g_adc0_channel_cfg);
    handle_error(err,"\r\n** adc_channel_config for unit 0 failed ** \r\n", ELC_DTC_ADC0_MODULE);

    /* Initialize TAU timer 0*/
    err = init_hal_tau(&g_timer0_ctrl, &g_timer0_cfg);
    handle_error(err,"\r\n** tau_init for timer 0 failed ** \r\n", ELC_DTC_ADC_MODULE_ALL);

    /* Enable the operation of the Event Link Controller */
    err = elc_enable(&g_elc_ctrl);
    handle_error(err,"\r\n** R_ELC_Enable failed ** \r\n", ALL);

    /*Enable transfers for adc group*/
    err = dtc_enable(&g_transfer_adc0_group_ctrl);
    handle_error(err,"\r\n** dtc_enable for ADC unit 0 group a failed ** \r\n", ALL);

}

/*******************************************************************************************************************//**
 *  @brief       Closes the ELC, DTC, TAU and ADC module, Print and traps error.
 *  @param[IN]   status    error status
 *  @param[IN]   err_str   error string
 *  @param[IN]   module    module to be closed
 *  @retval      None
 **********************************************************************************************************************/
static void handle_error( fsp_err_t err, char *err_str, module_name_t module)
{
    if(FSP_SUCCESS != err)
    {
        switch (module)
        {
            case ELC_MODULE:
            {
                /* close elc instance */
                deinit_hal_elc(&g_elc_ctrl);
            }
            break;
            case ELC_DTC1:
            {
                /* close elc instance */
                deinit_hal_elc(&g_elc_ctrl);

                /* close dtc instance */
                deinit_hal_dtc(&g_transfer_adc0_group_ctrl);
            }
            break;
            case ELC_DTC12:
            {
                /* close elc instance */
                deinit_hal_elc(&g_elc_ctrl);

                /* close dtc instances */
                deinit_hal_dtc(&g_transfer_adc0_group_ctrl);
                //deinit_hal_dtc(&g_transfer_adc0_group_b_ctrl);
            }
            break;

            case ELC_DTC_ADC0_MODULE:
            {
                /* close elc instance */
                deinit_hal_elc(&g_elc_ctrl);

                /* close dtc instances */
                deinit_hal_dtc(&g_transfer_adc0_group_ctrl);

                /* close adc instance */
                deinit_hal_adc(&g_adc0_ctrl);
            }
            break;
            case ELC_DTC_ADC_MODULE_ALL:
            {
                /* close elc instance */
                deinit_hal_elc(&g_elc_ctrl);

                /* close DTC opened instance */
                deinit_hal_dtc(&g_transfer_adc0_group_ctrl);

                /* close adc instance */
                deinit_hal_adc(&g_adc0_ctrl);
            }
            break;
            case ELC_DTC_ADC_TAU_MODULE:
            {
                /* close elc instance */
                deinit_hal_elc(&g_elc_ctrl);

                /* close DTC instances */
                deinit_hal_dtc(&g_transfer_adc0_group_ctrl);

                /*close adc instances*/
                deinit_hal_adc(&g_adc0_ctrl);

                /* close tau instance */
                deinit_hal_tau(&g_timer0_ctrl);
            }
            break;
            case ALL:
            {
                /* close elc instance */
                deinit_hal_elc(&g_elc_ctrl);

                /*close adc instances*/
                deinit_hal_adc(&g_adc0_ctrl);

                /* close TAU 0 instance */
                deinit_hal_tau(&g_timer0_ctrl);

                /* close DTC opened instance */
                deinit_hal_dtc(&g_transfer_adc0_group_ctrl);
            }
            break;
            default:
            {
                /*do nothing */
            }
        }
//        APP_PRINT(err_str);
//        APP_ERR_TRAP(err);
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

/*******************************************************************************************************************//**
 *  @brief      User callback function
 *  @param[in]  p_args
 *  @retval None
 **********************************************************************************************************************/
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


