/*
 ============================================================================
 Name        : mesgEL1S.h
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : functions to pass messages between the
 	 	 	   non secure and secure worlds using two
 	 	 	   fixed memory locations
 ============================================================================
 */
#ifndef __mesgEL1S_h
#define __mesgEL1S_h

// Set up the message structure for secure
void mesgSSetup_EL1S(void* addr);
// Read secure message - char only
unsigned int mesgSRead_EL1S(void);
// Write to secure message - char only
void mesgSWrite_EL1S(unsigned int mesgChar);
// increment secure char by numChar
void incS_EL1S(int numChar);

#endif
