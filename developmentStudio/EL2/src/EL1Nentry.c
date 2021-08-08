/*
 ============================================================================
 Name        : EL1Nentry.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL1 main non secure (normal world) code
 	 	 	   In this set up EL2 hypervisor controls EL1 memory and page tables.
 	 	 	   EL1 tries to make changes to the memory set up.
 	 	 	   When the EL2 hypervisor disables EL1 from making changes to the mmu
 	 	 	   registers an exception occurs which is routed to the EL2 vector table.
 	 	 	   When the EL2 hypervisor disables EL1 from making changes to the memory
 	 	 	   region where the page tables / vector table is stored an exception
 	 	 	   occurs which is routed to the EL1 vector table. Because the exception
 	 	 	   handler tries to read ESR_EL1, this gets trapped to EL2.


 Limitations : The c function library is initialised by the default compile/
 	 	 	   linker to reside in the lower section of DRAM0, which is set
 	 	 	   up as the secure memory location for EL3 in this setup.
 	 	 	   Currently EL2 also resides in the lower half of DRAM0 memory,
 	 	 	   so that EL1N mostly occupies upper DRAM0 memory, except for the page
 	 	 	   tables and vector tables which can be access read only if this is set.

 	 	 	   It appears that the nested virtualisation feature is not present in Morello
 	 	 	   meaning that there is no option to trap a VBAR_EL1 access from EL1 to EL2.
 	 	 	   This means that EL1 could change where the Vector Table register is
 	 	 	   pointing to in memory, and thus change the exception handlers when
 	 	 	   a write occurs to a read only memory location.

 ============================================================================
 */

//*****************************************
// DEFINES
//*****************************************
//Need to put all non secure code into non secure memory regions
//Attributes are used to define memory sections.
//The linker script places the memory sections into the correct regions.
//Note: Attributes can only be assigned to global variables and functions
#define LOCATE_FUNC  __attribute__((__section__(".NONSECUREsection_c_el1")))
#define LOCATE_STR  __attribute__((__section__(".NONSECUREStringSection_c_el1")))


//*****************************************
// INCLUDES
//*****************************************
#include <stdio.h>
#include <stdlib.h>

// C Functions to include
#include "uartN.h" //non secure uart functions

// Assembly functions to include
extern void el1nmmu(void); //mmu set up
// function to change MAIR reg
extern void el1MAIRmod(void);
// function to modify the translation table in memory using register
extern void el1TTmod(void);
// function to modify the translation table in memory knowing the memory address
extern void el1TTMemMod(void);
// function to modify the vector table in memory using register
extern void el1VTmod(void);
// function to modify the vector table in memory knowing the memory address
extern void el1VTMemMod(void);
// function to change TCR reg
extern void el1TCRmod(void);
// function to change SCTLR reg
extern void el1SCTLRmod(void);

// Define string as a global variable to give it an attribute
//can't do this!! as only stores the pointer in non secure memory,
//the string still gets stored in the secure .bss region!!!
//const char* uartStr LOCATE_STR = "hello world to uart at EL1N\n";

