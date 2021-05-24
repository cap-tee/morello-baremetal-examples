/*
 ============================================================================
 Name        : EL3entry.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL3 main c code
 	 	 	   This example sets up the mmu, uart, and a message structure
 	 	 	   in a fixed secure memory location (0xBFFC1000) and a fixed
 	 	 	   non secure memory location (0xFFFC1000) for passing messages
 	 	 	   from EL1N/EL1S to EL3 and back
 	 	 	   This example demonstrates the use of the smc instruction to
 	 	 	   call back into EL3. A vector table at EL3 catches the
 	 	 	   exception and processes it, before returning to EL1S or EL1N.
 ============================================================================
 */

//*****************************************
// INCLUDES
//*****************************************
#include <stdio.h>
#include <stdlib.h>
#include "exceptionHandlerFuncsEL3.h"
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
extern void setRegInstallVectors(void);

//*****************************************
// FUNCTIONS
//*****************************************
//-----------------------------------------
// main
// Main program code of non secure EL1
//-----------------------------------------
int main(void)
{

	unsigned int mesg = 65; //65 is A
	unsigned int mesg2 = 90;
    char mesgcount[5] = { mesg, '\n', '\0'};
    char mesgcount2[5] = { mesg2, '\n', '\0'};

    // SETUP
	// el3mmu set up translation tables to use DRAM0, uart, and interrupt controller
	el3mmu();
	// set up the registers and install the vector table for EL3
	setRegInstallVectors();
	puts("Performed set up in EL3 and Installed vector table");

	// set up memory sections for non secure and secure to store character
	mesgNSetup((void*)(MESG_EL1N_ADDR)); // non secure
	mesgSSetup((void*)(MESG_EL1S_ADDR)); // secure

	// PERFORM SOME TESTS
	// test reading and writing to non secure memory location
	memTestN(3); // test for the first 3 characters of the alphabet
	memTestS(3); // test for the first 3 characters of the alphabet
	// test moving chars
	//move char from secure to non secure memory location
	moveStoN();
	//move char from non secure to secure memory location
	moveNtoS();

	// PASS MESSAGES
	// Initialise memory sections for passing messages to EL3
	//Initialise non secure character to A
	mesgNWrite(65);
	//Initialise secure character to A
	mesgSWrite(65);

	// Go to Normal World and pass messages to and from EL3 only
	// Perform an ERET to EL1 normal
	ERETtoEL1N();

    // Or goto secure world and pass messages to and from EL3 only
	// Perform an ERET to EL1 secure
	//ERETtoEL1S();

	// Never get here
	return EXIT_SUCCESS;
}
