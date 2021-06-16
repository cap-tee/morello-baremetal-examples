/*
 ============================================================================
 Name        : exceptionHandlerFuncsEL1S.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL1S exception handler functions
   	 	 	   Contains a FIQ exception handler for secure group 0
 	 	 	   generated by EL1S in program state 1
 ============================================================================
 */

//*****************************************
// INCLUDES
//*****************************************
#include <stdio.h>
#include <stdlib.h>

// Program defined headers
#include "uartS.h"
#include "exceptionHandlerFuncsEL1S.h" //include the global flagEL1S flag

//functions used
extern void disableTimerS(void);
extern uint32_t readIAR0S(void);
extern void writeEOIR0S(uint32_t);

//*****************************************
// FUNCTIONS
//*****************************************
//------------------------------------
// fiqHandlerEL1S
//------------------------------------
// FIQ exception handler for EL1S, program state 1
// This is called by the vector table
// checks for the secure physical timer event
// disables the timer and resets the interrupt
// Note: this example uses secure group 0 (FIQ) for secure EL1
void fiqHandlerEL1S(void)
{
uint32_t intid;
// read group 0 interrupt acknowledge register
intid = readIAR0S();
// uartSTransmitString("inside fiqHandlerEL1S\n");
// Secure and non-secure timer events have different interrupt IDs
//   ID30 = Non-secure Physical Timer Event.
//   ID29 = Secure Physical Timer Event
if (intid == 29)
{
	// uartSTransmitString("ID is 29\n"); //must be 29 if here
	flagEL1S = 1;
	// disable secure timer
	disableTimerS();
} else
{
	// Should never get here
}
// write end of interrupt for ID 29 - group 0 register
writeEOIR0S(intid);
return;
}