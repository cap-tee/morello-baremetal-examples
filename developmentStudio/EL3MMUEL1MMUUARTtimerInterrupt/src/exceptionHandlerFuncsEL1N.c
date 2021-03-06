/*
 ============================================================================
 Name        : exceptionHandlerFuncsEL1N.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL1N exception handler functions
 	 	 	   Contains an IRQ exception handler for non secure group 1
 	 	 	   generated by EL1N in program state 1
 ============================================================================
 */

//define attributes for non secure memory sections
// this is used by the linker script to place non secure program code into non secure memory
// attributes can be assigned to functions and global variables
#define HANDLER_FUNC  __attribute__((__section__(".NONSECUREhandlerFuncSection_c")))

#include <stdio.h>
#include <stdlib.h>

// Program defined headers
#include "uartN.h"
#include "exceptionHandlerFuncsEL1N.h" //include the global flagEL1N flag

//timer functions
extern void stopTimerN(void);
//group 1 interrupt functions
extern uint32_t getIntidAckReg1N(void);
extern void setIntidEndReg1N(uint32_t);

// get global flag
extern volatile uint32_t flagEL1N;

//************************************
//irqHandlerEL1N
//************************************
//IRQ exception handler for EL1, program state 1
//This is called by the vector table
//checks for the non secure physical timer event
//disables the timer and resets the interrupt
//Note: this example uses Non-secure group 1 (IRQ) for Non secure EL1
void HANDLER_FUNC irqHandlerEL1N(void)
{
uint32_t intid;
//get group 1 interrupt id from acknowledge register
intid = getIntidAckReg1N();
//local string - don't forget the \n else will get overwritten by next message
char uartstr[8] = {'I', 'R', 'Q', ' ', 'N', '\n', '\0'};
char uartstr1[7] = {'I', 'D', '3', '0', '\n', '\0'};
uartNTransmitString(uartstr);
// Secure and non-secure timer events have different interrupt IDs
//   ID30 = Non secure Physical Timer Event.
//   ID29 = Secure Physical Timer Event
if (intid == 30)
{
	uartNTransmitString(uartstr1); //ID must be 30
	flagEL1N = 1;
	//stop the non secure physical timer
	stopTimerN();
}
else
{
	//error
}
//set end of interrupt for ID 30 - group 1 register
setIntidEndReg1N(intid);
return;
}
