/*
 ============================================================================
 Name        : EL1Nentry.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL1 main non secure (normal world) code

 Limitations : The c function library is initialised by the default compile/
 	 	 	   linker to reside in the lower section of DRAM0, which is set
 	 	 	   up as the secure memory location for EL3 in this setup. This
 	 	 	   means that any c function call from the normal world such as
 	 	 	   puts tries to jump to secure memory to run. The set up of the
 	 	 	   MMU enforces the restrictions and will terminate by exception
 	 	 	   for any secure memory access. This code uses the UART to
 	 	 	   output a message.
 ============================================================================
 */

//Need to put all non secure code into non secure memory regions
//Attributes are used to define memory sections.
//The linker script places the memory sections into the correct regions.
//Note: Attributes can only be assigned to global variables and functions
#define LOCATE_FUNC  __attribute__((__section__(".NONSECUREsection_c")))
#define LOCATE_STR  __attribute__((__section__(".NONSECUREStringSection_c")))

#include <stdio.h>
#include <stdlib.h>

//Functions to include
#include "uartN.h"
extern void el1nmmu(void);

// Define string as a global variable to give it an attribute
//can't do this!! as only stores the pointer in non secure memory,
//the string still gets stored in the secure .bss region!!!
//const char* uartStr LOCATE_STR = "hello world to uart at EL1N\n";

//Main program code of non secure EL1
int LOCATE_FUNC el1nmain(void)
{
	//create a flag for the wait loop
	volatile uint32_t flag = 1;
	//even though inside LOCATE_FUNC, string still stored in secure .bss region!!!
	//so can't do this!
	//const char* uartStr = "hello world to uart at EL1N\n"

	//These three ways seem to be the only way to assign a string locally in the same non secure memory region
	//(for the current set up - where you have one program trying to switch EL)
	// Not Ideal!!!
	//Remember can't use c lib funcs to find the string length
	//char uartstr[24] = {'H', 'e', 'l', 'l', 'o',' ', 'W','o', 'r','l', 'd',' ', 'f','r', 'o','m', ' ', 'E', 'L', '1', 'N','\n', '\0'};
	char uartstr[24] = "Hello World from EL1N\n";
	//char uartstr[] = "Hello World from EL1N\n\0";

	//el1nmmu set up translation tables to use DRAM0, uart, and interrupt controller
	el1nmmu();

	//set up the memory mapped uart pl011 ststandard setup at 0x1C090000
	uartNSetup((void*)(0x1C090000));

	//send message to uart
	uartNTransmitString(uartstr);

	//loop here
	while(flag==1){}

	// We never get here
	return EXIT_SUCCESS;
}
