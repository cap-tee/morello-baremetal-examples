/*
 ============================================================================
 Name        : regForEL1N.s
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : setting up the registers to go to EL1 normal and performing an ERET
 ============================================================================
 */
 // Program state
 //.equ PS0_EL1N, 0x04 // EL1N program state 0
 .equ PS0_EL1N, 0x05 // EL1N program state 1 - interrupts only work in this state, loses stack otherwise

 .global ERETtoEL1N
 .type ERETtoEL1N, "function"
ERETtoEL1N:

  // Disable trapping
  MSR      CPTR_EL3, xzr // Clear all trap bits
  MSR      CPTR_EL2, xzr // Clear all trap bits

  // SET NORMAL WORLD HERE
  // Set SCR_EL3 - secure configuration register to determine what happens at lower exception levels
  MOV      w1, #1              //  NS (#0-EL0/EL1 are in Secure state, #1 is non secure)
  ORR 	   w1, w1, #(1 << 11)  // set ST bit (disable trapping of timer control registers)
  ORR	   w1, w1, #(1 << 10)  // set RW bit (next lower EL in aarch64 mode)
  //we do not want exceptions to be routed to EL3
  //we would like the timer interrupt to be routed to EL1N vector table
  //so do not set these exception bits:
  //ORR      w1, w1, #(1 << 3)   // Set EA bit (SError routed to EL3)
  //ORR      w1, w1, #(1 << 2)   // Set FIQ bit (FIQs routed to EL3)
  //ORR      w1, w1, #(1 << 1)   // Set IRQ bit (IRQs routed to EL3)
  MSR      SCR_EL3, x1
  ISB

  //**ADDED THIS SECTION TO REMOVE DEBUGGER MMU WARNINGS*******************
  //ADDED TO REMOVE DEBUGGER MMU WARNINGS FOR TTBR1 EL1 AND VTTBR EL2
  // Although not using stage 2 translation, the debugger MMU complains
  // if VTTBR is not set with sensible values
  // Need to set in EL3 because can not access EL2 registers in EL1
  //**********************************************************************
  // Set SCTLR to safe values
  MSR     SCTLR_EL2, xzr
  //set VTTBR_EL2, and TTBR1_EL1(upper virtual address space translation) with dummy empty tables
  MSR     VTTBR_EL2, xzr
  LDR      x0, =tt_dummy_baseN
  MSR     VTTBR_EL2, x0
  MSR    TTBR1_EL1, x0
  ISB
  //set up VTCR_EL2, so there are no conflicts between TGO,SLO and T0SZ, to please the debugger
  MOV      x0, #0x19                        // T0SZ=0b011001 Limits VA space to 39 bits, translation starts @ l1
  ORR      x0, x0, #(0x1 << 6)              // SLO[7:6] 01
								            //TGO 00
  MSR      VTCR_EL2, x0
  ISB

  //*****************************************************************
  //do we need this to get non secure timer accessible
  // Configure HCR_EL2 - the hypervisor configuration register
  // ---------------------------------------------------------
  MRS      x0, HCR_EL2
  MOV	   x1, #(1 << 31)
  ORR	   x0, x0, x1
  MSR      HCR_EL2, x0
  // Configure CNTHCTL_EL2 - the Counter-timer Hypervisor Control register
  // ---------------------------------------------------------------------
  // we need to enable timer register access for lower EL levels
  MRS      x0, CNTHCTL_EL2
  ORR	   x0, x0, #(1 << 1)
  ORR	   x0, x0, #1
  MSR	   CNTHCTL_EL2, x0
 //***************************************************************************

 // Clear interrupt masks
 MSR      DAIFClr, #0xF
 MSR      DAIF, xzr

 // Initialize SCTLR_EL1 before we can enter EL1
 MSR 	  SCTLR_EL1, xzr

 LDR      x0, =el1_normal_entry // where to branch to when exception completes
 LDR	  x1, =PS0_EL1N 		//the program state
 MSR 	  ELR_EL3, x0 			// where to branch to when exception completes
 MSR      SPSR_EL3, x1 			// set the program state for this point to a known value
 ISB
 ERET  //This is ERET - LEAVES EL3

 // ------------------------------------------------------------
 // EL1 normal (non secure) AArch64
 // ------------------------------------------------------------
 //need to put into non secure memory region
 .section .NONSECUREel1entrysection_ass, "ax"
 .align 3                   // Align to 2^3 byte boundary

 .global el1_normal_entry
 .type el1_normal_entry, "function"
el1_normal_entry:
  // ENTERS EL1N HERE
  // Get stack address defined in linker script
  // Needs this because it looses the stack on entry to EL1, and doesn't work without it
  ADRP x0, _stack_el1n_limit
  MOV  sp, x0

  // Install EL1N vector table here for exceptions and interrupts
  // VBAR_EL1 points to the vector table vectorsEL1N
  .global vectorsEL1N
  LDR  x0, =vectorsEL1N
  MSR  VBAR_EL1, x0
  ISB

  // We must ensure that floating point register accesses are not trapped
  // since the c library for AArch64-v8A uses them
  MOV      x0, #(0x3 << 20)
  MSR      CPACR_EL1, x0

  // Ensures that all instructions complete before branching
  ISB
  // Branch to the main EL1 normal code
  B        el1nmain


  //**ADDED THIS SECTION TO REMOVE DEBUGGER MMU WARNINGS*******************
  // set VTTBR_EL2, and TTBR1_EL1 with dummy empty tables
  // Need to set in EL3 because can not access EL2 registers in EL1
  //need to put in non secure memory so define a section
  .section  .NONSECUREttdummysection_ass,"ax"
  .align 12

  .global tt_dummy_baseN
tt_dummy_baseN:
  .fill 4096 , 1 , 0
//***********************************************************************
