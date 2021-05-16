/*
 ============================================================================
 Name        : exceptionHandlerFuncsEL3.h
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL3 exception handler functions
 ============================================================================
 */
#ifndef __exceptionHandlerFuncsEL3_h
#define __exceptionHandlerFuncsEL3_h

//define global flag, this is used by main()
//as well to reset the flag
volatile uint32_t flag;

void fiqHandlerEL3(void);

#endif
