/*
 ============================================================================
 Name        : uartS.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : secure UART header file for both non capability & capability modes
 ============================================================================
 */

// standard includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// uart includes
#include "uartS.h"
//capability mode includes
#include "capfuncs.h"
//#include <cheriintrin.h>

//--------------------------------------------------------------------
// Define UART structure of capabilities
//--------------------------------------------------------------------
// set up structure of capabilities - do not need to rely on position, each reg set seperately
struct uart_struct_cap {
	    //define registers and there offset addr -  see Tech Ref Manual 3.2 Summary of registers
        volatile unsigned int* UARTDR;        // +0x00 - Data register
        volatile unsigned int* UARTECR;       // +0x04 - Error clear register
//  const volatile unsigned int* reserved0[4];  // +0x08 to +0x14 reserved
  const volatile unsigned int* UARTFR;        // +0x18 - RO
// const volatile unsigned int* reserved1;     // +0x1C reserved
// 	  	volatile unsigned int* UARTILPR;      //NOT USED +0x20
        volatile unsigned int* UARTIBRD;      // +0x24 - integer baud rate register
        volatile unsigned int* UARTFBRD;      // +0x28 - fractional baud rate register
        volatile unsigned int* UARTLCR_H;     // +0x2C - line control register
        volatile unsigned int* UARTCR;        // +0x30
//        volatile unsigned int* UARTIFLS;      //NOT USED +0x34
        volatile unsigned int* UARTIMSC;      // +0x38 - Interrupt mask set/clear register
//        volatile unsigned int* UARTRIS;       //NOT USED +0x3C - RO
//  const volatile unsigned int* UARTMIS;       //NOT USED +0x40 - RO
        volatile unsigned int* UARTICR;       // +0x44 - WO - Interrupt Clear Register
        volatile unsigned int* UARTDMACR;     // +0x48
};
//not a pointer - this is an instance
struct uart_struct_cap uartScap;


