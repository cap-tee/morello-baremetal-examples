/*
 ============================================================================
 Name        : exceptionHandlerFuncsEL1N.h
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL1N exception handler functions header file
 ============================================================================
 */
#ifndef __exceptionHandlerFuncsEL1N_h
#define __exceptionHandlerFuncsEL1N_h

// DEFINE GLOBAL FLAG
#define HANDLER_GLOBAL  __attribute__((__section__(".NONSECUREhandlerGlobalSection_c")))
// define global flag for timer interrupt, this is used by el1nmain() and irqHandlerEL1N()
// use the HANDLER_GLOBAL attribute to make sure the variable is put in non secure memory
volatile uint32_t flagEL1N HANDLER_GLOBAL;

// FUNCTIONS
// irq exception handler to process the timer interrupt
void irqHandlerEL1N(void);

#endif
