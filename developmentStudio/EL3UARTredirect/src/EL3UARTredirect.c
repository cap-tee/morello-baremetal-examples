/*
 ============================================================================
 Name        : EL3UARTredirect.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : testing re-direct of printf function to uart
 	 	 	   so can display numbers easily. Uses an embedded printf function
 	 	 	   under an MIT License
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

//include a different printf function to redirect to uart using _putchar()
//(had problems trying to redirect the default printf function as it didn't call putc/fputc)
#include "printf.h"
#include "uart_redirect.h"
extern void el3mmu(void);


//purecap global-------------
#ifdef __CHERI_PURE_CAPABILITY__
	#include "capfuncs.h"
	#include <cheriintrin.h>
	// these two funcs should be run in boot code
	extern void el3DDCset(void);
	extern void setGLOBAL_UART_CAP(void);
	//create global capability for UART space here and extern in assembly
	//DO NOT extern here in C as does not work!
	//void* GLOBAL_UART_CAP;
	// now works - fixed error in size set up of capability in assembly and force .bss section
	// this is how other global built-in capabilities are referenced in c
	// see caddr_t _sbrk in syscalls.c where uses extern char* HeapCap, and set up in crt0.S
	extern void* GLOBAL_UART_CAP;
#endif

extern void el3mmu(void);




int main(void) {

#ifdef __CHERI_PURE_CAPABILITY__
	//GLOBAL CAPABILITY SET UP FROM BOOT CODE
	//-------------------------------------------------
	//This would be in boot code before DDC is NULLED
	el3DDCset(); //set ddc //this would be in boot code so not needed
	setGLOBAL_UART_CAP(); //this would go in boot code
	//-------------------------------------------------
	// !!Can't use printf now until mmu and uart set up as we've included the new "printf.h"!!
#endif
	//MMU
	//This would be in boot code before DDC is NULLED for capability mode
	//print to console
	puts("default mmu setup in EL3");
	//el3mmu change translation tables so can access device memory where uart space is
	el3mmu();
	//print to console
	puts("new mmu setup in EL3");

	// start of main code

#ifdef	__CHERI_PURE_CAPABILITY__
	// capability uart setup
	// set up the memory mapped uart pl011 standard setup defined by
	// global capability GLOBAL_UART_CAP
	// first check for invalid capability tag
	if (cheri_tag_get(GLOBAL_UART_CAP) == 0)
	{
		puts("Warning! - GLOBAL_UART_CAP capability not valid!\n");
		return -1;
	}
	else
	{
		puts("GLOBAL_UART_CAP capability check ok....\n");
		puts(".............................................\n\n");
	};
	uartRcapSetup(GLOBAL_UART_CAP);
	// write a string to the capability uart without printf
	uartRcapTransmitString("hello world to capability UART at EL3\n");
#else
	//non capability uart setup
	//set up the memory mapped uart pl011 standard setup at 0x1C090000
	uartRSetup((void*)(0x1C090000));
	// write a string to the standard uart without printf
	uartRTransmitString("hello world to uart at EL3\n");
#endif

	//print to uart using re-direct printf
	printf("printf re-direct test after uart set up\n");
	printf("This printf test prints a character, %c\na number, %d\na floating point, %f\n", 'z', 123, 456.789);
	printf(".............................................\n");

#ifdef __CHERI_PURE_CAPABILITY__
	printf("Capability details of global UART space \n");
	printcapabilityPar(GLOBAL_UART_CAP);
#endif
	//print to console
    puts("Finished program...\n");
	//loop here
    volatile uint32_t flag = 1;
	while(flag==1){}



	return EXIT_SUCCESS;
}
