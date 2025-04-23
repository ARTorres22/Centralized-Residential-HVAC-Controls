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

// Prototypes
void R_BSP_WarmStart(bsp_warm_start_event_t event);
static volatile i2c_master_event_t i2c_event = I2C_MASTER_EVENT_ABORTED;
volatile uint8_t i2cBusy = false;
fsp_err_t err     = FSP_SUCCESS;
volatile uint8_t x = 0;
#define OLED_DISPLAY_I2C_BUS_ADDRESS 0x3C

char str1[16];
char str2[16];
char str3[16];
char str4[16];

extern uint16_t tempValue;
uint16_t temp;

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

     g_iica_master0_ctrl.slave = OLED_DISPLAY_I2C_BUS_ADDRESS;

     SSD1306_Init();
     SSD1306_Clear ();
     SSD1306_GotoXY(10,20);
     SSD1306_Puts("Booting...", &Font_11x18, SSD1306_COLOR_WHITE);

// Main Loop Start
    while (1)
    {



        //---------------------------------
        // 10mS Tasks
        if (ten_mS_Flag) {
            ten_mS_Flag = false;

        }  // end of 10mS Tasks
        //---------------------------------


        //---------------------------------
        // 25mS Tasks
        if (twentyfive_mS_Flag) {
            twentyfive_mS_Flag = false;

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

        sprintf(str1, "%u", 0);
        sprintf(str2, "%u", 0);
        sprintf(str3, "%u", 0);
         sprintf(str4, "%u", temp);

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
        else{
            x=0;
        }
        // end Every time through the loop
        //---------------------------------

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

