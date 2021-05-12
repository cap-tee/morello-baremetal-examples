/*
 ============================================================================
 Name        : EL3entry.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL3 main c code
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

//Functions to include
extern void ERETtoEL1S(void);
extern void ERETtoEL1N(void);
extern void el3mmu(void);

//Main program code of secure EL3
int main(void)
{
	puts("now in EL3");
	puts("default mmu setup in EL3");
	//el3mmu set up translation tables to use DRAM0, uart, and interrupt controller
	el3mmu();
	puts("new mmu setup in EL3");
	// You can only ERET to either secure or non secure (normal), but not both
	// Perform an ERET to EL1 secure
	//ERETtoEL1S();
	// Perform an ERET to EL1 normal
	ERETtoEL1N();
	// Never get here
	return EXIT_SUCCESS;
}
