/*
 ============================================================================
 Name        : exceptionHandlerFuncsEL3.h
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL3 exception handler functions header
 ============================================================================
 */
#ifndef __exceptionHandlerFuncsEL3_h
#define __exceptionHandlerFuncsEL3_h

// DEFINE GLOBAL FLAG
//define global flag, this is used by main() and fiqHandlerEL3() for timer
volatile uint32_t flagEL3;

// FUNCTIONS
// fiq exception handler to process the timer interrupt
void fiqHandlerEL3(void);
// synchronisation exception handler for processing SMC
void syncHandlerEL3(void);

#endif
