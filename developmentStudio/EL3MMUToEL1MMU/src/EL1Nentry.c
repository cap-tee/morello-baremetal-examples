/*
 ============================================================================
 Name        : EL1Nentry.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL1 normal code
 Limitations : The c function library is initialised by the default compile/
 	 	 	   linker to reside in the lower section of DRAM0, which is set
 	 	 	   up as the secure memory location for EL3 in this setup. This
 	 	 	   means that any c function call from the normal world such as
 	 	 	   puts tries to jump to secure memory to run.
   	   	   	   A heap has not yet been setup
 ============================================================================
 */

//need to put into non secure memory region
#define LOCATE_FUNC  __attribute__((__section__(".NONSECUREsection_c")))

#include <stdio.h>
#include <stdlib.h>
extern void el1nmmu(void);

int LOCATE_FUNC el1nmain(void) {
	volatile uint32_t flag = 1;
	//The puts c function is stored in secure memory so can't use it!
	//puts("now in EL1 normal");
	//el1nmmu set up translation tables to use DRAM0, uart, and interrupt controller
	//el1nmmu(); //now called from EL1N boot code before DDC is nulled in purecap mode
	//loop here
	while(flag==1){}

	// We never get here
	return EXIT_SUCCESS;
}
