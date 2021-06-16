/*
 ============================================================================
 Name        : EL3entry.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL3 main c code
 	 	 	   This example sets up the mmu, and uart and performs an ERET
			   to EL2
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "uartS.h"

//*****************************************
// DEFINES
//*****************************************

//Functions to include
extern void ERETtoEL2N(void);
extern void el3mmu(void);


//Main program code of secure EL3
int main(void)
{
	// uart strings
	char uartstr[8] = {'E', 'L', '3', ' ', '\n', '\0'};

	// el3mmu set up translation tables to use DRAM0, and device memory
	el3mmu();

	// set up the memory mapped uart pl011 standard setup at 0x1C090000
	uartSSetup((void*)(0x1C090000));

	// send string to uart
	uartSTransmitString(uartstr);

	// Perform an ERET to EL2 hypervisor (non secure)
	ERETtoEL2N();


	return EXIT_SUCCESS;
}
