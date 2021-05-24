/*
 ============================================================================
 Name        : EL1Sentry.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL1 main secure code
 	 	 	   This example sets up the mmu, uart, and a message structure
 	 	 	   in a fixed secure memory location (0xBFFC1000) for passing
 	 	 	   messages to the non secure world.
 	 	 	   This example demonstrates the use of the smc instruction to
 	 	 	   call back into EL3 to pass the message to EL1N. A vector
 	 	 	   table at EL3 catches the exception and processes it.
 	 	 	   A timer interrupt is used to produce a wait to slow the
 	 	 	   program down. This uses the secure physical timer, the
 	 	 	   interrupt controller and an exception vector table at EL1S.
 	 	 	   It uses Secure group 0 (FRQ) for secure EL1.
 	 	 	   It then waits for the interrupt handler to
 	 	 	   complete.

 Known Issues:
 1. MMU Tables view in Development Studio gives VSTCR_EL2 error
 Have not got to the bottom of why the MMU Tables displays VSTCR_EL2 error, since
 the SCR_EL3 EEL2 bit is not set, and the register does not exist!
 ============================================================================
 */

//*****************************************
// DEFINES
//*****************************************
// Define memory region of non secure message to pass to secure world
#define MESG_EL1S_ADDR	 0xBFFC1000
// Set up secure IDs for moving a message between worlds - user defined
// Note: Non secure IDs currently set up for 0x00 to 0x1F, 0x00 not used
//       Secure IDs currently set up for 0x20 to ...
#define SMC_ID_MESGS (0x20) 			  // secure message ID to move char from defined secure memory region

//*****************************************
// INCLUDES
//*****************************************
#include <stdio.h>
#include <stdlib.h>

//Functions to include
#include "uartS.h"
#include "exceptionHandlerFuncsEL1S.h" //include the global flagEL1S flag
#include "mesgEL1S.h" //secure message functions
extern void el1smmu(void);
extern void setTimerTicksS(uint32_t);
extern void enableTimerS(void);
extern void smcPassMesgS(void); //smcEL1S.s

//Main program code of secure EL1
int el1smain(void)
{
	//create a flag for the end wait loop
	volatile uint32_t flag = 1;

	// memory message strings
	char uartstr[8] = {'E', 'L', '1', 'S', '\n', '\0'};
	char uartstr1[8] = {'w', 'a', 'i', 't', '\n', '\0'};
	char mesgReadText[8] = { 'm', 'e', 's', 'g', '\n', '\0'};
	unsigned int mesgRead = 90; //90 is Z
    char mesgReadStr[5] = { mesgRead, '\n', '\0'};
	char mesgWriteText[8] = {'I', 'n', 'c', '1', '\n', '\0'};

	//*****************************************************************
	//SETUP
	//*****************************************************************
	// el1smmu set up translation tables to use DRAM0, uart, and interrupt controller
	el1smmu();

	// set up the memory mapped uart pl011 standard setup at 0x1C090000
	uartSSetup((void*)(0x1C090000));

	// set up non secure memory section to store message to pass to secure
    mesgSSetup_EL1S((void*)(MESG_EL1S_ADDR)); // non secure

    //****************************************************
	// Send message from secure to non secure world
	//****************************************************
	// send message to uart
	uartSTransmitString(uartstr);
	// read current message
	mesgRead = mesgSRead_EL1S();
	// display values in uart window
	// put char into string and write to uart
	mesgReadStr[0] = mesgRead;
	// send message to uart
	uartSTransmitString(mesgReadText);
	uartSTransmitString(mesgReadStr);

	incS_EL1S(1); //increment char by 1 in secure mem
	// read current message
	mesgRead = mesgSRead_EL1S();
	// display values in uart window
	// put char into string and write to uart
	mesgReadStr[0] = mesgRead;
	//send message to uart
	uartSTransmitString(mesgWriteText);
	uartSTransmitString(mesgReadStr);

	//**************************************************
	// Wait for a bit to slow program down.......
	// set up the timer interrupt flag
	flagEL1S = 0;
	uartSTransmitString(uartstr1);
	setTimerTicksS(0xFFFF);  // Generate an interrupt in ticks
	enableTimerS();
	// Wait for the interrupt to arrive
	while(flagEL1S==0){}
	//**************************************************

	// perform an SMC instruction to go to EL3
	smcPassMesgS();
	//****************************************************

	// loop here
	while(flag==1){}

	// We never get here
	return EXIT_SUCCESS;
}
