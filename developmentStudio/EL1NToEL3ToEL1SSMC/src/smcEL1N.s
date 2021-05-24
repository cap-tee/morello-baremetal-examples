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
.align 3                                 // Align to 2^3 byte boundary

//*****************************************
// DEFINES
//*****************************************
// Define memory region of non secure character
.equ CHAR_ADDR,			  0xFFFC1000
// Define SMC ID to pass message to secure EL1N, increment EL1N character in EL1S/EL1N
.equ SMC_ID_MESGN,		  0x1
// Define SMC ID to increment EL1N character in EL3
.equ SMC_ID_MESGN_INC,		  0x2

//********************************************
// FUNCTIONS
//*******************************************
//---------------------------------------------
// smcPassMesgN
// use the SMC instruction to call to EL3 SYNC exception handler
// pass immediate value ID to copy the EL1N character
//---------------------------------------------
  .global smcPassMesgN
  .type smcPassMesgN, "function"
smcPassMesgN:
SMC SMC_ID_MESGN
RET

//---------------------------------------------
// smcPassMesgN_inc
// use the SMC instruction to call to EL3 SYNC exception handler
// pass immediate value ID to increment char in EL3
//---------------------------------------------
  .global smcPassMesgN_inc
  .type smcPassMesgN_inc, "function"
smcPassMesgN_inc:
SMC SMC_ID_MESGN_INC
RET

