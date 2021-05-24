/*
 ============================================================================
 Name        : uartS.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : secure UART functions
 ============================================================================
 */

//*************************************************************
// INCLUDES
//**************************************************************
#include <stdio.h>
#include <string.h>
// Program defined headers
#include "uartS.h"

//*************************************************************
// DEFINES
//*************************************************************
//------------------------------------------
// Define values for the UART register bits
//-------------------------------------------
//UARTLCR_H - line control register bits used
//Note: bit[4] FIFO not set, so disabled, this means TXFF is set to one when the transmit holding register is full.
//number of data bits transmitted or received in a frame as follows:
#define LCR_H_WLEN   (0x60) //value b11 = 8 bits, bit[6:5] WLEN of UARTLCR_H

//UARTFR - UART flag register bits used
#define FR_TXFF        (0x20) //BIT[5] TXFF - If the FIFO is disabled, this bit is set when the transmit holding register is full.

//UARTICR - UART interrupt clear register bits used
#define ICR_CLR_ALL    (0x07FF) //bits[10:0] - clears all the interrupts

//UARTIBRD - UART integer baud rate register
//38400 bits per second (baud rate)
//See uart technical reference manual for calculations
//used by ARM with 38400 baud rate
#define IBRD_DIV      (0x27) //value - integer part of the baud rate divisor value.
//UARTFBRD - UART fractional baud rate register
#define FBRD_DIV      (0x09) //value - fractional part of the baud rate divisor value.

//UARTCR - UART Control register bits used
#define CR_UARTEN      (0x01) // bit[0] UARTEN - UART enable
#define CR_TXE        (0x0100) //bit[8] TXE - Transmit enable
#define CR_RXE        (0x0200) //bit[9] RXE - Receive enable

//-----------------------------------
// Define standard UART structure
//-----------------------------------
struct uart_struct {
	    //define registers and there offset addr -  see Tech Ref Manual 3.2 Summary of registers
        // names are as defined in the table
		volatile unsigned int UARTDR;        // +0x00 - Data register
        volatile unsigned int UARTECR;       // +0x04 - Error clear register
  const volatile unsigned int reserved0[4];  // +0x08 to +0x14 reserved
  const volatile unsigned int UARTFR;        // +0x18 - RO
  const volatile unsigned int reserved1;     // +0x1C reserved
  	  	volatile unsigned int UARTILPR;      //NOT USED +0x20
        volatile unsigned int UARTIBRD;      // +0x24 - integer baud rate register
        volatile unsigned int UARTFBRD;      // +0x28 - fractional baud rate register
        volatile unsigned int UARTLCR_H;     // +0x2C - line control register
        volatile unsigned int UARTCR;        // +0x30
        volatile unsigned int UARTIFLS;      //NOT USED +0x34
        volatile unsigned int UARTIMSC;      // +0x38 - Interrupt mask set/clear register
        volatile unsigned int UARTRIS;       //NOT USED +0x3C - RO
  const volatile unsigned int UARTMIS;       //NOT USED +0x40 - RO
        volatile unsigned int UARTICR;       // +0x44 - WO - Interrupt Clear Register
        volatile unsigned int UARTDMACR;     // +0x48
};

struct uart_struct* uartS;

//*************************************************************
// Functions
//*************************************************************
//-------------------------------------------------------------
// uartSSetup
// This function sets up the UART
//-------------------------------------------------------------
void uartSSetup(void* addr)
{
  // Create UART
  uartS = (struct uart_struct*) addr;
  // Reset the control register and disable the UART.
  uartS->UARTCR  = 0x0;
  // Reset the error-clear register
  uartS->UARTECR   = 0x0;
  // Reset UARTLCR_H register, and then set the word length to 8 bits
  uartS->UARTLCR_H = 0x0 | LCR_H_WLEN ;
  // Set the integer baud rate register
  uartS->UARTIBRD = IBRD_DIV;
  // Set the fractional baud rate register
  uartS->UARTFBRD = FBRD_DIV;
  // Clear the interrupt mask set/clear register
  uartS->UARTIMSC = 0x0;
  // Clear all the interrupts in the interrupt clear register
  uartS->UARTICR  = ICR_CLR_ALL;  // Clear interrupts
  //Enable the transmit and receive, and the UART
  uartS->UARTCR  = 0x0 | CR_UARTEN | CR_TXE | CR_RXE;
  return;
}

//-----------------------------------------------------------
// uartSTransmitString
// This function transmits a string of characters
//-----------------------------------------------------------
void uartSTransmitString(const char* uartstr)
{
int i; // index
int lengthstr; //length of string

   //output string
   lengthstr = strlen(uartstr);
   for( i = 0 ; i <= lengthstr; i++ )
   {
	   // Wait until transmit holding register has space
	   while ((uartS->UARTFR & FR_TXFF) != 0x0) {}

	   // Write character into transmit holding register
	   uartS->UARTDR = uartstr[i];

	   // Write a carriage return at the end
	   if ((char)uartstr[i] == '\n')
	   {
		 //need to move to start of row because \n just goes onto next line
		 //under same point finished
	     while ((uartS->UARTFR & FR_TXFF) != 0x0) {}
	     uartS->UARTDR = '\r';
	   }
   }

return;
}

