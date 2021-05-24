/*
 ============================================================================
 Name        : smcEL1N.s
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : SMC calls from EL1N

 ============================================================================
 */

//*****************************************
// SECTION
//*****************************************
// go into non secure section of memory
.section  .NONSECURESMC_ass,"ax"         // Define an executable ELF section
.align 3                   // Align to 2^3 byte boundary

//*****************************************
// DEFINES
//*****************************************
// Define memory region of non secure character
.equ CHAR_ADDR,			  0xFFFC1000
// Define SMC ID to increment EL1N character
.equ SMC_ID_MESGN,		  0x01


//********************************************
// FUNCTIONS
//*******************************************

//-------------------------------------------------------------
// smcPassMesgN
// use the SMC instruction to call to EL3 SYNC exception handler
// pass immediate value ID to pass the EL1N character
//--------------------------------------------------------------
  .global smcPassMesgN
  .type smcPassMesgN, "function"
smcPassMesgN:
SMC #SMC_ID_MESGN
RET



