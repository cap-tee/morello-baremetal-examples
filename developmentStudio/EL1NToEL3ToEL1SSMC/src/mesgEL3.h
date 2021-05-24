/*
 ============================================================================
 Name        : mesgEL3.h
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : functions to pass messages between the
 	 	 	   non secure and secure worlds using two
 	 	 	   fixed memory locations
 ============================================================================
 */
#ifndef __mesgEL3_h
#define __mesgEL3_h

// Set up the message structure for non secure
void mesgNSetup(void* addr);
// Set up the message structure for secure
void mesgSSetup(void* addr);
// Read non secure message - char only
unsigned int mesgNRead(void);
// Read secure message - char only
unsigned int mesgSRead(void);
// Write to non secure message - char only
void mesgNWrite(unsigned int mesgChar);
// Write to secure message - char only
void mesgSWrite(unsigned int mesgChar);
// non secure memory test
void memTestN(int numChar);
// secure memory test
void memTestS(int numChar);
// move char from secure to non secure memory location
void moveStoN(void);
// move char from non secure to secure memory location
void moveNtoS(void);
// increment secure char by numChar
void incS(int numChar);
// increment non secure char by numChar
void incN(int numChar);

#endif
