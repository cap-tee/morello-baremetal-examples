/*
 ============================================================================
 Name        : regForEL1S.s
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : setting up the registers to go to EL1 secure and performing an ERET
 ============================================================================
 */

 //****************************************************************************
 // SECTION AND DEFINES
 //****************************************************************************

 // goes into secure memory region by default
 .section .SECURERegEL1Ssection_ass, "ax"
 .align 3                   // Align to 2^3 byte boundary

 // Program state
 // .equ PS0_EL1S, 0x04 // EL1S - program state 0
 .equ PS1_EL1S, 0x05 // EL1S - program state 1, doesn't work in program state 0

 // function to set up registers and ERET to EL1S
 .global ERETtoEL1S
 // function to initialise entry to EL1S, sets up stack and registers and branches to main
 // EL1S c code
 .global EL1S_secure_entry


 //***********************************************************
 // FUNCTIONS
 //***********************************************************
 //-----------------------------------------------------------
 // ERETtoEL1S
 //-----------------------------------------------------------
 .type ERETtoEL1S, "function"
ERETtoEL1S:

  // Disable trapping of CPTR_EL3 access
  MSR      CPTR_EL3, xzr // Clear all trap bits
  MSR      CPTR_EL2, xzr // BBB

  // SET SECURE WORLD HERE
  // Set SCR_EL3 - secure configuration register to determine what happens at lower exception levels
  MSR      SCR_EL3, xzr
  MOV      w1, #0              // NS (#0-EL0/EL1 are in Secure state, #1 is non secure)
  ORR 	   w1, w1, #(1 << 11)  // set ST bit (disable trapping of timer control registers)
  ORR	   w1, w1, #(1 << 10)  // set RW bit (next lower EL in aarch64)
  							   // [7] = 0 SMC instructions are enabled at EL3, EL2 and EL1.
  // Route FIQ to EL1 handler, so do not set these bits
  // we would like the timer interrupt to be routed to EL1S vector table
  // ORR      x0, x0, #(1 << 8)                // HCE=1         HVC instructions are enabled BBB
  // ORR      w1, w1, #(1 << 3)   // Set EA bit (SError routed to EL3)
  // ORR      w1, w1, #(1 << 2)   // Set FIQ bit (FIQs routed to EL3)
  // ORR      w1, w1, #(1 << 1)   // Set IRQ bit (IRQs routed to EL3)
  MSR      SCR_EL3, x1
  ISB



  // **ADDED THIS SECTION TO REMOVE DEBUGGER MMU WARNINGS*******************
 // ADDED TO REMOVE DEBUGGER MMU WARNINGS FOR TTBR1 EL1 AND VTTBR EL2
 // Although not using stage 2 translation, the debugger MMU complains if VTTBR is not set with sensible values
 //*********************************************************
 // Set SCTLR to safe values
 MSR     SCTLR_EL2, xzr
 // set VTTBR_EL2, and TTBR1_EL1 with dummy empty tables
 MSR     VTTBR_EL2, xzr
 LDR      x0, =tt_dummy_baseS
 MSR     VTTBR_EL2, x0
 MSR    TTBR1_EL1, x0
 ISB
 // set up VTCR_EL2, so there are no conflicts between TGO,SLO and T0SZ, to please the debugger
 MOV      x0, #0x19                        // T0SZ=0b011001 Limits VA space to 39 bits, translation starts @ l1
 ORR      x0, x0, #(0x1 << 6)              // SLO[7:6] 01
								           // TGO 00
  MSR      VTCR_EL2, x0
  ISB
 //*****************************************************************


  // Clear interrupt masks
  MSR      DAIFClr, #0xF

  // Initialize SCTLR_EL1 before we can enter EL1
  MSR 	   SCTLR_EL1, xzr

  LDR      x0, =EL1S_secure_entry // where to branch to when exception completes
  LDR	   x1, =PS1_EL1S //the program state
  MSR 	   ELR_EL3, x0   // where to branch to when exception completes
  MSR      SPSR_EL3, x1  // set the program state for this point to a known value
  ISB
  ERET  // This is ERET - LEAVES EL3

//********************************************************************************

  // ------------------------------------------------------------
  // EL1S_secure_entry  (For AArch64)
  // ------------------------------------------------------------
  .align 3                   // Align to 2^3 byte boundary
  .type EL1S_secure_entry, "function"

EL1S_secure_entry:
  // ENTERS EL1S HERE
  // Get stack address defined in linker script
  // Needs this because it looses the stack on entry to EL1, and doesn't work without it
  ADRP x0, _stack_el1s_limit
  MOV  sp, x0

  // don't need vector table for this example
  // Install EL1S vector table here for exceptions and interrupts
  //.global vectorsEL1S
  // LDR  x0, =vectorsEL1S
  // MSR  VBAR_EL1, x0
  // ISB

  // We must ensure that floating point register accesses are not trapped
  // since the c library for AArch64-v8A uses them
  MOV      x0, #(0x3 << 20)
  MSR      CPACR_EL1, x0

  // Ensures that all instructions complete before branching
  ISB
  // Branch to the main EL1 secure code
  B        el1smain

  //**ADDED THIS SECTION TO REMOVE DEBUGGER MMU WARNINGS*******************
  // set VTTBR_EL2, and TTBR1_EL1 with dummy empty tables
  // Need to set in EL3 because can not access EL2 registers in EL1
  .section  .SECUREttdummysection_ass,"ax"
  .align 12

  .global tt_dummy_baseS
tt_dummy_baseS:
  .fill 4096 , 1 , 0
//***********************************************************************
