/*
 ============================================================================
 Name        : EL1Nentry.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL1 main non secure (normal world) code
 	 	 	   In this set up EL2 hypervisor controls EL1 memory and page tables.
 	 	 	   EL1 tries to make changes to the memory set up.
 	 	 	   When the EL2 hypervisor disables EL1 from making changes an exception
 	 	 	   occurs which is routed to the EL2 vector table.


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


//*****************************************
// INCLUDES
//*****************************************
#include <stdio.h>
#include <stdlib.h>

// C Functions to include
#include "uartN.h" //non secure uart functions
//#include "exceptionHandlerFuncsEL1N.h" //include the global flagEL1N flag
//#include "mesgEL1N.h" // read/write messages to EL1S/EL1N
// Assembly functions to include
extern void el1nmmu(void); //mmu set up
// function to change MAIR reg
extern void el1MAIRmod(void);
// function to modify the translation table in memory
extern void el1TTmod(void);
// function to change TCR reg
extern void el1TCRmod(void);
// function to change SCTLR reg
extern void el1SCTLRmod(void);

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

	// uart strings
	char uartstr[8] = {'E', 'L', '1', 'N', '\n', '\0'};
	char uartstr2[8] = {'D', 'O', 'N', 'E', '\n', '\0'};
	// EL1N mmu already set up

	// set up the memory mapped uart pl011 standard setup at 0x1C090000
	uartNSetup((void*)(0x1C090000));

	// send string to uart
	uartNTransmitString(uartstr);

    //------------------------------------------------------
	//MESS ABOUT WITH MEMORY - TESTING
	// Try to mess with the EL1 MMU memory registers
	// uncomment tests as needed

	// function to change MAIR reg - try to change memory properties
	// this will cause an MSR MRS exception in EL2 hypervisor if mmu register access is disabled in EL2
	el1MAIRmod();
	uartNTransmitString(uartstr2); //only get here if not disabled

	// function to modify the translation table in memory
	// this will cause an MSR MRS exception in EL2 hypervisor if mmu register access is disabled in EL2
	//el1TTmod();
	//uartNTransmitString(uartstr2); //only get here if not disabled

	// function to change TCR reg - control reg
	// this will cause an MSR MRS exception in EL2 hypervisor if mmu register access is disabled in EL2
	//el1TCRmod();
	//uartNTransmitString(uartstr2); //only get here if not disabled

	// function to change SCTLR reg - try to turn on or off mmu
	// this will cause an MSR MRS exception in EL2 hypervisor if mmu register access is disabled in EL2
    //el1SCTLRmod();
	//uartNTransmitString(uartstr2); //only get here if not disabled



    //------------------------------------------------------

	// loop here
	while(flag==1){}

	// We never get here
	return EXIT_SUCCESS;
}
