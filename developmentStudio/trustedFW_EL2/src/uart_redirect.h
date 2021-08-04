/*
 ============================================================================
 Name        : uart_redirect.h
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : UART header file
 ============================================================================
 */
#ifndef __uart_fputcRedirect_h
#define __uart_fputcRedirect_h

#include <stdio.h>

void uartRSetup(void* addr);
void uartRTransmitString(const char* uartstr);
int fputc(int c, FILE *f);
void _putchar(char character);


#endif
