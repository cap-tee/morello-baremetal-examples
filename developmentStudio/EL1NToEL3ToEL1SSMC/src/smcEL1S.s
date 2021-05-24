/*
 ============================================================================
 Name        : smcEL1S.s
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : SMC calls from EL1S

 ============================================================================
 */

//*****************************************
// SECTION
//*****************************************
// go into non secure section of memory
.section  .SECURESMC_ass,"ax"         // Define an executable ELF section
.align 3                   // Align to 2^3 byte boundary

//*****************************************
// DEFINES
//*****************************************
// Define memory region of secure character
.equ CHAR_ADDR,			  0xBFFC1000
// Define SMC ID to pass message to secure EL1N, increment EL1N character in EL1S/EL1N
.equ SMC_ID_MESGS,		  0x20


//********************************************
// FUNCTIONS
//*******************************************

//-------------------------------------------------------------
//smcPassMesgS
// use the SMC instruction to call to EL3 SYNC exception handler
// pass immediate value ID to pass the EL1S character
//--------------------------------------------------------------
  .global smcPassMesgS
  .type smcPassMesgS, "function"
smcPassMesgS:
SMC SMC_ID_MESGS
RET


