/*
 ============================================================================
 Name        : EL3-purecap.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : Testing in purecap mode. Tests performed at instruction level - assembly
 	 	 	 	Performs bounds and permissions tests and
 				tests store and load capability into memory
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
extern void el3DDCset(void);
extern void el3BoundsTest(void);
extern void el3PermsTest(void);
extern void el3StoreCapTest(void);
extern void el3LoadCapTest(void);

int main(void) {
    // perform tests at the assembly level to understand functionality
	// set default data capability
	el3DDCset();
	// perform bounds test
	el3BoundsTest();
	// perform permissions test
	el3PermsTest();
	// perform store capability from memory
	el3StoreCapTest();
	// perform load capability from memory
	el3LoadCapTest();

	puts("tests completed");
	return EXIT_SUCCESS;
}
