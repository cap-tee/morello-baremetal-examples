/*
 ============================================================================
 Name        : EL3entry.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL3 main c code
 	 	 	   This example sets up the mmu, and sends a message.
 	 	 	   This example demonstrates a timer interrupt using
 	 	 	   the secure physical timer, the interrupt controller
 	 	 	   and an exception vector table.
 	 	 	   It uses secure group 0 (FRQ) for secure EL3.
 	 	 	   It then waits for the interrupt handler to
 	 	 	   complete before doing an ERET to either EL1N or EL1S
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "exceptionHandlerFuncsEL3.h" //include the global flagEL3 flag

//Functions to include
extern void ERETtoEL1S(void);
extern void ERETtoEL1N(void);
extern void el3mmu(void);

extern void setupGIC600S(void); // for secure timer interrupt
extern void setupGIC600N(void); //for non secure timer set up but goes in EL3 so secure mem
extern void setRegInstallVectors(void);
extern void setTimerTicksS(uint32_t);
extern void startTimerS(void);

//define global flag, this is used by the
//fiqHandlerEL3 function as well to set the flag
volatile uint32_t flagEL3;

//Main program code of secure EL3
int main(void)
{
	puts("default mmu setup in EL3");
	//el3mmu set up translation tables to use DRAM0, uart, and interrupt controller
	el3mmu();
	puts("new mmu setup in EL3");
	//set up the registers and install the vector table for EL3
	setRegInstallVectors();
	puts("Installed vector table");

	// Initialize memory mapped GIC registers, and enable timer interrupt
	setupGIC600S();
	puts("Initialised GIC");
	//set up the timer interrupt
	flagEL3 = 0;
	setTimerTicksS(0x0010);  // Generate an interrupt in 1000 ticks
	startTimerS();
	// Wait for the interrupt to arrive
	while(flagEL3==0){}
	puts("got interrupt AT EL3");
	// Infinite loop after interrupt
	//while(flagEL3==1){}


	// You can only ERET to either secure or non secure (normal), but not both

	// Perform an ERET to EL1 secure
	//don't need to set up gicInitS again as using secure group 0
	//ERETtoEL1S();

	// Perform an ERET to EL1 normal
	setupGIC600N(); //need to set registers differently for non secure group 1
	ERETtoEL1N();
	// Never get here
	return EXIT_SUCCESS;
}
