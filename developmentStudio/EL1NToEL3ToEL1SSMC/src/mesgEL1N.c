/*
 ============================================================================
 Name        : mesgEL1N.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : functions to pass messages between the
 	 	 	   non secure and secure worlds using two
 	 	 	   fixed memory locations
 ============================================================================
 */

//*****************************************
// INCLUDES
//*****************************************

#include <stdio.h>
#include <string.h>
// Program defined headers
#include "mesgEL1N.h"

//*****************************************
// DEFINES
//*****************************************

//define attributes for non secure memory sections
// this is used by the linker script to place non secure program code into non secure memory
// attributes can be assigned to functions and global variables
#define MESG_FUNC  __attribute__((__section__(".NONSECUREmesgFuncSection_c")))
#define MESG_STRUCT  __attribute__((__section__(".NONSECUREmesgStructSection_c")))

// Define memory region of non secure character
#define MESG_EL1N_ADDR	 (0xFFFC1000)

//-----------------------------------
// Define MesgEL1N structure
//-----------------------------------
struct MesgEL1N_struct {
	    //define memory and their offset addr
        volatile unsigned int MESGCHAR;        // +0x00 - character
        volatile unsigned int MESGINT;       // +0x08 - integer
};

//use the MESG_STRUCT attribute to make sure the variable is put in non secure memory
struct MesgEL1N_struct* MesgEL1N MESG_STRUCT;

//*************************************************************
// Functions
//*************************************************************

//-------------------------------------------------------------
// mesgNSetup_EL1N
// This function sets up the message structure for non secure
// addr - address of memory location
//-------------------------------------------------------------
void MESG_FUNC mesgNSetup_EL1N(void* addr)
{
  // Create message
  MesgEL1N = (struct MesgEL1N_struct*) addr;
  // don't reset, this is done in EL3
  // Reset the character
  //MesgEL1N->MESGCHAR  = 0x41; // dec 65, char A
  // Reset the integer
  //MesgEL1N->MESGINT   = 0x0;
  return;
}

//-----------------------------------------------------------
// mesgNRead_EL1N
// Read secure message - char only
//-----------------------------------------------------------
int MESG_FUNC mesgNRead_EL1N(void)
{
	int mesgChar = 0;
    //read char as an int
	mesgChar = MesgEL1N->MESGCHAR;
return mesgChar;
}

//-----------------------------------------------------------
// mesgNWrite_EL1N
// Write to secure message - char only
//-----------------------------------------------------------
void MESG_FUNC mesgNWrite_EL1N(int mesgChar)
{
    //write char as an int
	MesgEL1N->MESGCHAR = mesgChar;
return;
}

//-----------------------------------------------------------
// incN_EL1N
// Increments the char in secure memory by numChar amount
// numChar - number of characters to increment by
//-----------------------------------------------------------
void MESG_FUNC incN_EL1N(int numChar)
{
	unsigned int mesgWrite = 65; //65 is A
	unsigned int mesgRead = 90; //90 is Z
    //char mesgWriteStr[5] = { mesgWrite, '\n', '\0'};
    //char mesgReadStr[5] = { mesgRead, '\n', '\0'};

	// read character from non secure memory
	mesgRead = mesgNRead_EL1N();
	// increment character
	mesgWrite = mesgRead+numChar;
	// write new character to non secure memory
	mesgNWrite_EL1N(mesgWrite);

	// display values in console
	// put char into string and write to console
	// mesgReadStr[0] = mesgRead;
	// uartNTransmitString(mesgReadStr);
	// put char into string and write to console
	// mesgWriteStr[0] = mesgWrite;
	// uartNTransmitString(mesgWriteStr);
return;
}


