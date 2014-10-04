/************************************************************************
*
* EUSART2 with Peripheral Pin Select code example for PIC18F46J11.
*
* This example demonstrates how to get the EUSART2 module up 
* and running using the peripheral pin select (PPS) feature
* to map EUSART2 signals to physical microcontroller pins. 
*
* As written, this code configures pins as follows:
* TX2 on RP17/RC6 and RX2 on RP18/RC7. This is a little silly
* because EUSART1 is on those pins, but for demonstration 
* purposes, this allows easy verification that EUSART2 is
* actually functioning. Most Microchip development boards
* connect RC6 and RC7 to RS232 level translators with a convenient
* DB9 serial connector. Your actual application can, of course, 
* configure EUSART2 signals for any other pair of PPS capable pins.
*
* Config Bits Settings:	
*   Watch dog timer is disabled
*   Extended instruction mode is disabled
*   Oscillator is configured as 4MHz INTOSC
*   Fail safe monitor is enabled
*   Internal/External oscillator switch over is enabled	
*
************************************************************************
* FileName:     eusart2_pps.c
* Dependencies: usart.h, pps.h
* Compiler:     MPLAB C18, v3.30 or higher
* Company:      Microchip Technology, Inc.
*
* Software License Agreement
*
* Copyright © 2009 Microchip Technology Inc. All rights reserved.
*
* Microchip licenses to you the right to use, modify, copy and distribute
* Software only when embedded on a Microchip microcontroller or digital
* signal controller, which is integrated into your product or third party
* product (pursuant to the sublicense terms in the accompanying license
* agreement).
*
* You should refer to the license agreement accompanying this Software for
* additional information regarding your rights and obligations.
*
* SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY
* KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY
* WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A
* PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE
* LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY,
* CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY
* DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY
* INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST
* PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY,
* SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO
* ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*/

#include <p18cxxx.h>
#include <usart.h>
#include <pps.h>

#if defined(__18F46J11) || defined(__18F45J11)
    #pragma config OSC = INTOSC
    #pragma config WDTEN = OFF, XINST = OFF
    #pragma config IOL1WAY = OFF // works either way
#else
    #error "Need configuration settings for this microcontroller."
#endif

void xmitc(char data)
{
    while(PIR3bits.TX2IF == 0);         // wait until transmit buffer is ready for data
    TXREG2 = data;
}

void xmits(const rom char* data)
{
    while(*data != 0)
    {
        xmitc(*data++);
    }
}

// BAUDRG is calculated as  = Fosc / (4 * Desired Baudrate) - 1
// So, 4MHz / (4 * 115200) - 1 = 8 (approx.)
#define BAUDRG 8

void main()
{
    OSCCON = 0b01100011;                // switch to 4MHz INTOSC clock source

    PPSUnLock();
    PPSOutput(PPS_RP17, PPS_TX2CK2);    // TX2 RP17/RC6
    PPSInput(PPS_RX2DT2, PPS_RP18);     // RX2 RP18/RC7
    PPSLock();

    Open2USART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_EIGHT_BIT & USART_ASYNCH_MODE & USART_ADDEN_OFF, BAUDRG);
    baud2USART(BAUD_IDLE_TX_PIN_STATE_HIGH & BAUD_IDLE_RX_PIN_STATE_HIGH & BAUD_AUTO_OFF & BAUD_WAKEUP_OFF & BAUD_16_BIT_RATE & USART_RX_INT_ON);

    LATDbits.LATD7 = 1;                 // LED output
    TRISDbits.TRISD7 = 0;   

    TXREG2 = '2';
    xmits(": Hello from PIC EUSART2!\n");

    // now go into echo mode...
    while(1)
    {
        if(PIR3bits.RC2IF)
        {
            xmitc(RCREG2);
            LATDbits.LATD7 ^= 1;
        }
    }
}

