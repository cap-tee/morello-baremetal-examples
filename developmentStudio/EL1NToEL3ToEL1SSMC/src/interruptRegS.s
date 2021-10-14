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
 .global getIntidAckReg0S
 // Write end of interrupt for secure Group 0
 .global setIntidEndReg0S

 //****************************************************************************
 // FUNCTIONS
 //****************************************************************************

 //------------------------------------------------------------------------------
 // getIntidAckReg0S
 // Description: Get the interrupt information for secure group 0
 //------------------------------------------------------------------------------
  .type getIntidAckReg0S, "function"
getIntidAckReg0S:
   // Check interrupt ID for Group 0
  // EL3 can access EL1 registers, but EL1 can not access EL3 registers
  // this is group 0 register
  // Read interrupt acknowledge register into x0
  MRS       x0, ICC_IAR0_EL1
  RET

 //****************************************************************************
 // setIntidEndReg0S
 // Description: signal end of interrupt for secure group 0
 //****************************************************************************
  .type setIntidEndReg0S, "function"
setIntidEndReg0S:
  // signal end of interrupt for Group 0
  // EL3 can access EL1 registers, but EL1 can not access EL3 registers
  // this is group 0 register Interrupt Controller End Of Interrupt Register
  //// Write x0 to end of interrupt reg
  MSR        ICC_EOIR0_EL1, x0
  RET

