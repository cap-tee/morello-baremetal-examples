
/*
 ============================================================================
 Name        : exceptionHandlerFuncsEL3.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : EL3 exception handler functions
 	 	 	   Contains a SYNC exception handler to process SMC
 ============================================================================
 */

//*****************************************
// DEFINES
//*****************************************
//define SYNC exception IDs for SMC calls
#define SMC 		 (0x5C000000)     // exception ID for an SMC call [0101 1100]00 0000 in ESR_EL3 register
// IDs for moving a message between worlds - user defined
#define SMC_ID_MESGN (0x1) 			  // non secure message ID to move char from defined non secure memory region
#define SMC_ID_MESGS (0x2) 			  // secure message ID to move char from defined secure memory region

//*****************************************
// INCLUDES
//*****************************************
#include <stdio.h>
#include <stdlib.h>
//functions used
#include "exceptionHandlerFuncsEL3.h"
#include "mesgEL3.h" //include message functions for manipulating a message across secure and non secure worlds
#include "uartS.h" //include writing messages to secure uart
extern uint32_t readESREL3(void);//exception information for EL3 exceptionRegEL3.s file

//******************************************
// Functions
//******************************************
//------------------------------------
// syncHandlerEL3
// SYNC exception handler for EL3 from lower EL
// This is called by the vector table
//------------------------------------
void syncHandlerEL3(void)
{
	uint32_t esr; //esr register - exception
	uint32_t esr_ec; //ESR.EC[31:26] type of exception
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

    // Check what type of SYNC exception it is
	switch (esr_ec)
	{
		// Check if its an SMC exception
		case SMC:
			switch (esr_smc)
			{
			// Check if its an SMC from the non secure world EL1N
			case SMC_ID_MESGN:
				// SMC_ID_MESGN (0x1) // non secure message ID
				// set up the memory mapped uart pl011 standard setup at 0x1C090000
				uartSSetup((void*)(0x1C090000));
				// send message to uart
				uartSTransmitString("EL3\n");
				mesgRead = mesgNRead();
				// put char into string and write to console
				mesgReadStr[0] = mesgRead;
				// send message to uart
				uartSTransmitString(mesgReadText);
				uartSTransmitString(mesgReadStr);

				//increment non secure message by 1 char
				incN(1);

				// send message to uart
				uartSTransmitString("Inc1\n");
				mesgRead = mesgNRead();
				// put char into string and write to console
				mesgReadStr[0] = mesgRead;
				// send message to uart
				uartSTransmitString(mesgReadText);
				uartSTransmitString(mesgReadStr);
				// return back to the normal world
			    break;

			// Check if its an SMC from the secure world EL1S
			case SMC_ID_MESGS:
				// SMC_ID_MESGS (0x2) // secure message ID
				// set up the memory mapped uart pl011 standard setup at 0x1C090000
				uartSSetup((void*)(0x1C090000));
				// send message to uart
				uartSTransmitString("EL3\n");
				mesgRead = mesgSRead();
				// put char into string and write to console
				mesgReadStr[0] = mesgRead;
				// send message to uart
				uartSTransmitString(mesgReadText);
				uartSTransmitString(mesgReadStr);

				//increment non secure message by 1 char
				incS(1);

				// send message to uart
				uartSTransmitString("Inc1\n");
				mesgRead = mesgSRead();
				// put char into string and write to console
				mesgReadStr[0] = mesgRead;
				// send message to uart
				uartSTransmitString(mesgReadText);
				uartSTransmitString(mesgReadStr);
				// return back to the secure world
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
