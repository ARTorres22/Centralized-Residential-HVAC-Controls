/*
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "hal_data.h"
#include "r_uart_api.h"
#include "Project.h"
#include "Serial_user.h"
#include "Scheduler.h"
#include "common_utils.h"

char int_packet[16];
int my_integer = 75;
int x = 0;

char msg[15];

void R_BSP_WarmStart(bsp_warm_start_event_t event);

int button_pressed1 = 0;
int button_pressed2 = 0;

extern bsp_leds_t g_bsp_leds;


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


    //-----------------------------------------------------
    // Set up system timing

    // Give me a SysTick every 1mS
     SysTick_Config (SystemCoreClock / 1000);


    /* Define the units to be used with the software delay function */
    const bsp_delay_units_t bsp_delay_units = BSP_DELAY_UNITS_MILLISECONDS;

    /* Set the blink frequency (must be <= bsp_delay_units */
    const uint32_t freq_in_hz = 2;

    /* Calculate the delay in terms of bsp_delay_units */
    const uint32_t delay = bsp_delay_units / freq_in_hz;


    //-----------------------------------------------------
    // System initialization

    // Start the UART
     R_SCI_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);

    /* Holds level to set for pins */
    bsp_io_level_t pin_level = BSP_IO_LEVEL_LOW;


    // Send out Test Message
//    SendString("Hello World!", strlen("Hello World!"), StripZeros, NoAddCRLF);
    SendString(DEFAULT_MODEL_NAME,strlen(DEFAULT_MODEL_NAME), StripZeros, NoAddCRLF);
    SendString(PROJ_REV,strlen(PROJ_REV), StripZeros, NoAddCRLF);


//    sprintf(msg, "$01Z%d\n", 1);
//    SendString(msg, strlen(msg), StripZeros, NoAddCRLF);
//
//    sprintf(msg, "$02Z%d\n", 2);
//    SendString(msg, strlen(msg), StripZeros, NoAddCRLF);
//
//    sprintf(msg, "$03Z%d\n", 3);
//    SendString(msg, strlen(msg), StripZeros, NoAddCRLF);
//
//    sprintf(msg, "$04Z%d\n", 4);
//    SendString(msg, strlen(msg), StripZeros, NoAddCRLF);

    //------------------------------------------------------
    // Code Start

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


              // The following line(s) send out a packet each second. The packet means the following:
              // $ = SOP
              // 0 = Incrimentor
              // 1 (or 2) = Target device on Net
              // L = LED command
              // 3 = command value: 0 is off, 1 is on, 3 is Toggle
              // \n = terminator character
              //SendString("$01L3\n", strlen("$L013\n"), StripZeros, NoAddCRLF);

              sprintf(msg, "$01S%d\n", my_integer);


              sprintf(msg, "$01S%d\n", my_integer);
              SendString(msg, strlen(msg), StripZeros, NoAddCRLF);
              // send LED toggle command to node 1
              // SendString("$02L3\n", strlen("$L013\n"), StripZeros, NoAddCRLF);  // send LED toggle command to node 2

              sprintf(msg, "$02S%d\n", my_integer);
              SendString(msg, strlen(msg), StripZeros, NoAddCRLF);

              sprintf(msg, "$03S%d\n", my_integer);
              SendString(msg, strlen(msg), StripZeros, NoAddCRLF);

              sprintf(msg, "$04S%d\n", my_integer);
              SendString(msg, strlen(msg), StripZeros, NoAddCRLF);


              // This is the NON-Token version- start here
              //SendString("$L3\n", strlen("$L3\n"), StripZeros, NoAddCRLF);  // This is the first to try


              // Update the leds
              R_PORT4->PODR_b.PODR3 = pin_level;  //LED1_B
              R_PORT4->PODR_b.PODR0 = pin_level;  //LED2_G
              R_PORT1->PODR_b.PODR0 = pin_level;  //LED3_R

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
            }

//            if (x == 10000000){
//                if(my_integer >= 90){
//                    my_integer = 60;
//                }
//                else{
//                    my_integer++;
//                }
//                x=0;
//            }
//            else{
//                x++;
//            }

            static bsp_io_level_t last_pin_state1 = BSP_IO_LEVEL_HIGH;

            bsp_io_level_t pin_state1;
            R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_00_PIN_08, &pin_state1);

            if (last_pin_state1 == BSP_IO_LEVEL_HIGH && pin_state1 == BSP_IO_LEVEL_LOW)
            {
                R_BSP_SoftwareDelay(50, BSP_DELAY_UNITS_MILLISECONDS);  // 50ms debounce
                R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_00_PIN_08, &pin_state1);
                button_pressed1 = 1;
                if (pin_state1 == BSP_IO_LEVEL_LOW)  // still pressed after debounce
                {
                    if (my_integer < 90)
                    {
                        my_integer++;
                    }
                }
            }
            last_pin_state1 = pin_state1;
            button_pressed1 = 0;

            static bsp_io_level_t last_pin_state2 = BSP_IO_LEVEL_HIGH;

            bsp_io_level_t pin_state2;
            R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_00_PIN_09, &pin_state2);

            if (last_pin_state2 == BSP_IO_LEVEL_HIGH && pin_state2 == BSP_IO_LEVEL_LOW)
            {
                R_BSP_SoftwareDelay(50, BSP_DELAY_UNITS_MILLISECONDS);  // 50ms debounce
                R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_00_PIN_09, &pin_state2);
                button_pressed2 = 1;
                if (pin_state2 == BSP_IO_LEVEL_LOW)  // still pressed after debounce
                {
                    if (my_integer > 60)
                    {
                        my_integer--;
                    }
                }
            }
            last_pin_state2 = pin_state2;
            button_pressed2 = 0;

            // end Every time through the loop
            //--------------------------------

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
