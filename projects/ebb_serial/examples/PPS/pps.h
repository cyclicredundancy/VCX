/* $Id: pps.h,v 1.1.2.2 2008/11/20 09:53:54 shrinivj Exp $ */
#include <p18cxxx.h>
#include <pconfig.h>


/*********************************************************
  * PPS Header File
  * 
  * Header for Pin Re-Mapping peripheral library functions
  *********************************************************/
  
#ifndef __PPS_H
#define __PPS_H

#if	defined (PPS_V1) || defined (PPS_V2)
#define IN_PIN_PPS_RP0				0x0   /* Assign RP0 as Input Pin */
#define IN_PIN_PPS_RP1				0x1   /* Assign RP1 as Input Pin */
#define IN_PIN_PPS_RP2				0x2   /* Assign RP2 as Input Pin */
#define IN_PIN_PPS_RP3				0x3   /* Assign RP3 as Input Pin */
#define IN_PIN_PPS_RP4				0x4   /* Assign RP4 as Input Pin */
#define IN_PIN_PPS_RP5				0x5   /* Assign RP5 as Input Pin */
#define IN_PIN_PPS_RP6				0x6   /* Assign RP6 as Input Pin */
#define IN_PIN_PPS_RP7				0x7   /* Assign RP7 as Input Pin */
#define IN_PIN_PPS_RP8				0x8   /* Assign RP8 as Input Pin */
#define IN_PIN_PPS_RP9				0x9   /* Assign RP9 as Input Pin */
#define IN_PIN_PPS_RP10				0xA   /* Assign RP10 as Input Pin */
#define IN_PIN_PPS_RP11				0xB   /* Assign RP11 as Input Pin */
#define IN_PIN_PPS_RP12				0xC   /* Assign RP12 as Input Pin */
#define IN_PIN_PPS_RP13				0xD   /* Assign RP13 as Input Pin */
#define IN_PIN_PPS_RP17				0x11  /* Assign RP17 as Input Pin */
#define IN_PIN_PPS_RP18				0x12  /* Assign RP18 as Input Pin */
#define IN_PIN_PPS_RP19				0x13  /* Assign RP19 as Input Pin */
#define IN_PIN_PPS_RP20				0x14  /* Assign RP20 as Input Pin */
#define IN_PIN_PPS_RP21				0x15  /* Assign RP21 as Input Pin */
#define IN_PIN_PPS_RP22				0x16  /* Assign RP22 as Input Pin */
#define IN_PIN_PPS_RP23				0x17  /* Assign RP23 as Input Pin */
#define IN_PIN_PPS_RP24				0x18  /* Assign RP24 as Input Pin */
#define IN_PIN_PPS_VSS				0x1F  /* Input Pin tied to Vss */
#define IN_PIN_PPS_MASK             (~(IN_PIN_PPS_VSS |IN_PIN_PPS_RP17))

#define IN_FN_PPS_INT1				RPINR1/* Assign External Interrupt 1 (INTR1) to the corresponding RPn pin*/
#define IN_FN_PPS_INT2				RPINR2/* Assign External Interrupt 2 (INTR2) to the corresponding RPn pin*/
#define IN_FN_PPS_INT3				RPINR3/* Assign External Interrupt 3 (INTR3) to the corresponding RPn pin*/
#define IN_FN_PPS_T0CK				RPINR4/* Assign Timer0 External Clock (T0CK) to the corresponding RPn pin*/
#define IN_FN_PPS_T3CK				RPINR6/* Assign Timer3 External Clock (T3CK) to the corresponding RPn pin*/
#define IN_FN_PPS_IC1				RPINR7/* Assign Input Capture 1 (IC1) to the corresponding RPn pin*/
#define IN_FN_PPS_IC2				RPINR8/* Assign Input Capture 2 (IC2) to the corresponding RPn pin*/
#define IN_FN_PPS_T1G				RPINR12/* Assign Timer1 External Gate Input (TG1) to the corresponding RPn pin*/
#define IN_FN_PPS_T3G				RPINR13/* Assign Timer3 External Gate Input (TG3) to the corresponding RPn pin*/
#define IN_FN_PPS_RX2DT2			RPINR16 /* Assign EUSART2 Synchronous/Asynchronous Receive (RX2/DT2)  to the corresponding RPn pin*/
#define IN_FN_PPS_CK2				RPINR17/* Assign EUSART2 Clock Input (CK2) to the corresponding RPn pin*/
#define IN_FN_PPS_SDI2				RPINR21/* Assign SDI2 Data Input (SDI2) to the corresponding RPn pin*/
#define IN_FN_PPS_SCK2IN			RPINR22/* Assign SCK2 Clock Input (SCK2IN) to the corresponding RPn pin*/
#define IN_FN_PPS_SS2IN				RPINR23/* Assign SS2 Slave Select Input (SS2IN) to the corresponding RPn pin*/
#define IN_FN_PPS_FLT0				RPINR24/* Assign PWM Fault Input (FLT0)  to the corresponding RPn pin*/
#endif

#define iPPSInput(fn,pin)		    fn=pin /*Assign given pin as input pin*/
#define PPSInput(fn,pin)	    	iPPSInput(IN_FN_PPS_##fn,IN_PIN_PPS_##pin)/*Assign given pin as input pin*/

/*----------------------------------------------------------------------------------------------------*/
/*SEction:  Remappable Peripheral Outputs: 																	  */
/*----------------------------------------------------------------------------------------------------*/

