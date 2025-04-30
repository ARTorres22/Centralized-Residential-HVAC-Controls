/*
 * Serial_user.c
 *
 *  Created on: 25JUN2023
 *      Author: VIn D'Agostino, D'Agostino Industries Group, Inc.
 */

/***********************************************************************************************************************
 * Copyright [2020-2023] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics America Inc. and may only be used with products
 * of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.  Renesas products are
 * sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for the selection and use
 * of Renesas products and Renesas assumes no liability.  No license, express or implied, to any intellectual property
 * right is granted by Renesas. This software is protected under all applicable laws, including copyright laws. Renesas
 * reserves the right to change or discontinue this software and/or this documentation. THE SOFTWARE AND DOCUMENTATION
 * IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND TO THE FULLEST EXTENT
 * PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY, INCLUDING WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE SOFTWARE OR
 * DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.  TO THE MAXIMUM
 * EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR DOCUMENTATION
 * (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER, INCLUDING,
 * WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY LOST PROFITS,
 * OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE POSSIBILITY
 * OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * This file is used with the contents of the "Serial_ASCII" module which is supplied for Renesas use by
 * D'Agostino Industries Group, Inc. It is part of a simple serial interface management system. "<module>_user" files
 * (.c and .h) are intended to be modified on an application by application basis, and <module> files (.c and .h) are
 * intended to be used as provided. <module_user.h is the only file required to be included in referencing files.
 **********************************************************************************************************************/

#include "hal_data.h"

#include "Project.h"

#include "Serial.h"
#include "Serial_user.h"
#include "ASCII_numbers.h"
#include <string.h>
#include <stdio.h>

uint16_t tempValue = 70;

// packet management
uint8_t packetBuffer[PACKET_BUFFER_LENGTH];
uint8_t inPacket = false;
uint8_t nextPacketChar = 0;
uint8_t processPacket = false;

uint8_t processBinaryPacket = false;
int16_t packetLength = -1;
uint8_t escapeDetected = false;

extern uint8_t rxBuffer[RX_BUFFER_SIZE];
extern uint8_t temp;
extern uint8_t zoneNumber;

// Process Variables from other modules/code
bsp_io_level_t pin_level2 = BSP_IO_LEVEL_LOW;


// function to process the input buffer with an ASCII-based protocol
// to be placed in main loop and called of the buffer contains received characters
// not yet processed
// arguments: none
// returns: error code: 0 = no error (default)
uint8_t ProcessReceiveBuffer(void)
{
//  SendString((char const *)&rxBuffer[nextSerialRx2Proc], 1, StripZeros, NoAddCRLF);
  if (rxBuffer[nextSerialRx2Proc] == '$') {
    inPacket = true;
    packetBuffer[0] = rxBuffer[nextSerialRx2Proc];
    nextPacketChar = 1;
  }
  else {
    if (inPacket == true) {
        packetBuffer[nextPacketChar++] = rxBuffer[nextSerialRx2Proc];

        if (rxBuffer[nextSerialRx2Proc] == '\n') {
#if TOKEN_RING
            packetBuffer[COUNTER_POSITION]++;
            if (packetBuffer[COUNTER_POSITION] == packetBuffer[TARGET_POSITION]) {
                processPacket = true;
                inPacket = false;
            }
            else {
                // forward it on to the next node right away
                SendString(packetBuffer, nextPacketChar, StripZeros, NoAddCRLF);
            }
#else
          processPacket = true;
          inPacket = false;
#endif
        }
    }
  }
  
  if (++nextSerialRx2Proc >= RX_BUFFER_SIZE) {
    nextSerialRx2Proc = 0;
  }

  return 0;
}


// Packet processing function. packet form: "$<cmd>[<parameter>]\n"
// for simplicity, commands are single characters.
// Each command is a case in a switch statement. Upper or lower case characters are valid.
uint8_t ProcessPacket(void)
{
    uint8_t errorCode = 0;

    char message[50];


    switch (packetBuffer[COMMAND_LOCATION]) {
        // list of commands
    // each command has intentional fall-thru for upper/lower case
    case 'l':     // L = LED command: 0 = off; 1 = on; 2= toggle
    case 'L':
        switch(packetBuffer[PARAMETER_START_LOCATION]) {
            case '0':
                R_PORT0->PODR_b.PODR9 = 0;
                break;
            case '1':
                R_PORT0->PODR_b.PODR9 = 1;
                break;
            case '3':
                R_PORT0->PODR_b.PODR9 = pin_level2;

                if (BSP_IO_LEVEL_LOW == pin_level2) {
                  pin_level2 = BSP_IO_LEVEL_HIGH;
                }
                else {
                  pin_level2 = BSP_IO_LEVEL_LOW;
                }
                break;

        }
    break;
    case 's':     // s = adjust the flash speed
    case 'S':
        errorCode = ConvertASCII2UINT16((char*)&packetBuffer[PARAMETER_START_LOCATION], 5, '\n', &tempValue);
        temp = tempValue;
        break;


    case 'z':     // s = adjust the flash speed
    case 'Z':
        errorCode = ConvertASCII2UINT16((char*)&packetBuffer[PARAMETER_START_LOCATION], 5, '\n', &tempValue);
        zoneNumber = tempValue;
        break;

    }

    processPacket = false;

    return errorCode;
}

