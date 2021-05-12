/*
 ============================================================================
 Name        : EL3MMUUart.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : Outputs a message to the pl011 uart without using c libraries
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "uartS.h"
extern void el3mmu(void);


int main(void) {
	volatile uint32_t flag = 1;
	puts("now in EL3");
	puts("default mmu setup in EL3");
	//el3mmu set up translation tables to use DRAM0, uart, and interrupt controller
	el3mmu();
	puts("new mmu setup in EL3");
	uartSSetup((void*)(0x1C090000)); //set up the memory mapped uart pl011 standard setup at 0x1C090000
	uartSTransmitString("hello world to uart at EL3\n");
    puts("Finished program...\n");
	//loop here
	while(flag==1){}



	return EXIT_SUCCESS;
}