#if	defined (PPS_V1) || defined (PPS_V2)
#define OUT_FN_PPS_NULL				0x0  /* RPn tied to default port pin */
#define OUT_FN_PPS_C1OUT			0x1  /* RPn tied to comparator 1 output */
#define OUT_FN_PPS_C2OUT			0x2  /* RPn tied to comparator 2 output */
#define OUT_FN_PPS_TX2CK2			0x5  /* RPn tied to EUSART 2 Asynchronous Transmit / Asynchronous Clock Output */
#define OUT_FN_PPS_DT2              0x6  /* RPn tied to EUSART 2 Synchronous Transmit*/
#define OUT_FN_PPS_SDO2				0x9  /* RPn tied to SPI2 Data Output */
#define OUT_FN_PPS_SCK2 			0xA  /* RPn tied to SPI2 Clock Output */
#define OUT_FN_PPS_SSDMA			0xC  /* RPn tied to SPI DMA Slave Select */
#define OUT_FN_PPS_ULPWU			0xD  /* RPn tied to Ultra Low Power Wake Up Event */
#define OUT_FN_PPS_CCP1P1A			0xE  /* RPn tied to ECCP1/CCP1 compare or PWM output channel A */
#define OUT_FN_PPS_P1B			    0xF  /* RPn tied to ECCP1 Enhanced PWM output, channel B */
#define OUT_FN_PPS_P1C			    0x10  /* RPn tied to ECCP1 Enhanced PWM output, channel C */
#define OUT_FN_PPS_P1D			    0x11  /* RPn tied to ECCP1 Enhanced PWM output, channel D */
#define OUT_FN_PPS_CCP2P2A			0x12  /* RPn tied to ECCP2/CCP2 compare or PWM output */
#define OUT_FN_PPS_P2B			    0x13  /* RPn tied to ECCP2 Enhanced PWM output, channel B */
#define OUT_FN_PPS_P2C			    0x14  /* RPn tied to ECCP2 Enhanced PWM output, channel C */
#define OUT_FN_PPS_P2D			    0x15  /* RPn tied to ECCP2 Enhanced PWM output, channel D */
#define OUT_FN_PPS_MASK             (~(OUT_FN_PPS_P2D |OUT_FN_PPS_P1D))

#define OUT_PIN_PPS_RP0				RPOR0/* Assign RP0 as Output Pin */
#define OUT_PIN_PPS_RP1				RPOR1/* Assign RP1 as Output Pin */
#define OUT_PIN_PPS_RP2				RPOR2/* Assign RP2 as Output Pin */
#define OUT_PIN_PPS_RP3				RPOR3/* Assign RP3 as Output Pin */
#define OUT_PIN_PPS_RP4				RPOR4/* Assign RP4 as Output Pin */
#define OUT_PIN_PPS_RP5				RPOR5/* Assign RP5 as Output Pin */
#define OUT_PIN_PPS_RP6				RPOR6/* Assign RP6 as Output Pin */
#define OUT_PIN_PPS_RP7				RPOR7/* Assign RP7 as Output Pin */
#define OUT_PIN_PPS_RP8				RPOR8/* Assign RP8 as Output Pin */
#define OUT_PIN_PPS_RP9				RPOR9/* Assign RP9 as Output Pin */
#define OUT_PIN_PPS_RP10			RPOR10/* Assign RP10 as Output Pin */
#define OUT_PIN_PPS_RP11			RPOR11/* Assign RP11 as Output Pin */
#define OUT_PIN_PPS_RP12			RPOR12/* Assign RP12 as Output Pin */
#define OUT_PIN_PPS_RP13			RPOR13/* Assign RP13 as Output Pin */
#define OUT_PIN_PPS_RP14			RPOR14/* Assign RP14 as Output Pin */
#define OUT_PIN_PPS_RP15			RPOR15/* Assign RP15 as Output Pin */
#define OUT_PIN_PPS_RP16			RPOR16/* Assign RP16 as Output Pin */
#define OUT_PIN_PPS_RP17			RPOR17/* Assign RP17 as Output Pin */
#define OUT_PIN_PPS_RP18			RPOR18/* Assign RP18 as Output Pin */
#endif

#if	defined (PPS_V2)
#define OUT_PIN_PPS_RP19			RPOR19/* Assign RP19 as Output Pin */
#define OUT_PIN_PPS_RP20			RPOR20/* Assign RP20 as Output Pin */
#define OUT_PIN_PPS_RP21			RPOR21/* Assign RP21 as Output Pin */
#define OUT_PIN_PPS_RP22			RPOR22/* Assign RP22 as Output Pin */
#define OUT_PIN_PPS_RP23			RPOR23/* Assign RP23 as Output Pin */
#define OUT_PIN_PPS_RP24			RPOR24/* Assign RP24 as Output Pin */
#endif

#define iPPSOutput(pin,fn)		    pin=fn /*Assign given pin as output pin*/
#define PPSOutput(fn,pin)		    iPPSOutput(OUT_PIN_PPS_##pin,OUT_FN_PPS_##fn)/*Assign given pin as output pin*/

#define  PPSUnLock()  				{EECON2 = 0b01010101; EECON2 = 0b10101010; PPSCONbits.IOLOCK = 0;} /*Unlock PPS*/
#define  PPSLock() 					{EECON2 = 0b01010101; EECON2 = 0b10101010; PPSCONbits.IOLOCK = 1;} /*Lock PPS*/

#endif /*__PPS_H */
