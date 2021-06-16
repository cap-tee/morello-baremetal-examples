/*
 ============================================================================
 Name        : exceptionHandlerFuncsEL1N.h
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL1N exception handler functions header file
 ============================================================================
 */
#ifndef __exceptionHandlerFuncsEL2N_h
#define __exceptionHandlerFuncsEL2N_h

// DEFINE GLOBAL FLAG
//#define HANDLER_GLOBAL  __attribute__((__section__(".NONSECUREhandlerGlobalSection_c")))
// define global flag for timer interrupt, this is used by el1nmain() and irqHandlerEL1N()
// use the HANDLER_GLOBAL attribute to make sure the variable is put in non secure memory
//volatile uint32_t flagEL2N HANDLER_GLOBAL;

// FUNCTIONS
// sync exception handler to process EL1N memory access exceptions
void syncHandlerEL2N(void);

#endif
