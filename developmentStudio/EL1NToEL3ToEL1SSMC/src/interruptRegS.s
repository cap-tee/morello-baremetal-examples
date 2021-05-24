/*
 ============================================================================
 Name        : interruptRegS.s
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : Group 0 secure interrupt registers - FRQ
             : functions to read and write to interrupt registers from secure
               world, EL3 and EL1S

 ============================================================================
 */

 //****************************************************************************
 // SECTION AND DEFINES
 //****************************************************************************

 // This goes in secure memory automatically
 .section .SECUREintregsection_ass, "ax"
 .align 3                   // Align to 2^3 byte boundary

 // Read interrupt ID for secure Group 0
 .global readIAR0S
 // Write end of interrupt for secure Group 0
 .global writeEOIR0S

 //****************************************************************************
 // FUNCTIONS
 //****************************************************************************

 //****************************************************************************
 // readIAR0S
 // Description: Get the interrupt information for secure group 0
 //****************************************************************************
  .type readIAR0S, "function"
readIAR0S:
  // Check interrupt ID for Group 0
  // EL3 can access EL1 registers, but EL1 can not access EL3 registers
  // this is group 0 register
  MRS       x0, ICC_IAR0_EL1  // Read ICC_IAR0_EL1 into x0 - INTERUPT ACKNOWLEDGE REGISTER
  RET

 //****************************************************************************
 // writeEOIR0S
 // Description: signal end of interrupt for secure group 0
 //****************************************************************************
  .type writeEOIR0S, "function"
writeEOIR0S:
  // signal end of interrupt for Group 0
  // EL3 can access EL1 registers, but EL1 can not access EL3 registers
  // this is group 0 register Interrupt Controller End Of Interrupt Register
  MSR        ICC_EOIR0_EL1, x0 // Write x0 to ICC_EOIR0_EL1
  RET

