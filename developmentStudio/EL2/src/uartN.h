/*
 ============================================================================
 Name        : uartN.h
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : non secure UART header file for non secure uart functions
 ============================================================================
 */
#ifndef __uartN_h
#define __uartN_h

void uartNSetup(void* addr);
void uartNTransmitString(const char* uartstrN);

#endif
