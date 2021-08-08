/*
 ============================================================================
 Name        : EL2entry.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL2 main c code
 	 	 	   This example sets up the mmu for EL2, and vector table.
			   At EL2 the hypervisor controls EL1 and sets up the two stage
			   translation tables.
			   To disable EL1N from reading or writing to the MMU registers
			   uncomment the disableEL1N_mmuChange function.
			   To disable EL1N from reading/writing to the vector table register
			   uncomment the disableEL1N_vectorChange() function NOTE: FEATURE
			   NOT PRESENT SO DOES NOT DISABLE
			   To disable EL1N from changing the EL1 page table entries and
			   vector table entries directly in memory
			   uncomment the disableEL1N_memRO() function.

 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "uartEL2N.h"

//*****************************************
// DEFINES
//*****************************************
#define LOCATE_FUNC  __attribute__((__section__(".NONSECUREsection_c_el2")))

//Functions to include
extern void ERETtoEL1N(void);
extern void el2nmmu(void);
extern void installVectorsEL2N(void);
extern void installVectorsEL1N(void);

extern void el2_stg1ForEl1n_mmu(void);
extern void el2_stg2ForEl1n_mmu(void);
extern void disableEL1N_mmuChange(void);
extern void disableEL1N_vectorChange(void);
extern void disableEL1N_memRO(void);


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
	uartEL2NSetup((void*)(0x1C090000));

	// send string to uart
	uartEL2NTransmitString(uartstr);


	// set up the TWO stage MMU translation for EL1N
    // 1.set second stage MMU translation
	el2_stg2ForEl1n_mmu();
	// 2.set first stage MMU translation
	el2_stg1ForEl1n_mmu();

	//install el1 vector table
	installVectorsEL1N(); //regForEL2N.s

	// disable EL1N from reading/writing to the MMU registers
	// uncomment this code to disable
	//disableEL1N_mmuChange(); //regForEL2N.s

	// disable EL1N from reading/writing to the vector table register
	// uncomment this code to disable
	// NOTE: feature not present in Morello, so does not disable
	//disableEL1N_vectorChange(); //regForEL2N.s

	// disable EL1N from changing the
	// EL1 page table entries and vector table entries
	// by setting the memory region to read only
	// uncomment this code to disable
	//disableEL1N_memRO(); //regForEL2N.s

	// Perform an ERET to EL1N
	ERETtoEL1N();


	return EXIT_SUCCESS;
}
