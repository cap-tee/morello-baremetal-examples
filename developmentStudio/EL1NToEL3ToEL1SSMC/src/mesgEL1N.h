/*
 ============================================================================
 Name        : mesgEL1N.h
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : functions to pass messages between the
 	 	 	   non secure and secure worlds using two
 	 	 	   fixed memory locations
 ============================================================================
 */
#ifndef __mesgEL1N_h
#define __mesgEL1N_h

// Set up the message structure for non secure
void mesgNSetup_EL1N(void* addr);
// Read non secure message - char only
int mesgNRead_EL1N(void);
// Write to non secure message - char only
void mesgNWrite_EL1N(int mesgChar);
// increment non secure char by numChar
void incN_EL1N(int numChar);

#endif
