/***********************************************************************************************************************
 * File Name    : common_utils.h
 * Description  : Contains macros, data structures and functions used  common to the EP
 ***********************************************************************************************************************/
/***********************************************************************************************************************
* Copyright (c) 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/

#ifndef COMMON_UTILS_H_
#define COMMON_UTILS_H_

/* generic headers */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hal_data.h"
/* SEGGER RTT and error related headers */
//#include "SEGGER_RTT/SEGGER_RTT.h"


#define BIT_SHIFT_8  (8u)
#define SIZE_64      (64u)

#define LVL_ERR      (1u)       /* error conditions   */
#define LVL_DEBUG    (3u)       /* debug-level messages */

#define LOG_LEVEL (LVL_ERR)      /* To See the Debug Messages, LOG_LEVEL should be set to LVL_DEBUG */

#define RESET_VALUE             (0x00)

#define EP_VERSION              ("1.0")
#define MODULE_NAME             "adc_tau_interval_sampling"
#define BANNER_INFO	            "\r\n************************************************************************"\
								"\r\n*   Renesas FSP Example Project for "MODULE_NAME" Module   *"\
								"\r\n*   Example Project Version %s                                        *"\
								"\r\n*   Flex Software Pack Version  %d.%d.%d                                  *"\
								"\r\n************************************************************************"\
								"\r\nRefer to readme.txt file for more details on Example Project and" \
        						"\r\nFSP User's Manual for more information about "MODULE_NAME" driver\r\n"

#define SEGGER_INDEX            (0)

#define APP_PRINT(fn_, ...)      (SEGGER_RTT_printf (SEGGER_INDEX,(fn_), ##__VA_ARGS__))

#define APP_ERR_PRINT(fn_, ...)  ({\
                                 if(LVL_ERR)\
                                 SEGGER_RTT_printf (SEGGER_INDEX, "[ERR] In Function: %s(), %s",__FUNCTION__,(fn_),##__VA_ARGS__);\
                                 })

#define APP_ERR_TRAP(err)        ({\
                                    if((err)) {\
                                    SEGGER_RTT_printf(SEGGER_INDEX, "\r\nReturned Error Code: 0x%x  \r\n", (err));\
                                    __asm("BKPT #0\n");} /* trap upon the error  */\
                                    })

#define APP_READ(read_data)     (SEGGER_RTT_Read (SEGGER_INDEX, (read_data), sizeof(read_data)))

#define APP_CHECK_DATA          (SEGGER_RTT_HasKey())

#define APP_DBG_PRINT(fn_, ...) ({\
                                    if(LOG_LEVEL >= LVL_DEBUG)\
                                    SEGGER_RTT_printf (SEGGER_INDEX, "[DBG] In Function: %s(), %s",__FUNCTION__,(fn_),##__VA_ARGS__);\
                                })
#endif /* COMMON_UTILS_H_ */
