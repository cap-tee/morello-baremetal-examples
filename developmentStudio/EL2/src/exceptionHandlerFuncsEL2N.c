/*
 ============================================================================
 Name        : exceptionHandlerFuncsEL2N.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL2N hypervisor exception handler functions
 	 	 	   Contains a SYNC exception handler for when
 	 	 	   the hypervisor disables EL1N from modifying
 	 	 	   MMU registers
 ============================================================================
 */

//*************************************************
// DEFINES
//*************************************************
// define attributes for non secure memory sections
// this is used by the linker script to place non secure program code into non secure memory
// attributes can be assigned to functions and global variables
#define HANDLER_FUNC  __attribute__((__section__(".NONSECUREhandlerFuncSectionEL2_c")))

#define ILLEGAL_EL1NMMU_ACCESS (0x60000000) //exception ID for an MSR MRS exception [0110 0000 0000...]

//*************************************************
// INCLUDES
//*************************************************
#include <stdio.h>
#include <stdlib.h>

// Program defined headers
#include "uartN.h" 						// uart non secure functions
#include "exceptionHandlerFuncsEL2N.h"  // include the global flagEL1N flag

// READ REG functions
extern uint32_t readESREL2(void);	//regForEL2N



//************************************
// FUNCTIONS
//************************************
//------------------------------------
// syncHandlerEL2N
//------------------------------------
// SYNC exception handler for EL2,
// This is called by the vector table
// checks for the MSR MRS exception if EL1 tries to modify the MMU/memory translation registers
void HANDLER_FUNC syncHandlerEL2N(void)
{
	uint32_t esr;     //esr register - exception register
	uint32_t esr_ec;  //ESR.EC[31:26] type of exception

	//create a flag for the wait loop - to stop program
	volatile uint32_t flag = 1;

	// memory message strings
	char mesgReadText[8] = { 'S', 'Y', 'N', 'C', '\n', '\0'};
	char mesgReadText2[8] = { 'M', 'M', 'U', ' ', '\n', '\0'};
	char mesgReadText3[8] = { '?', '?', '?', '?', '\n', '\0'};
	uartNTransmitString(mesgReadText);

	// read exception information
	esr = readESREL2();  //regForEL2N.s
	// mask bits 31:26 ESR.EC to check type of SYNC exception
	//       [1111][1100]00 0000
	esr_ec = esr & 0xFC000000;


    // Check what type of SYNC exception it is
	switch (esr_ec)
	{
		// Check if its an MSR MRS exception
		case ILLEGAL_EL1NMMU_ACCESS:
		// send message to uart
		uartNTransmitString(mesgReadText2);
		// loop here and stop program
		while(flag==1){}
		break;

		default:
		// send message to uart
		uartNTransmitString(mesgReadText3);
		break;
	}
return;
}
