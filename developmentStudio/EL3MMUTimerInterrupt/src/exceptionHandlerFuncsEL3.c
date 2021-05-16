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
#include "exceptionHandlerFuncsEL3.h" //include the global flagEL3 flag

//functions used
extern void disableTimer(void);
extern uint32_t readIAR0(void);
extern void writeEOIR0(uint32_t);

//FIQ exception handler
//All FIQ's at EL3 come here
void fiqHandlerEL3(void)
{
uint32_t intid;
intid = readIAR0();
// Secure and non-secure timer events have different interrupt IDs
//   ID30 = Non-secure Physical Timer Event.
//   ID29 = Secure Physical Timer Event
if (intid == 29) {
	flag = 1;
	disableTimer();
} else {
	puts("Should never reach here!");
}
writeEOIR0(intid);
return;
}
