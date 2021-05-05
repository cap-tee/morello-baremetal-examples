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
 	 	 	   puts jumps to secure memory to run. This is not a realistic
 	 	 	   or secure scenario in the current set up.
 ============================================================================
 */

//need to put into non secure memory region
#define LOCATE_FUNC  __attribute__((__section__(".NONSECUREsection_c")))

#include <stdio.h>
#include <stdlib.h>


int LOCATE_FUNC el1nmain(void) {
	volatile uint32_t flag = 1;
	//The puts c function is stored in secure memory though, so wouldn't really be able to do this!
	puts("now in EL1 normal");
	while(flag==1){}

	// We never get here
	return EXIT_SUCCESS;
}
