/*
 ============================================================================
 Name        : EL2entry.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL3 main c code
 	 	 	   This example sets up the mmu for EL2, and vector table.
			   At EL2 the hypervisor controls EL1 and sets up the two stage
			   translation tables.
			   To disable EL1N from reading or writing to the MMU registers
			   uncomment the disableEL1N_mmuChange function
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
//#include "exceptionHandlerFuncsEL3.h" //include the global flagEL3 flag if using timer
//#include "mesgEL3.h" // read/write messages to EL1S/EL1N
#include "uartN.h"

//*****************************************
// DEFINES
//*****************************************
#define LOCATE_FUNC  __attribute__((__section__(".NONSECUREsection_el2_c")))

//Functions to include
extern void ERETtoEL1N(void);
extern void el2nmmu(void);
extern void installVectorsEL2N(void);

extern void el2_stg1ForEl1n_mmu(void);
extern void el2_stg2ForEl1n_mmu(void);
extern void disableEL1N_mmuChange(void);


//Main program code of non secure Hypervisor EL2
int LOCATE_FUNC el2nmain(void)
{

	// install vector table for EL2N hypervisor
	installVectorsEL2N(); //regForEL2N.s

	//  uart string
	char uartstr[8] = {'E', 'L', '2', 'N', '\n', '\0'};

	// el2mmu set up translation tables to use DRAM0, and device memory
	el2nmmu();

	// set up the memory mapped uart pl011 standard setup at 0x1C090000
	uartNSetup((void*)(0x1C090000));

	// send string to uart
	uartNTransmitString(uartstr);


	// set up the TWO stage MMU translation for EL1N
    // 1.set second stage MMU translation
	el2_stg2ForEl1n_mmu();
	// 2.set first stage MMU translation
	el2_stg1ForEl1n_mmu();

	// disable EL1N from changing the MMU registers
	// uncomment this code to disable
	//disableEL1N_mmuChange(); //regForEL2N.s

	// Perform an ERET to EL1N
	ERETtoEL1N();


	return EXIT_SUCCESS;
}
