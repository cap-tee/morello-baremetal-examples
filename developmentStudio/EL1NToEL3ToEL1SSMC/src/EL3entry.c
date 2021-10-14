/*
 ============================================================================
 Name        : EL3entry.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL3 main c code
 	 	 	   This example sets up the mmu, vector table, and a message structure
 	 	 	   in both a fixed secure memory location (0xBFFC1000) and a fixed non
 	 	 	   secure memory location (0xBFFC1000) for passing
 	 	 	   messages to the secure and non secure worlds.
 	 	 	   This example demonstrates the use of the smc instruction to
 	 	 	   call back into EL3 to pass the messages back and forth between EL1N
 	 	 	   (normal world) and EL1S (secure world).
 	 	 	   A vector table at EL3 catches the smc exception and processes it.
 	 	 	   Press Interrupt in the debugger to stop the program
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "exceptionHandlerFuncsEL3.h" //include the global flagEL3 flag if using timer
#include "mesgEL3.h" // read/write messages to EL1S/EL1N
#include "uartS.h"

//*****************************************
// DEFINES
//*****************************************
// Define memory region of non secure message
#define MESG_EL1N_ADDR	 0xFFFC1000
// Define memory region of secure message
#define MESG_EL1S_ADDR	 0xBFFC1000

//Functions to include
extern void ERETtoEL1S(void);
extern void ERETtoEL1N(void);
extern void el3mmu(void);

extern void setupGIC600S(void); // for secure timer interrupt
extern void setupGIC600N(void); //for non secure timer set up but goes in EL3 so secure mem
extern void setRegInstallVectors(void);
extern void setTimerTicksS(uint32_t);
extern void startTimerS(void);


//Main program code of secure EL3
int main(void)
{
	// el3mmu set up translation tables to use DRAM0, uart, and interrupt controller
	el3mmu();
	// set up the registers and install the vector table for EL3
	setRegInstallVectors();
	puts("set up EL3 and Installed vector table");

	// set up memory sections for non secure and secure to store character
	mesgNSetup((void*)(MESG_EL1N_ADDR)); // non secure
	mesgSSetup((void*)(MESG_EL1S_ADDR)); // secure

	// PERFORM SOME TESTS - NOT USED
	// test reading and writing to non secure memory location
	//memTestN(3); // test for the first 3 characters of the alphabet
	//memTestS(3); // test for the first 3 characters of the alphabet
	// test moving chars
	// move char from secure to non secure memory location
	// moveStoN();
	// move char from non secure to secure memory location
	// moveNtoS();

	// PASS MESSAGES
	// Initialise memory sections for passing messages to EL3
	// Initialise non secure character to A
	mesgNWrite(65);
	// Initialise secure character to A
	mesgSWrite(65);

	// Start off in the Normal World by default
	// use the timer to slow down the speed
	setupGIC600N(); // need to set registers differently for non secure group 1
	// Perform an ERET to EL1 non secure
	ERETtoEL1N();


    // Or start off in the secure world
	// use the timer to slow down the speed
	//setupGIC600S(); // need to set registers differently for secure group 0
	// Perform an ERET to EL1 secure
	//ERETtoEL1S();


	// should never get here


	// TIMER INTERRUPT AT EL3 NOT USED
	// CODE INCLUDED FOR REFERENCE
	// Initialize memory mapped GIC registers, and enable timer interrupt
	//setupGIC600S();
	//puts("Initialised GIC");
	// set up the timer interrupt
	//flagEL3 = 0;
	//setTimerTicksS(0x0010);  // Generate an interrupt in 1000 ticks
	//startTimerS();
	// Wait for the interrupt to arrive
	//while(flagEL3==0){}
	//puts("got interrupt AT EL3");
	// Infinite loop after interrupt
	//while(flagEL3==1){}

	return EXIT_SUCCESS;
}
