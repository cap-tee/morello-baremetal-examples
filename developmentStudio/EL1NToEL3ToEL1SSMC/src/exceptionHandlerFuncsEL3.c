/*
 ============================================================================
 Name        : exceptionHandlerFuncsEL3.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL3 exception handler functions
 	 	 	   Contains a FIQ exception handler for secure group 0
 	 	 	   generated by EL3
 ============================================================================
 */

//*****************************************
// DEFINES
//*****************************************
//define SYNC exception IDs for SMC calls
#define SMC 		 (0x5C000000)     // exception ID for an SMC call [0101 1100]00 0000 in ESR_EL3 register
// IDs for moving a message between worlds - user defined - 16 bits
#define SMC_ID_MESGN (0x01) 			  // non secure message ID to move char from defined non secure memory region
#define SMC_ID_MESGN_INC (0x02)      // 16 bit non secure message ID to inc char in EL3 and go back to non secure
#define SMC_ID_MESGS (0x20) 			  // secure message ID to move char from defined secure memory region

//*****************************************
// INCLUDES
//*****************************************
#include <stdio.h>
#include <stdlib.h>
//functions used
#include "exceptionHandlerFuncsEL3.h" //include the global flagEL3 flag for timer interrupt
#include "mesgEL3.h" //include message functions for manipulating a message across secure and non secure worlds
#include "uartS.h" //include writing messages to secure uart

//extern functions used
extern void stopTimerS(void);   //timerS.s
extern uint32_t getIntidAckReg0S(void);   // interrupt register information interruptRegS.s
extern void setIntidEndReg0S(uint32_t); // interrupt register information interruptRegS.s
extern uint32_t readESREL3(void);  //exception information for EL3 exceptionRegEL3.s file

extern void setupGIC600S(void);   // for secure timer interrupt gicS.s
extern void ERETtoEL1S(void); //regForEL1S.s
extern void setupGIC600N(void);   // for non secure timer interrupt gicN.s
extern void ERETtoEL1N(void); //regForEL1N.s

//*****************************************
// FUNCTIONS
//*****************************************
//------------------------------------
// syncHandlerEL3
//------------------------------------
// SYNC exception handler for EL3 from lower EL
// This is called by the vector table
void syncHandlerEL3(void)
{
	uint32_t esr;     //esr register - exception
	uint32_t esr_ec;  //ESR.EC[31:26] type of exception
	uint32_t esr_smc; //smc exception ID

	// memory message strings
	char mesgReadText[8] = { 'm', 'e', 's', 'g', '\n', '\0'};
	unsigned int mesgRead = 90; //90 is Z
    char mesgReadStr[5] = { mesgRead, '\n', '\0'};

	puts("inside syncHandlerEL3");

	// read exception information
	esr = readESREL3();
	// mask bits 31:26 ESR.EC to check type of SYNC exception
	//       [1111][1100]00 0000
	esr_ec = esr & 0xFC000000;
	// mask lower 16 bits to get SMC ID
	esr_smc = esr & 0xFFFF; // mask off lower 16 bits
	// set up the memory mapped uart pl011 standard setup at 0x1C090000
	uartSSetup((void*)(0x1C090000));

    // Check what type of SYNC exception it is
	switch (esr_ec)
	{
		// Check if its an SMC exception
		case SMC:
			switch (esr_smc)
			{

			// Check if its an SMC from the non secure world EL1N
			// Pass message to secure
			case SMC_ID_MESGN:
				// SMC_ID_MESGN (0x1) // non secure message ID
				// send message to uart
				uartSTransmitString("EL3 - read\n");
				mesgRead = mesgNRead();
				// put char into string and write to uart
				mesgReadStr[0] = mesgRead;
				// send message to uart
				uartSTransmitString(mesgReadText);
				uartSTransmitString(mesgReadStr);

				// if non secure message, move char from non secure to secure
				moveNtoS();

				// send message to uart
				uartSTransmitString("EL3 - moved message to secure memory\n");
				// dont go back to normal world, go to EL1S
				// Initialise GIC registers for the secure world
				// NEED TO CHANGE THIS SO ONLY INCLUDE WHAT REGS NEED CHANGING HERE
				// NOT ALL NEEDED
				setupGIC600S();
				// puts("Initialised GIC for secure, about to ERET to EL1S...");
				// Perform an ERET to EL1 secure
				ERETtoEL1S();
			    break;
			// Check if its an SMC from the secure world EL1S
			case SMC_ID_MESGS:
				// SMC_ID_MESGN (0x1) // non secure message ID
				// send message to uart
				uartSTransmitString("EL3 - read\n");
				mesgRead = mesgSRead();
				// put char into string and write to uart
				mesgReadStr[0] = mesgRead;
				// send message to uart
				uartSTransmitString(mesgReadText);
				uartSTransmitString(mesgReadStr);

				// if secure message, move char from secure to non secure
				moveStoN();

				// send message to uart
				uartSTransmitString("EL3 - moved message to non secure memory\n");
				// Initialise GIC registers for the non secure world
				// NEED TO CHANGE THIS SO ONLY INCLUDE WHAT REGS NEED CHANGING HERE
				// NOT ALL NEEDED
				setupGIC600N();
				// puts("Initialised GIC for secure, about to ERET to EL1S...");
				// Perform an ERET to EL1 non secure
				ERETtoEL1N();
				break;

			 // NOT USED IN THIS EXAMPLE - ID FOR INCREMENTING MESSAGE IN EL3 FROM NON SECURE
			case SMC_ID_MESGN_INC:
				// SMC_ID_MESGN (0x1) // non secure message ID
				// send message to uart
				uartSTransmitString("EL3 - read\n");
				mesgRead = mesgNRead();
				// put char into string and write to console
				mesgReadStr[0] = mesgRead;
				// send message to uart
				uartSTransmitString(mesgReadText);
				uartSTransmitString(mesgReadStr);

				incN(1); // increment non secure message by 1 char

				// send message to uart
				uartSTransmitString("EL3 - inc by 1 char\n");
				mesgRead = mesgNRead();
				// put char into string and write to console
				mesgReadStr[0] = mesgRead;
				//send message to uart
				uartSTransmitString(mesgReadText);
				uartSTransmitString(mesgReadStr);

				// go back to normal world

			    break;
			default:
				puts("inside syncHandlerEL3 - SMC not valid!");

			}
			break;
		default:
			puts("inside syncHandlerEL3 - but not an SMC exception!");
			break;
	}
return;
}

//------------------------------------
//fiqHandlerEL3
//------------------------------------
// FIQ exception handler for EL3
// This is called by the vector table
// checks for the secure physical timer event
// disables the timer and resets the interrupt
// Note: this example uses secure group 0 (FIQ) for secure EL3
void fiqHandlerEL3(void)
{
uint32_t intid;
// read group 0 interrupt acknowledge register
intid = getIntidAckReg0S();
puts("inside fiqHandlerEL3");
// Secure and non-secure timer events have different interrupt IDs
//   ID30 = Non-secure Physical Timer Event.
//   ID29 = Secure Physical Timer Event
if (intid == 29)
{
	puts("ID is 29"); // must be 29 if here
	flagEL3 = 1;
	stopTimerS();
}
else
{
	// error
}
// set end of interrupt for ID 29 - group 0 register
setIntidEndReg0S(intid);
return;
}
