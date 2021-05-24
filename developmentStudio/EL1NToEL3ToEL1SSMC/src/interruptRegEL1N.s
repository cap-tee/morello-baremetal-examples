/*
 ============================================================================
 Name        : interruptRegEL1N.s
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : Group 1 interrupt registers - In this example used for non secure group 1 - IRQ
             : functions to read and write to interrupt registers from non secure world

 ============================================================================
 */

 //****************************************************************************
 // SECTION AND DEFINES
 //****************************************************************************
 // This needs to go in non secure memory by linker script
 .section .NONSECUREgicsection_ass, "ax"
 .align 3                   // Align to 2^3 byte boundary

 // Read interrupt ID for Group 1
  .global readIAR1N
 // Write end of interrupt for Group 1
  .global writeEOIR1N

 //****************************************************************************
 // FUNCTIONS
 //****************************************************************************

 //****************************************************************************
 // readIAR1N
 // Description: Get the interrupt information for group 1
 //****************************************************************************
 .type readIAR1N, "function"
readIAR1N:
  // Check interrupt ID for Group 1, Note group 0 (secure) has a different register
  // this is group 1 register
  MRS       x0, ICC_IAR1_EL1  // Read ICC_IAR1_EL1 into x0 - INTERUPT ACKNOWLEDGE REGISTER
  RET

 //****************************************************************************
 // writeEOIR1N
 // Description: signal end of interrupt for Group 1
 //****************************************************************************
 .type writeEOIR1N, "function"
writeEOIR1N:
  // This is group 1 register Interrupt Controller End Of Interrupt Register
  // Note group 0 (secure) has a different register
  MSR        ICC_EOIR1_EL1, x0 // Write x0 to ICC_EOIR1_EL1
  RET

