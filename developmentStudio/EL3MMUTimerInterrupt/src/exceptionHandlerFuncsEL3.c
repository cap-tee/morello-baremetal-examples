/*
 ============================================================================
 Name        : exceptionHandlerFuncsEL3.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL3 exception handler functions
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "exceptionHandlerFuncsEL3.h"

//functions used
extern void stopTimer(void);
extern uint32_t getIntidAckReg0(void);
extern void setIntidEndReg0(uint32_t);
// get global flag
extern volatile uint32_t flagEL3;

//FIQ exception handler
//All FIQ's at EL3 come here
void fiqHandlerEL3(void)
{
uint32_t intid;
//get the interrupt ID - group0 - should be 29
intid = getIntidAckReg0();
// Secure and non-secure timer events have different interrupt IDs
//   ID30 = Non-secure Physical Timer Event.
//   ID29 = Secure Physical Timer Event
if (intid == 29)
	{
	flagEL3 = 1;
	stopTimer();
	}
else
	{
	puts("oops! intid is not 29!");
	}
//set interrupt end for intid
setIntidEndReg0(intid);
return;
}
