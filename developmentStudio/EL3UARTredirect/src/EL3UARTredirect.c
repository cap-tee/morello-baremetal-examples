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


int main(void) {

	volatile uint32_t flag = 1;
	//print to console
	puts("now in EL3");
	puts("default mmu setup in EL3");
	//el3mmu set up translation tables to use DRAM0, uart, and interrupt controller
	el3mmu();
	//print to console
	puts("new mmu setup in EL3");
	uartRSetup((void*)(0x1C090000)); //set up the memory mapped uart pl011 standard setup at 0x1C090000
	//print to uart using re-direct printf
	printf("printf re-direct test after uart set up\n");
	printf("This printf test prints a character, %c\na number, %d\na floating point, %f\n", 'z', 123, 456.789);
	//print to uart - string only - can't print numbers
	uartRTransmitString("string only test to uart\n");
	uartRTransmitString("finished test\n");
	//print to console
    puts("Finished program...\n");
	//loop here
	while(flag==1){}



	return EXIT_SUCCESS;
}
