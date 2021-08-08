/*
 ============================================================================
 Name        : uartEL2N.h
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : non secure UART header file for EL2N uart functions
 ============================================================================
 */
#ifndef __uartEL2N_h
#define __uartEL2N_h

void uartEL2NSetup(void* addr);
void uartEL2NTransmitString(const char* uartstrN);

#endif
