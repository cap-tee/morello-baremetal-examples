/*
 ============================================================================
 Name        : EL3entry.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL3 secure code
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
extern void ERETtoEL1S(void);
extern void ERETtoEL1N(void);

int main(void) {
	puts("now in EL3");
	// You can only ERET to either secure or non secure (normal), but not both
	// Perform an ERET to EL1 secure
	//ERETtoEL1S();
	// Perform an ERET to EL1 normal
	ERETtoEL1N();
	// Never get here
	return EXIT_SUCCESS;
}
