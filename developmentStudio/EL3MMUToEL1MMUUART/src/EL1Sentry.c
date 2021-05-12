/*
 ============================================================================
 Name        : EL1Sentry.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL1 main secure code

 Known Issues:
 1. MMU Tables view in Development Studio gives VSTCR_EL2 error
 Havn't got to the bottom of why the MMU Tables displays VSTCR_EL2 error, since
 the SCR_EL3 EEL2 bit is not set, and the register does not exist!
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

//Functions to include
#include "uartS.h"
extern void el1smmu(void);

//Main program code of secure EL1
int el1smain(void)
{
	//create a flag for the wait loop
	volatile uint32_t flag = 1;

	puts("now in EL1 secure");
	//el1smmu set up translation tables to use DRAM0, uart, and interrupt controller
	el1smmu();
	puts("mmu setup in EL1S");
	//set up the memory mapped uart pl011 ststandard setup at 0x1C090000
	uartSSetup((void*)(0x1C090000));
	//send message to uart
    uartSTransmitString("hello world to uart at EL1S\n");

    //wait loop here
	while(flag==1){}

	// We never get here
	return EXIT_SUCCESS;
}
