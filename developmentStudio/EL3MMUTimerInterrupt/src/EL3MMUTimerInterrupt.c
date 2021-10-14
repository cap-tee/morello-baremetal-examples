/*
 ============================================================================
 Name        : EL3MMUTimerInterrupt.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : sets up the mmu at EL3, installs the vector tables, sets up the
  	  	  	   GIC, and performs a timer interrupt
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "exceptionHandlerFuncsEL3.h"
extern void el3mmu(void);
extern void setupGIC600(void);
extern void setRegInstallVectors(void);
extern void setTimerTicks(uint32_t);
extern void startTimer(void);


//define global flag, this is used by the
//fiqHandlerEL3 function as well to set the flag
volatile uint32_t flagEL3;

int main(void) {
	// create a flag for the wait loop - to stop program
	volatile uint32_t flag = 1;
	puts("default mmu setup in EL3");
	// el3mmu set up translation tables to use DRAM0, uart, and interrupt controller
	el3mmu();
	puts("new mmu setup in EL3");
	// set up the registers and install the vector table for EL3
	setRegInstallVectors();
	puts("Installed vector table");
	// Initialize memory mapped GIC registers, and enable timer interrupt
	setupGIC600();
	puts("Initialised GIC");
	// set up the timer interrupt
	flagEL3 = 0;
	setTimerTicks(0x1000);  // Generate an interrupt in 1000 ticks
	startTimer();
	// Wait for the interrupt to arrive
	while(flagEL3==0){}
	puts("got interrupt");
	// Infinite loop after interrupt
	while(flag==1){}
	// We never get here */
	return 0;
}