//-----------------------------------
// Define UART structure - for normal uart
// registers are defined in order to give correct offset
//-----------------------------------
struct uart_struct {
	    //define registers and there offset addr -  see Tech Ref Manual 3.2 Summary of registers
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
//this is a pointer
struct uart_struct* uartS;

//--------------------------------------------------------------------
// UART functions - purecap
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// uartScapSetup - set up uart with capabilities
// for capability uart
//--------------------------------------------------------------------
void uartScapSetup(void* UARTrootCap)

{
  #ifdef __CHERI_PURE_CAPABILITY__
	//--------------------------------------------------------------------
	// set up a capability for each uart register derived from the
	// root uart capability memory space
	// it is assumed that UARTrootCap is set up to point to the base address
	// of the UART memory space, and it's bound spans all the necessary
	// contiguous UART registers
	//--------------------------------------------------------------------
	// +0x00 - Data register
	uartScap.UARTDR = UARTrootCap;
	printf(".............................................\n");
	printf("UARTDR\n");
	printcapabilityPar((void*)uartScap.UARTDR);
    // +0x04 - Error clear register
	//uartScap.UARTECR = cheri_offset_set(UARTrootCap, UARTECR_offset); //don't need to do this
	uartScap.UARTECR = UARTrootCap+UARTECR_offset;//just increment in purecap mode
	printf(".............................................\n");
	printf("UARTECR\n");
	printcapabilityPar((void*)uartScap.UARTECR);
    // +0x18 - RO
	//uartScap.UARTFR = cheri_offset_set(UARTrootCap, UARTFR_offset);
	uartScap.UARTFR = UARTrootCap+UARTFR_offset;
	printf(".............................................\n");
	printf("UARTFR\n");
	printcapabilityPar((void*)uartScap.UARTFR);
    // +0x24 - integer baud rate register
	//uartScap.UARTIBRD = cheri_offset_set(UARTrootCap, UARTIBRD_offset);
	uartScap.UARTIBRD = UARTrootCap+UARTIBRD_offset;
	printf(".............................................\n");
	printf("UARTIBRD\n");
	printcapabilityPar((void*)uartScap.UARTIBRD);
    // +0x28 - fractional baud rate register
	//uartScap.UARTFBRD = cheri_offset_set(UARTrootCap, UARTFBRD_offset);
	uartScap.UARTFBRD = UARTrootCap+UARTFBRD_offset;
	printf(".............................................\n");
	printf("UARTFBRD\n");
	printcapabilityPar((void*)uartScap.UARTFBRD);
    // +0x2C - line control register
	//uartScap.UARTLCR_H = cheri_offset_set(UARTrootCap, UARTLCR_H_offset);
	uartScap.UARTLCR_H = UARTrootCap+UARTLCR_H_offset;
	printf(".............................................\n");
	printf("UARTLCR_H\n");
	printcapabilityPar((void*)uartScap.UARTLCR_H);
    // +0x30
	//uartScap.UARTCR = cheri_offset_set(UARTrootCap, UARTCR_offset);
	uartScap.UARTCR = UARTrootCap+UARTCR_offset;
	printf(".............................................\n");
	printf("UARTCR\n");
	printcapabilityPar((void*)uartScap.UARTCR);
    // +0x38 - Interrupt mask set/clear register
	//uartScap.UARTIMSC = cheri_offset_set(UARTrootCap, UARTIMSC_offset);
	uartScap.UARTIMSC = UARTrootCap+UARTIMSC_offset;
	printf(".............................................\n");
	printf("UARTIMSC\n");
	printcapabilityPar((void*)uartScap.UARTIMSC);
    // +0x44 - WO - Interrupt Clear Register
	//uartScap.UARTICR = cheri_offset_set(UARTrootCap, UARTICR_offset);
	uartScap.UARTICR = UARTrootCap+UARTICR_offset;
	printf(".............................................\n");
	printf("UARTICR\n");
	printcapabilityPar((void*)uartScap.UARTICR);
	//--------------------------------------------------------------------

	//--------------------------------------------------------------------
	//write to uart registers to set them up
	//--------------------------------------------------------------------
	// Reset the control register and disable the UART.
	*uartScap.UARTCR  = 0x0;
	// Reset the error-clear register
	*uartScap.UARTECR   = 0x0;
	// Reset UARTLCR_H register, and then set the word length to 8 bits
	*uartScap.UARTLCR_H = 0x0 | LCR_H_WLEN ;
	// Set the integer baud rate register
	*uartScap.UARTIBRD = IBRD_DIV;
	// Set the fractional baud rate register
	*uartScap.UARTFBRD = FBRD_DIV;
	// Clear the interrupt mask set/clear register
	*uartScap.UARTIMSC = 0x0;
	// Clear all the interrupts in the interrupt clear register
	*uartScap.UARTICR  = ICR_CLR_ALL;  // Clear interrupts
	//Enable the transmit and receive, and the UART
	*uartScap.UARTCR  = 0x0 | CR_UARTEN | CR_TXE | CR_RXE;
  #endif
}

//--------------------------------------------------------------------
// uartScapTransmitString - write a string of characters to uart terminal
// for capability uart
//--------------------------------------------------------------------
void uartScapTransmitString(const char* uartstr)
{
  #ifdef __CHERI_PURE_CAPABILITY__
	//--------------------------------------------------------------------
	// write string
	// do not use any c lib functions such as strlen
	//--------------------------------------------------------------------
	int i; // index
	//int lengthstr; //length of string

	//output string
	// lengthstr = strlen(uartstr);
	//  for( i = 0 ; i <= lengthstr; i++ )
		i=0;
		while (uartstr[i] != '\0')
	   {
		   // Wait until transmit holding register has space
		   while ((*uartScap.UARTFR & FR_TXFF) != 0x0) {}

		   // Write character into transmit holding register
		   *uartScap.UARTDR = uartstr[i];

		   // Write a carriage return at the end
		   if ((char)uartstr[i] == '\n')
		   {
			 //need to move to start of row because \n just goes onto next line
			 //under same point finished
		     while ((*uartScap.UARTFR & FR_TXFF) != 0x0) {}
		     *uartScap.UARTDR = '\r';
		   }
		   i=i+1;
	   }
  #endif
}

//--------------------------------------------------------------------
// UART functions - for normal uart
//--------------------------------------------------------------------
// ------------------------------------------------------------
// Functions
//-------------------------------------------------------------
//--------------------------------------------------------------------
// uartSSetup
// This function sets up the UART
// for normal uart
//--------------------------------------------------------------------
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
//--------------------------------------------------------------------
// uartSTransmitString - write a string of characters to uart terminal
// for normal uart
// do not use any c lib functions such as strlen
//--------------------------------------------------------------------
void uartSTransmitString(const char* uartstr)
{
	int i; // index
	//int lengthstr; //length of string

	//output string
	// lengthstr = strlen(uartstr);
	//  for( i = 0 ; i <= lengthstr; i++ )
	i=0;
	while (uartstr[i] != '\0')
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
	   i=i+1;
   }

return;
}
