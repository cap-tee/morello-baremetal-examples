/*
 ============================================================================
 Name        : EL1Sentry.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL1 secure code
 ============================================================================
 */
//Known issues:
//1. MMU Tables view in Development Studio gives VSTCR_EL2 error
//Havn't got to the bottom of why the MMU Tables displays VSTCR_EL2 error, since
//the SCR_EL3 EEL2 bit is not set, and register does not exist!


#include <stdio.h>
#include <stdlib.h>
extern void el1smmu(void);

int el1smain(void) {
	volatile uint32_t flag = 1;

	puts("now in EL1 secure");
	//el1smmu set up translation tables to use DRAM0, uart, and interrupt controller
	el1smmu();
	puts("mmu setup in EL1S");


	while(flag==1){}

	// We never get here
	return EXIT_SUCCESS;
}