//Main program code of non secure EL1
int LOCATE_FUNC el1nmain(void)
{
	//create a flag for the wait loop - to stop program
	volatile uint32_t flag = 1;

	//*****STRINGS***********************************************************
	//even though inside LOCATE_FUNC, string still stored in secure .bss region!!!
	//so can't do this either!
	//const char* uartStr = "hello world to uart at EL1N\n"

	//These three ways seem to be the only way to assign a string locally in the same non secure memory region
	//(for the current set up - where you have one program trying to switch EL)
	// Not Ideal!!! - need to find better method
	//Remember can't use c lib funcs to find the string length
	//char uartstr[24] = {'H', 'e', 'l', 'l', 'o',' ', 'W','o', 'r','l', 'd',' ', 'f','r', 'o','m', ' ', 'E', 'L', '1', 'N','\n', '\0'};
	//char uartstr[24] = "Hello World from EL1N\n";
	//char uartstr[] = "Hello World from EL1N\n\0";
	//*************************************************************************

	// uart strings
	char uartstr[8] = {'E', 'L', '1', 'N', '\n', '\0'};
	char uartstr1[8] = {'T', 'E', 'S', 'T', ' ', '\0'};
	char uartstr2[8] = {'D', 'O', 'N', 'E', '\n', '\0'};
	// EL1N mmu already set up

	// set up the memory mapped uart pl011 standard setup at 0x1C090000
	uartNSetup((void*)(0x1C090000));

	// send string to uart
	uartNTransmitString(uartstr);

    //------------------------------------------------------
	//MESS ABOUT WITH MEMORY - TESTING
	// Try to mess with the EL1 MMU memory registers
	// Try to write to page table memory locations
	// uncomment tests as needed

	// Test1
	// function to change MAIR reg - try to change memory properties
	// this will cause an MSR MRS exception in EL2 hypervisor if mmu register access is disabled in EL2
	uartNTransmitString(uartstr1);
	uartNTransmitString("1\n\0");
	el1MAIRmod();
	uartNTransmitString(uartstr2); //only get here if not disabled

	//Test2
	// function to modify the translation table in memory using the register
	// this will cause an MSR MRS exception in EL2 hypervisor if mmu register access is disabled in EL2
	uartNTransmitString(uartstr1);
	uartNTransmitString("2\n\0");
	el1TTmod();
	uartNTransmitString(uartstr2); //only get here if not disabled

	//Test3
	// function to modify the translation table in memory knowing the address.
	// This function will only complete if the memory region is set to read/write.
	// Otherwise this will cause a data abort on trying to write to a memory location (see ESR_EL1)
	// because the section of memory where the page tables are stored are set to read only to
	// prevent EL1N from writing to them.
	// The data abort synchronous exception goes to the EL1N vector table.
	// However the exception handler reads the ESR register, and if the EL1 registers
	// are disabled as well this will then be trapped to EL2 before the exception handler can
	// complete, causing exception handler code in EL2 to be run instead.
	uartNTransmitString(uartstr1);
	uartNTransmitString("3\n\0");
	el1TTMemMod();
	uartNTransmitString(uartstr2); //only get here if not disabled

	//Test4
	// try to modify VBAR_EL1 register
	// if this test passes, whilst VBAR access is disabled, its
	// because the nested virtualisation feature is not implemented.
	// Trying to set NV1 bit to 1 to create an exception for VBAR_EL1
	// doesn't cause a change in NV1 bit in the register, it stays at zero
	// This implies the nested virtualisation feature is not implemented.
	// We can therefore not prevent VBAR from being modified to point to a different memory location
	uartNTransmitString(uartstr1);
	uartNTransmitString("4\n\0");
	el1VTmod();
	uartNTransmitString(uartstr2); //gets here even if try to disable

	//Test5
	// try to modify the vector table in memory
	// This function will only complete if the memory region is set to read/write.
	// Otherwise this will cause a data abort on trying to write to a memory location (see ESR_EL1)
	// because the section of memory where the vector table is stored is set to read only
	// to prevent EL1N from writing to them.
	// The data abort synchronous exception goes to the EL1N vector table.
	// However the exception handler reads the ESR register, and if the EL1 registers
	// are disabled as well this will then be trapped to EL2 before the exception handler can
	// complete, causing exception handler code in EL2 to be run instead.
	uartNTransmitString(uartstr1);
	uartNTransmitString("5\n\0");
	el1VTMemMod();
	uartNTransmitString(uartstr2); //only get here if not disabled


	//Test6
	// function to change TCR reg - control reg
	// this will cause an MSR MRS exception in EL2 hypervisor if mmu register access is disabled in EL2
	uartNTransmitString(uartstr1);
	uartNTransmitString("6\n\0");
	el1TCRmod();
	uartNTransmitString(uartstr2); //only get here if not disabled

	//Test7
	// function to change SCTLR reg - try to turn on or off mmu
	// this will cause an MSR MRS exception in EL2 hypervisor if mmu register access is disabled in EL2
	uartNTransmitString(uartstr1);
	uartNTransmitString("7\n\0");
	el1SCTLRmod();
	uartNTransmitString(uartstr2); //only get here if not disabled

    //------------------------------------------------------

	// loop here
	while(flag==1){}

	// We never get here
	return EXIT_SUCCESS;
}
