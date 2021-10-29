/*
 ============================================================================
 Name        : EL3MMUUart.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : Outputs a message to the pl011 uart without using c libraries.
 	 	 	   In purecap mode, a global capability is created in assembly to
 	 	 	   cover the device memory of the UART register space. This is extern
 	 	 	   to c. Note that the assembly code to generate the global capability
 	 	 	   should normally go in the boot code before the DDC is nulled. This
 	 	 	   example is set up to not do that for simplicity of demonstration
 	 	 	   without modifying the default boot code.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "uartS.h"
extern void el3mmu(void);

//purecap global-------------
#include "capfuncs.h"
#include <cheriintrin.h>
// these two funcs should be run in boot code
extern void el3DDCset(void);
extern void setGLOBAL_UART_CAP(void);
// global capability for UART space created in assembly
// see caddr_t _sbrk in syscalls.c where uses extern char* HeapCap, and set up in crt0.S for example on how to set up
extern void* GLOBAL_UART_CAP;



int main(void) {

	#ifdef __CHERI_PURE_CAPABILITY__
		//GLOBAL CAPABILITY SET UP FROM BOOT CODE
		//-------------------------------------------------
		//This would be in boot code before DDC is NULLED
		el3DDCset(); //set ddc //this would be in boot code so not needed
		setGLOBAL_UART_CAP(); //this would go in boot code
		//-------------------------------------------------
		//print capability details
		printf(".............................................\n");
		printf("Capability details of global UART space \n");
		printcapabilityPar(GLOBAL_UART_CAP);
	#endif
		//MMU
		//This would be in boot code before DDC is NULLED for capability mode
		puts("default mmu setup in EL3");
		//el3mmu change translation tables so can access device memory where uart space is
		el3mmu();
		puts("new mmu setup in EL3");

	// start of main code
	#ifdef	__CHERI_PURE_CAPABILITY__
		// capability uart
		// set up the memory mapped uart pl011 standard setup defined by
		// global capability GLOBAL_UART_CAP
		uartScapSetup(GLOBAL_UART_CAP);
		// write a string to the capability uart
		uartScapTransmitString("hello world to capability UART at EL3");
	#else
		//non capability uart
		//set up the memory mapped uart pl011 standard setup at 0x1C090000
		uartSSetup((void*)(0x1C090000));
		// write a string to the standard uart
		uartSTransmitString("hello world to uart at EL3\n");
	#endif

	puts("Finished program...\n");
	//wait here
	volatile uint32_t flag = 1;
	while(flag==1){}

	return EXIT_SUCCESS;
}
