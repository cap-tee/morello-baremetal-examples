/*
 ============================================================================
 Name        : regForEL3.s
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : Sets registers and installs the vector table at EL3 ready
 			   to catch the timer interrupt
 ============================================================================
 */

  .global setRegInstallVectors
  .type setRegInstallVectors, "function"
setRegInstallVectors:

  // Clear all trap bits
  MSR      CPTR_EL3, xzr // Clear all trap bits
  
  // Configure SCR_EL3 to route exceptions to EL3
  // This will mean that the timer interrupt exception will route to EL3 vector table
  MOV      w1, #0              //  NS (#0 is secure, #1 is non secure)
  ORR      w1, w1, #(1 << 3)   // Set EA bit (SError routed to EL3)
  ORR      w1, w1, #(1 << 2)   // Set FIQ bit (FIQs routed to EL3)
  ORR      w1, w1, #(1 << 1)   // Set IRQ bit (IRQs routed to EL3)
  MSR      SCR_EL3, x1

  // Install EL3 vector table here for exceptions and interrupts
  //VBAR_EL3 points to the vector table
  .global vectorsEL3
  LDR  x0, =vectorsEL3
  MSR  VBAR_EL3, x0
  ISB
  
  // Clear interrupt masks
  MSR      DAIFClr, #0xF

  //Return here!
  RET

