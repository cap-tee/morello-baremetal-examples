/*
 ============================================================================
 Name        : ChericExample.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2001
 Description : Cheri c example using the cheri c/c++ API for Morello
 ============================================================================
 */


#include <stdio.h>
#include <stdlib.h>

extern int using_cherih(void);
extern int using_cheriintrinh(void);



int main(void) {

	//morello-purecap: test cheri specific functions in pure cap mode
	#ifdef	__CHERI_PURE_CAPABILITY__
	    printf("******************************************************************\n");
		printf("\nPerforming tests in pure capability mode.....\n\n");
		printf("******************************************************************\n\n");

		//testing using the cheri.h file and cheri API
		printf("==================================================================\n");
		printf("Testing with cheri.h API functions.....\n");
		printf("==================================================================\n\n");
		int result_cheri = using_cherih();

		//testing using the cheriintrin.h file and the built in cheri compiler functions
		printf("==================================================================\n");
		printf("\nTesting with cheriintrin.h built-in compiler functions.....\n");
		printf("==================================================================\n\n");
		int result_cheriintrin = using_cheriintrinh();

	//morello: do nothing, print message
	#else
		printf("\nThis program is not running in pure capability mode\n");
		printf("Try changing the settings, and run again! ......\n");
	#endif

	//wait here
	volatile uint32_t flag = 1;
	while(flag==1){}

	return EXIT_SUCCESS;
}
