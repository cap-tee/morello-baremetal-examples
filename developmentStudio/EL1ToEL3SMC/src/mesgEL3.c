/*
 ============================================================================
 Name        : mesgEL3.c
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
//**************************************************************

#include <stdio.h>
#include <string.h>
// Program defined headers
#include "mesgEL3.h"

//*************************************************************
// DEFINES
//*************************************************************
// Define memory region of non secure message
#define MESG_EL1N_ADDR	 (0xFFFC1000)
// Define memory region of secure message
#define MESG_EL1S_ADDR	 (0xBFFC1000)

//-----------------------------------
// Define MesgN structure for EL1N
//-----------------------------------
struct MesgN_struct {
	    //define memory and their offset addr
        volatile unsigned int MESGCHAR;        // +0x00 - character
        volatile unsigned int MESGINT;       // +0x08 - integer
};

struct MesgN_struct* MesgN;

//-----------------------------------
// Define MesgS structure for EL1S
//-----------------------------------
struct MesgS_struct {
	    //define memory and their offset addr
        volatile unsigned int MESGCHAR;        // +0x00 - character
        volatile unsigned int MESGINT;       // +0x08 - integer
};

struct MesgS_struct* MesgS;

//*************************************************************
// Functions
//*************************************************************

//-------------------------------------------------------------
// mesgNSetup
// This function sets up the message structure for non secure
// addr - address of memory location
//-------------------------------------------------------------
void mesgNSetup(void* addr)
{
  // Create message
  MesgN = (struct MesgN_struct*) addr;
  // Reset the character
  MesgN->MESGCHAR  = 0x5A; // dec 65 hex 41 char A, 5A is 90 char Z
  // Reset the integer
  MesgN->MESGINT   = 0x0;
  return;
}

//-----------------------------------------------------------
// mesgSSetup
// This function sets up the message structure for secure
// addr - address of memory location
//-----------------------------------------------------------
void mesgSSetup(void* addr)
{
  // Create message
  MesgS = (struct MesgS_struct*) addr;
  // Reset the character
  MesgS->MESGCHAR  = 0x5A; // dec 65 hex 41 char A, 5A is 90 char Z
  // Reset the integer
  MesgS->MESGINT   = 0x0;
  return;
}

//-----------------------------------------------------------
// mesgNRead
// Read non secure message - char only
//-----------------------------------------------------------
unsigned int mesgNRead(void)
{
	unsigned int mesgChar;
    //read char as an int
	mesgChar = MesgN->MESGCHAR;
return mesgChar;
}

//-----------------------------------------------------------
// mesgSRead
// Read secure message - char only
//-----------------------------------------------------------
unsigned int mesgSRead(void)
{
	unsigned int mesgChar;
    //read char as an int
	mesgChar = MesgS->MESGCHAR;
return mesgChar;
}

//-----------------------------------------------------------
// mesgNWrite
// Write to non secure message - char only
//-----------------------------------------------------------
void mesgNWrite(unsigned int mesgChar)
{
    //write char as an int
	MesgN->MESGCHAR = mesgChar;
return;
}

//-----------------------------------------------------------
// mesgSWrite
// Write to secure message - char only
//-----------------------------------------------------------
void mesgSWrite(unsigned int mesgChar)
{
    //write char as an int
	MesgS->MESGCHAR = mesgChar;
return;
}

//-----------------------------------------------------------
// memTestN
// Test non secure memory
// Writes and reads a number of characters to the non secure message location
//-----------------------------------------------------------
void memTestN(int numChar)
{
	unsigned int mesgWrite = 65; //65 is A
	unsigned int mesgRead = 90; //90 is Z
    char mesgWriteStr[5] = { mesgWrite, '\n', '\0'};
    char mesgReadStr[5] = { mesgRead, '\n', '\0'};

	//i - number of characters to write and read
	int i=0;

	puts("Doing non secure memory character test...");
	for (i=1; i<=numChar; i++)
	{
	// write character to non secure memory
	mesgNWrite(mesgWrite);
	// put char into string and write to console
	mesgWriteStr[0] = mesgWrite;
	puts("write char");
	puts(mesgWriteStr);
	// read character from non secure memory
	mesgRead = mesgNRead();
	// put char into string
	mesgReadStr[0] = mesgRead;
	puts("read char");
	puts(mesgReadStr);
	// increment character
	mesgWrite = mesgWrite+1;
	}
return;
}

//-----------------------------------------------------------
// memTestS
// Test secure memory
// Writes and reads a number of characters to the secure message location
//-----------------------------------------------------------
void memTestS(int numChar)
{
	unsigned int mesgWrite = 65; //65 is A
	unsigned int mesgRead = 90; //90 is Z
    char mesgWriteStr[5] = { mesgWrite, '\n', '\0'};
    char mesgReadStr[5] = { mesgRead, '\n', '\0'};

	//i - number of characters to write and read
	int i=0;

	puts("Doing secure memory character test...");
	for (i=1; i<=numChar; i++)
	{
	// write character into secure memory
	mesgSWrite(mesgWrite);
	// put char into string and write to console
	mesgWriteStr[0] = mesgWrite;
	puts("write char");
	puts(mesgWriteStr);
	// read character from secure memory
	mesgRead = mesgSRead();
	// put char into string
	mesgReadStr[0] = mesgRead;
	puts("read char");
	puts(mesgReadStr);
	// increment character
	mesgWrite = mesgWrite+1;
	}
return;
}

//-----------------------------------------------------------
// moveStoN
// Move char from secure to non secure memory location
//-----------------------------------------------------------
void moveStoN(void)
{
	unsigned int mesgRead = 90; //90 is Z
    char mesgReadStr[5] = { mesgRead, '\n', '\0'};

	puts("Moving character from secure to non secure...");
	// read character from secure memory
	mesgRead = mesgSRead();
	// write character into non secure memory
	mesgNWrite(mesgRead);
	// put char into string
	mesgReadStr[0] = mesgRead;
	puts("read char");
	puts(mesgReadStr);
	puts("Done.");
return;
}

//-----------------------------------------------------------
// moveNtoS
// Move char from non secure to secure memory location
//-----------------------------------------------------------
void moveNtoS(void)
{
	unsigned int mesgRead = 90; //90 is Z
    char mesgReadStr[5] = { mesgRead, '\n', '\0'};

	puts("Moving character from non secure to secure...");
	// read character from non secure memory
	mesgRead = mesgNRead();
	// write character into secure memory
	mesgSWrite(mesgRead);
	// put char into string
	mesgReadStr[0] = mesgRead;
	puts("read char");
	puts(mesgReadStr);
	puts("Done.");
return;
}

//-----------------------------------------------------------
// incN
// Increments the char in non secure memory by numChar amount
// numChar - number of characters to increment by
//-----------------------------------------------------------
void incN(int numChar)
{
	unsigned int mesgWrite = 65; //65 is A
	unsigned int mesgRead = 90; //90 is Z
    char mesgWriteStr[5] = { mesgWrite, '\n', '\0'};
    char mesgReadStr[5] = { mesgRead, '\n', '\0'};

    puts("incrementing non secure char");
	// read character from non secure memory
	mesgRead = mesgNRead();
	// increment character
	mesgWrite = mesgRead+numChar;
	// write new character to non secure memory
	mesgNWrite(mesgWrite);

	// display values in console
	// put char into string and write to console
	mesgReadStr[0] = mesgRead;
	puts("read char");
	puts(mesgReadStr);
	// put char into string and write to console
	mesgWriteStr[0] = mesgWrite;
	puts("write char");
	puts(mesgWriteStr);
return;
}

//-----------------------------------------------------------
// incS
// Increments the char in secure memory by numChar amount
// numChar - number of characters to increment by
//-----------------------------------------------------------
void incS(int numChar)
{
	unsigned int mesgWrite = 65; //65 is A
	unsigned int mesgRead = 90; //90 is Z
    char mesgWriteStr[5] = { mesgWrite, '\n', '\0'};
    char mesgReadStr[5] = { mesgRead, '\n', '\0'};

    puts("incrementing secure char");
	// read character from non secure memory
	mesgRead = mesgSRead();
	// increment character
	mesgWrite = mesgRead+numChar;
	// write new character to non secure memory
	mesgSWrite(mesgWrite);

	// display values in console
	// put char into string and write to console
	mesgReadStr[0] = mesgRead;
	puts("read char");
	puts(mesgReadStr);
	// put char into string and write to console
	mesgWriteStr[0] = mesgWrite;
	puts("write char");
	puts(mesgWriteStr);
return;
}
