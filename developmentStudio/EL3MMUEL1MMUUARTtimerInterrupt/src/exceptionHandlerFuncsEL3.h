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

//define global flag, this is used by main() and fiqHandlerEL3()
volatile uint32_t flagEL3;

void fiqHandlerEL3(void);

#endif
