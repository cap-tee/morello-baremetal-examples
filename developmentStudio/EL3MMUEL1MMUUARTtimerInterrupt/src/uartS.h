/*
 ============================================================================
 Name        : uartS.h
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : secure UART header file
 ============================================================================
 */
#ifndef __uartS_h
#define __uartS_h

void uartSSetup(void* addr);
void uartSTransmitString(const char* uartstr);

#endif
