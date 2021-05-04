/*
 ============================================================================
 Name        : MMUEL3.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : Basic translation table for EL3 to include memory and peripheral access
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
extern void el3mmu(void);

int main(void) {
	puts("default mmu setup in EL3");
	//el3mmu set up translation tables to use DRAM0, uart, and interrupt controller
	el3mmu();
	puts("new mmu setup in EL3");
	return EXIT_SUCCESS;
}
