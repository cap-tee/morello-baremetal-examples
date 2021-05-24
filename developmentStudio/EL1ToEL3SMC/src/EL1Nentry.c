/*
 ============================================================================
 Name        : EL1Nentry.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL1 main non secure (normal world) code
  	 	 	   This example sets up the mmu, uart, and a message structure
 	 	 	   in a fixed non secure memory location (0xFFFC1000) for passing
 	 	 	   messages to the secure world - EL3
 	 	 	   This example demonstrates the use of the smc instruction to
 	 	 	   call back into EL3. A vector table at EL3 catches the exception
 	 	 	   and processes it, before returning to EL1N. You can set the number
 	 	 	   of times the messages are sent. The default is currently 5.


 Limitations : The c function library is initialised by the default compile/
 	 	 	   linker to reside in the lower section of DRAM0, which is set
 	 	 	   up as the secure memory location for EL3 in this setup. This
 	 	 	   means that any c function call from the normal world such as
 	 	 	   puts tries to jump to secure memory to run. The set up of the
 	 	 	   MMU enforces the restrictions and will terminate by exception
 	 	 	   for any secure memory access. This code uses the UART to
 	 	 	   output a message.
 ============================================================================
 */

//*****************************************
// DEFINES
//*****************************************
// Need to put all non secure code into non secure memory regions
// Attributes are used to define memory sections.
// The linker script places the memory sections into the correct regions.
// Note: Attributes can only be assigned to global variables and functions
#define LOCATE_FUNC  __attribute__((__section__(".NONSECUREsection_c")))
#define LOCATE_STR  __attribute__((__section__(".NONSECUREStringSection_c")))
#define LOCATE_FLAG  __attribute__((__section__(".NONSECUREflagSection_c")))

// Define memory region of non secure message to pass to secure world
#define MESG_EL1N_ADDR	 0xFFFC1000
// Define the number of messages to pass
#define NUM_MESG 5

//*****************************************
// INCLUDES
//*****************************************
#include <stdio.h>
#include <stdlib.h>

// Functions to include
#include "uartN.h"
#include "mesgEL1N.h" 			// read/write messages
extern void el1nmmu(void);      // defined in el1nmmusetup.s
extern void smcPassMesgN(void); // defined in smcEL1N.s

// Define string as a global variable to give it an attribute
// can't do this!! as only stores the pointer in non secure memory,
// the string still gets stored in the secure .bss region!!!
// const char* uartStr LOCATE_STR = "hello world to uart at EL1N\n";

//*****************************************
// FUNCTIONS
//*****************************************
//-----------------------------------------
// el1nmain
// Main program code of non secure EL1
//-----------------------------------------
int LOCATE_FUNC el1nmain(void)
{
	// create a flag for the wait loop
	volatile uint32_t flag = 1;
	volatile int i; //index for message loop
	// even though inside LOCATE_FUNC, string still stored in secure .bss region!!!
	// so can't do this!
	// const char* uartStr = "hello world to uart at EL1N\n"

	// These three ways seem to be the only way to assign a string locally in the same non secure memory region
	// (for the current set up - where you have one program trying to switch EL)
	// Not Ideal!!!
	// Remember can't use c lib funcs to find the string length
	// char uartstr[24] = {'H', 'e', 'l', 'l', 'o',' ', 'W','o', 'r','l', 'd',' ', 'f','r', 'o','m', ' ', 'E', 'L', '1', 'N','\n', '\0'};
	// char uartstr[24] = "Hello World from EL1N\n";
	// char uartstr[] = "Hello World from EL1N\n\0";

	// memory message strings
	char uartstr[8] = {'E', 'L', '1', 'N', '\n', '\0'};
	char mesgReadText[8] = { 'm', 'e', 's', 'g', '\n', '\0'};
	unsigned int mesgRead = 90; //90 is Z
    char mesgReadStr[5] = { mesgRead, '\n', '\0'};
	char mesgWriteText[8] = {'I', 'n', 'c', '1', '\n', '\0'};

	// SETUP
	// el1nmmu set up translation tables to use DRAM0, uart, and interrupt controller
	el1nmmu();
	// set up the memory mapped uart pl011 standard setup at 0x1C090000
	uartNSetup((void*)(0x1C090000));
	// set up non secure memory section to store message to pass to secure
    mesgNSetup_EL1N((void*)(MESG_EL1N_ADDR)); // non secure

    // PASS MESSAGES
	// messages with secure world EL3
	// pass messages, incrementing a char each time
	for (i = 1; i <=NUM_MESG; i++)
		{
		// set up non secure memory section to store message to pass to EL3
		mesgNSetup_EL1N((void*)(MESG_EL1N_ADDR));
		// send message to uart
		uartNTransmitString(uartstr);
		// read current message
		mesgRead = mesgNRead_EL1N();
		// display values in uart window
		// put char into string and write to console
		mesgReadStr[0] = mesgRead;
		// send message to uart
		uartNTransmitString(mesgReadText);
		uartNTransmitString(mesgReadStr);

		// increment char by 1 in memory
		incN_EL1N(1);

		// read current message
		mesgRead = mesgNRead_EL1N();
		// display values in uart window
		// put char into string and write to console
		mesgReadStr[0] = mesgRead;
		//send message to uart
		uartNTransmitString(mesgWriteText);
		uartNTransmitString(mesgReadStr);

		//perform smc command to el3 to increment char in memory
		smcPassMesgN(); // defined in smcEL1N.s
		}

	//loop here
	while(flag==1){}

	// We never get here
	return EXIT_SUCCESS;
}
