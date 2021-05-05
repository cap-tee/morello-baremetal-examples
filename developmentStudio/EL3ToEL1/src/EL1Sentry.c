/*
 ============================================================================
 Name        : EL1Sentry.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL1 secure code
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

int el1smain(void) {
	volatile uint32_t flag = 1;

	puts("now in EL1 secure");
	while(flag==1){}

	// We never get here
	return EXIT_SUCCESS;
}
