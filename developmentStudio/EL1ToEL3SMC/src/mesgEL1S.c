/*
 ============================================================================
 Name        : mesgEL1S.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : functions to pass messages between the
 	 	 	   non secure and secure worlds using two
 	 	 	   fixed memory locations
 ============================================================================
*/

//*************************************************************
// INCLUDES
//*************************************************************

#include <stdio.h>
#include <string.h>
// Program defined headers
#include "mesgEL1S.h"

//*************************************************************
// DEFINES
//*************************************************************
// Define memory region of secure message
#define MESG_EL1S_ADDR	 (0xBFFC1000)

//-----------------------------------
// Define MesgS structure for EL1S
//-----------------------------------
struct MesgEL1S_struct {
	    //define memory and their offset addr
        volatile unsigned int MESGCHAR;        // +0x00 - character
        volatile unsigned int MESGINT;       // +0x08 - integer
};

struct MesgEL1S_struct* MesgEL1S;

//*************************************************************
// Functions
//*************************************************************

//-------------------------------------------------------------
// mesgSSetup_EL1S
// This function sets up the message structure for secure
// addr - address of memory location
//-------------------------------------------------------------
void mesgSSetup_EL1S(void* addr)
{
  // Create message
  MesgEL1S = (struct MesgEL1S_struct*) addr;
  // don't reset, this is done in EL3
  // Reset the character
  // MesgS->MESGCHAR  = 0x5A; // dec 65 hex 41 char A, 5A is 90 char Z
  // Reset the integer
  // MesgS->MESGINT   = 0x0;
  return;
}

//-----------------------------------------------------------
// mesgSRead_EL1S
// Read secure message - char only
//-----------------------------------------------------------
unsigned int mesgSRead_EL1S(void)
{
	unsigned int mesgChar;
    //read char as an int
	mesgChar = MesgEL1S->MESGCHAR;
return mesgChar;
}

//-----------------------------------------------------------
// mesgSWrite_EL1S
// Write to secure message - char only
//-----------------------------------------------------------
void mesgSWrite_EL1S(unsigned int mesgChar)
{
    //write char as an int
	MesgEL1S->MESGCHAR = mesgChar;
return;
}

//-----------------------------------------------------------
// incS_EL1S
// Increments the char in secure memory by numChar amount
// numChar - number of characters to increment by
//-----------------------------------------------------------
void incS_EL1S(int numChar)
//numChar - number of characters to increment by
{
	unsigned int mesgWrite = 65; //65 is A
	unsigned int mesgRead = 90; //90 is Z
    // char mesgWriteStr[5] = { mesgWrite, '\n', '\0'};
    // char mesgReadStr[5] = { mesgRead, '\n', '\0'};

	// read character from non secure memory
	mesgRead = mesgSRead_EL1S();
	// increment character
	mesgWrite = mesgRead+numChar;
	// write new character to non secure memory
	mesgSWrite_EL1S(mesgWrite);

	// display values in window
	// put char into string and write to console
	// mesgReadStr[0] = mesgRead;
	// uartSTransmitString(mesgReadStr);
	// put char into string and write to console
	// mesgWriteStr[0] = mesgWrite;
	// uartSTransmitString(mesgWriteStr);
return;
}
