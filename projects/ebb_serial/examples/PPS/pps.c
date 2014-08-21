/**********************************************************************
* © 2009 Microchip Technology Inc.
*
* FileName:        pps.c
* Dependencies:    pps.h (comes with MCC18)
* Processor:       PIC18xxxx
* Compiler:        MPLAB C18, v3.30 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and 
* intellectual property rights in the code accompanying this message and in all 
* derivatives hereto.  You may use this code, and any derivatives created by 
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes 
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify, test, 
* certify, or support the code.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Trey L 			04/07/09  First release of source file
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* This example sets up the SPI2 module as a slave to the RP22, 23, and 24 PPS pins.
* It then sends data from the SPI1 module set up as a master to the SPI2 module.
* After this is completed, the SPI2 module is changed to be the master and the
* PPS directions are changed accordingly.  The SPI1 module is changed to slave mode,
* and the SPI2 then initiates the data transfer.  Each breakpoint should show that
* the SSP2BUF and SSP1BUF have switched values.
*
* In order for this example to work, the following pins must be connected:
*      SCK1 (RB4) to RP24 (RD7)
*      SDI1 (RB5) to RP23 (RD6)
*      SDO1 (RC7) to RP22 (RD5)
* 
* Code Tested on:
* Universal Programming Module, DIP Board with PIC18F46J50
**********************************************************************/

#include <p18cxxx.h>
#include <pps.h>

#pragma config OSC = EC, WDTEN = OFF, XINST = OFF, IOL1WAY = OFF

void main(void){
	// Assign SPI2 module to RP22, 23, and 24
	PPSInput(SCK2IN, RP24);	// Set SCK2 as input at RP24
	PPSInput(SDI2, RP22);	// Set SDI2 as input at RP22
	PPSOutput(SDO2, RP23);	// Set SDO2 as output at RP23
	PPSLock()				// Lock pins
	
	// Init SPI modules 1 and 2
	TRISBbits.TRISB4 = 0;	// SCK1 is output
	TRISCbits.TRISC7 = 0;	// SDO1 is output
	TRISDbits.TRISD6 = 0;	// SDO2 is output
	TRISDbits.TRISD7 = 1;	// SCK2 is input
	TRISBbits.TRISB5 = 1;	// SDI1 is input
	TRISDbits.TRISD5 = 1;	// SDI2 is input
	
	// Initialize SPI1 as master and SPI2 as slave
	SSP1STAT = SSP2STAT = 0x00;
	SSP2CON1 = 0x25;
	SSP1CON1 = 0x20;
	
	// Send 0x55 from SPI2 to SPI1 module and 0xAA from SPI1 to SPI2
	SSP2BUF = 0x55;
	SSP1BUF = 0xAA;
	while(~SSP1STATbits.BF || ~SSP2STATbits.BF);
	
	Nop();		// Set breakpoint here to make sure SSP2BUF and SSP1BUF have switched values
	
	// Disable SPI modules
	SSP1CON1bits.SSPEN = 0;
	SSP2CON2bits.SSPEN = 0;
	
	// Change directions of pins so SPI2 module can be master and SPI1 can be slave
	PPSUnLock()				// Unlock pins
	PPSOutput(SCK2, RP24);	// Set SCK2 as output on RP24 (and keep it as an input, too, since master)
	PPSLock()				// Lock pins
	
	TRISDbits.TRISD7 = 0;	// SCK2 is output
	TRISBbits.TRISB4 = 1;	// SCK1 is input
	
	// Switch SPI1 to slave and SPI2 to master
	SSP2CON1 = 0x20;
	SSP1CON1 = 0x25;
	
	SSP1BUF = 0x88;
	SSP2BUF = 0x11;
	while(~SSP1STATbits.BF || ~SSP2STATbits.BF);
	
	Nop();		// Set breakpoint here to make sure SSP2BUF and SSP1BUF have switched values
	
	while(1);
}
