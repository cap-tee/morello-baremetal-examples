/*
 ============================================================================
 Name        : EL1Sentry.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL1 main secure code
 	 	 	   This example sets up the mmu, uart, and a message structure
 	 	 	   in a fixed secure memory location (0xBFFC1000) for passing
 	 	 	   messages to the secure world - EL3
 	 	 	   This example demonstrates the use of the smc instruction to
 	 	 	   call back into EL3. A vector table at EL3 catches the exception
 	 	 	   and processes it, before returning to EL1S.'You can set the number
 	 	 	   of times the messages are sent. The default is currently 5.

 Known Issues:
 1. MMU Tables view in Development Studio gives VSTCR_EL2 error
 Havn't got to the bottom of why the MMU Tables displays VSTCR_EL2 error, since
 the SCR_EL3 EEL2 bit is not set, and the register does not exist!
 ============================================================================
 */

//*****************************************
// DEFINES
//*****************************************
// Define memory region of secure message
#define MESG_EL1S_ADDR	 0xBFFC1000
// Define the number of messages to pass
#define NUM_MESG 5

//*****************************************
// INCLUDES
//*****************************************
#include <stdio.h>
#include <stdlib.h>

//#include "exceptionHandlerFuncsEL1S.h" //include the global flagEL1S flag
//Functions to include
#include "uartS.h"
#include "mesgEL1S.h"           // read/write messages
extern void el1smmu(void);      // defined in el1smmusetup.s
extern void smcPassMesgS(void); // defined in smcEL1N.s

//*****************************************
// FUNCTIONS
//*****************************************
//-----------------------------------------
// el1smain
// Main program code of secure EL1
//-----------------------------------------
int el1smain(void)
{
	//create a flag for the wait loop
	volatile uint32_t flag = 1;
	volatile int i; //index for message loop

	// memory message strings
	char uartstr[8] = {'E', 'L', '1', 'S', '\n', '\0'};
	char mesgReadText[8] = { 'm', 'e', 's', 'g', '\n', '\0'};
	unsigned int mesgRead = 90; //90 is Z
    char mesgReadStr[5] = { mesgRead, '\n', '\0'};
	char mesgWriteText[8] = {'I', 'n', 'c', '1', '\n', '\0'};

	// SETUP
	// el1smmu set up translation tables to use DRAM0, uart, and interrupt controller
	el1smmu();
	// set up the memory mapped uart pl011 standard setup at 0x1C090000
	uartSSetup((void*)(0x1C090000));
	// set up secure memory section to store message to pass to EL3
    mesgSSetup_EL1S((void*)(MESG_EL1S_ADDR)); // non secure

    // PASS MESSAGES
	// messages with secure world EL3
	// pass messages, incrementing a char each time
	for (i = 1; i <=NUM_MESG; i++)
		{
		// send message to uart
		uartSTransmitString(uartstr);
		// read current message
		mesgRead = mesgSRead_EL1S();
		// display values in uart window
		// put char into string and write to console
		mesgReadStr[0] = mesgRead;
		//send message to uart
		uartSTransmitString(mesgReadText);
		uartSTransmitString(mesgReadStr);

		//increment char by 1 in memory
		incS_EL1S(1);

		//read current message
		mesgRead = mesgSRead_EL1S();
		// display values in uart window
		// put char into string and write to console
		mesgReadStr[0] = mesgRead;
		//send message to uart
		uartSTransmitString(mesgWriteText);
		uartSTransmitString(mesgReadStr);

		//perform smc command to el3 to increment char in memory
		smcPassMesgS(); // defined in smcEL1S.s
		}

	//loop here
	while(flag==1){}

	// We never get here
	return EXIT_SUCCESS;
}
