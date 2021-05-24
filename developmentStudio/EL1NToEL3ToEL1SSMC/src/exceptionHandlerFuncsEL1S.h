/*
 ============================================================================
 Name        : exceptionHandlerFuncsEL1S.h
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL1S exception handler functions
  	 	 	   Contains a FIQ exception handler for secure group 0
 	 	 	   generated by EL1S in program state 1
 ============================================================================
 */
#ifndef __exceptionHandlerFuncsEL1S_h
#define __exceptionHandlerFuncsEL1S_h

// DEFINE GLOBAL FLAG
// define global flag for timer interrupt, this is used by el1smain() and fiqHandlerEL1S()
volatile uint32_t flagEL1S;

// FUNCTIONS
// fiq exception handler to process the timer interrupt
void fiqHandlerEL1S(void);

#endif
