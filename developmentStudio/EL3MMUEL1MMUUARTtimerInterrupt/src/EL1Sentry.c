/*
 ============================================================================
 Name        : EL1Sentry.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL1 main secure code
 	 	 	   This example sets up the mmu, uart, and sends a message.
 	 	 	   This example demonstrates a timer interrupt using
 	 	 	   the secure physical timer, the interrupt controller
 	 	 	   and an exception vector table at EL1N.
 	 	 	   It uses Secure group 0 (FRQ) for secure EL1.
 	 	 	   It then waits for the interrupt handler to
 	 	 	   complete.

 Known Issues:
 1. MMU Tables view in Development Studio gives VSTCR_EL2 error
 Havn't got to the bottom of why the MMU Tables displays VSTCR_EL2 error, since
 the SCR_EL3 EEL2 bit is not set, and the register does not exist!
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

//Functions to include
#include "uartS.h"
extern void el1smmu(void);
extern void setTimerTicksS(uint32_t);
extern void startTimerS(void);

//DEFINE GLOBAL FLAG
//define global flag for timer interrupt, this is used by el1smain() and fiqHandlerEL1S()
volatile uint32_t flagEL1S;

//Main program code of secure EL1
int el1smain(void)
{
	//create a flag for the end wait loop
	volatile uint32_t flag = 1;

	puts("now in EL1 secure");
	//el1smmu set up translation tables to use DRAM0, uart, and interrupt controller
	el1smmu();
	puts("mmu setup in EL1S");
	//set up the memory mapped uart pl011 standard setup at 0x1C090000
	uartSSetup((void*)(0x1C090000));
	//send message to uart
    uartSTransmitString("hello world to uart at EL1S\n");

    //set up the timer interrupt flag
	flagEL1S = 0;
	uartSTransmitString("set timer interrupt flag at EL1S\n");
	setTimerTicksS(0x0010);  // Generate an interrupt in ticks
	uartSTransmitString("set timer period at EL1S\n");
	startTimerS();
	uartSTransmitString("enabled the timer at EL1S\n");
	// Wait for the interrupt to arrive
	while(flagEL1S==0){}
	//send message to uart
	uartSTransmitString("Got interrupt at EL1S\n");


    //program wait loop here
	while(flag==1){}

	// We never get here
	return EXIT_SUCCESS;
}
