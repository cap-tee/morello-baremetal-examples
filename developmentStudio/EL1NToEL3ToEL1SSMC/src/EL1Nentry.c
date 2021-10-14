/*
 ============================================================================
 Name        : EL1Nentry.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL1 main non secure (normal world) code
 	 	 	   This example sets up the mmu, uart, and a message structure
 	 	 	   in a fixed non secure memory location (0xFFFC1000) for passing
 	 	 	   messages to the secure world.
 	 	 	   This example demonstrates the use of the smc instruction to
 	 	 	   call back into EL3 to pass the message to EL1S. A vector
 	 	 	   table at EL3 catches the exception and processes it.

 	 	 	   A timer interrupt is used to produce a wait to slow the
 	 	 	   program down. It uses the non secure physical timer,
 	 	 	   the interrupt controller and the exception vector table.
 	 	 	   It uses Non-secure group 1 (IRQ) for Non secure EL1.
 	 	 	   It then waits for the interrupt handler to
 	 	 	   complete.

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
//Need to put all non secure code into non secure memory regions
//Attributes are used to define memory sections.
//The linker script places the memory sections into the correct regions.
//Note: Attributes can only be assigned to global variables and functions
#define LOCATE_FUNC  __attribute__((__section__(".NONSECUREsection_c")))
#define LOCATE_STR  __attribute__((__section__(".NONSECUREStringSection_c")))
#define LOCATE_FLAG  __attribute__((__section__(".NONSECUREflagSection_c")))

// Define memory region of non secure message to pass to secure world
#define MESG_EL1N_ADDR	 0xFFFC1000
// Set up Non secure IDs for moving a message between worlds - user defined
// Note: Non secure IDs currently set up for 0x00 to 0x1F, 0x00 not used
//       Secure IDs currently set up for 0x20 to ...
#define SMC_ID_MESGN_INC (0x01) 	 // 16 bit non secure message ID to inc char in EL3 and go back to non secure
#define SMC_ID_MESGN_INCEL1S (0x02) // 16 bit non secure message ID to move char from defined non secure memory region

//*****************************************
// INCLUDES
//*****************************************
#include <stdio.h>
#include <stdlib.h>

// C Functions to include
#include "uartN.h" //non secure uart functions
//#include "exceptionHandlerFuncsEL1N.h" //include the global flagEL1N flag
#include "mesgEL1N.h" // read/write messages to EL1S/EL1N
// Assembly functions to include
extern void el1nmmu(void); //mmu set up
extern void setTimerTicksN(uint32_t);
extern void startTimerN(void); //timerN.s
extern void smcPassMesgN(void); //smcEL1N.s
extern void smcPassMesgN_inc(void); //smcEL1N.s

extern uint32_t flagEL1N; //include the global flagEL1N flag from exceptionHandlerFuncsEL1N.h

// Define string as a global variable to give it an attribute
//can't do this!! as only stores the pointer in non secure memory,
//the string still gets stored in the secure .bss region!!!
//const char* uartStr LOCATE_STR = "hello world to uart at EL1N\n";

//Main program code of non secure EL1
int LOCATE_FUNC el1nmain(void)
{
	//create a flag for the wait loop - to stop program
	volatile uint32_t flag = 1;

	//*****STRINGS***********************************************************
	//even though inside LOCATE_FUNC, string still stored in secure .bss region!!!
	//so can't do this either!
	//const char* uartStr = "hello world to uart at EL1N\n"

	//These three ways seem to be the only way to assign a string locally in the same non secure memory region
	//(for the current set up - where you have one program trying to switch EL)
	// Not Ideal!!! - need to find better method
	//Remember can't use c lib funcs to find the string length
	//char uartstr[24] = {'H', 'e', 'l', 'l', 'o',' ', 'W','o', 'r','l', 'd',' ', 'f','r', 'o','m', ' ', 'E', 'L', '1', 'N','\n', '\0'};
	//char uartstr[24] = "Hello World from EL1N\n";
	//char uartstr[] = "Hello World from EL1N\n\0";
	//*************************************************************************

	// memory message strings
	char uartstr[8] = {'E', 'L', '1', 'N', '\n', '\0'};
	//char uartstr1[12] = {'w', 'a', 'i', 't', '.', '.', '.', ' ','\n', '\0'};
	char uartstr1[8] = {'w', 'a', 'i', 't','\n', '\0'};
	char mesgReadText[8] = { 'm', 'e', 's', 'g', '\n', '\0'};
	unsigned int mesgRead = 90; //90 is Z
    char mesgReadStr[5] = { mesgRead, '\n', '\0'};
	char mesgWriteText[8] = {'I', 'n', 'c', '1', '\n', '\0'};
    //***************************************************************************

	//*****************************************************************
	//SETUP
	//*****************************************************************
	// el1nmmu set up translation tables to use DRAM0, uart, and interrupt controller
	el1nmmu();

	// set up the memory mapped uart pl011 standard setup at 0x1C090000
	uartNSetup((void*)(0x1C090000));

	// set up non secure memory section to store message to pass to secure
	mesgNSetup_EL1N((void*)(MESG_EL1N_ADDR)); // non secure

    //****************************************************
	// Send message from non secure to secure world
	//****************************************************
	// send string to uart
	uartNTransmitString(uartstr);
	// read current message in non secure memory
	mesgRead = mesgNRead_EL1N();
	// display values in uart window
	// put char into string and write to uart
	mesgReadStr[0] = mesgRead;
	// send message to uart
	uartNTransmitString(mesgReadText);
	uartNTransmitString(mesgReadStr);

	// increment char by 1 in memory, e.g A to B
	incN_EL1N(1); //located in smcEL1N.s
	// read current message in memory
	mesgRead = mesgNRead_EL1N();
	// display values in uart window
	// put char into string and write to uart
	mesgReadStr[0] = mesgRead;
	// send message to uart
	uartNTransmitString(mesgWriteText);
	uartNTransmitString(mesgReadStr);

    //**************************************************
	// Wait for a bit to slow program down.......
    // set up the timer interrupt flag
	flagEL1N = 0;
	uartNTransmitString(uartstr1);
	setTimerTicksN(0xFFFF);  // Generate an interrupt in ticks
	// start the timer
	startTimerN();
	// Wait for the interrupt to arrive
	while(flagEL1N==0){}
    //**************************************************

	// perform an SMC instruction to go to EL3
	smcPassMesgN(); //located in smcEL1N.s
	//****************************************************

	// loop here
	while(flag==1){}

	// We never get here
	return EXIT_SUCCESS;
}
