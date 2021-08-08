/*
 ============================================================================
 Name        : regForEL1N.s
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : setting up the registers to go to EL1 normal and performing an ERET
 ============================================================================
 */

 //****************************************************************************
 // SECTION AND DEFINES
 //****************************************************************************
   // this file goes into NON secure memory region
 .section .NONSECUREeretEL1Nsection_ass_el2, "ax"
 .align 3                   // Align to 2^3 byte boundary

 // Program state
 //.equ PS0_EL1N, 0x04 // EL1N program state 0
 .equ PS0_EL1N, 0x05 // EL1N program state 1 - interrupts only work in this state, loses stack otherwise

 // function to set up registers and ERET to EL1S
 .global ERETtoEL1N

 //***********************************************************
 // FUNCTIONS
 //***********************************************************

 //-----------------------------------------------------------
 // ERETtoEL1N
 //-----------------------------------------------------------
 .type ERETtoEL1N, "function"
ERETtoEL1N:
 LDR      x0, =el1_normal_entry // where to branch to when exception completes
 LDR	  x1, =PS0_EL1N 		//the program state
 MSR 	  ELR_EL2, x0 			// where to branch to when exception completes
 MSR      SPSR_EL2, x1 			// set the program state for this point to a known value
 ISB
 ERET  //This is ERET - LEAVES EL2

 //****************************************************************************
 // SECTION AND DEFINES
 //****************************************************************************
 // need to put into non secure memory region by linker script
 .section .NONSECUREel1entrysection_ass_el1, "ax"
 .align 3                   // Align to 2^3 byte boundary


 // function to get the exception information for EL1N
 .global readESREL1N
 // function to initialise entry to EL1N, sets up stack and registers and branches to main
 // EL1N c code
 .global EL1N_normal_entry

 //***********************************************************
 // FUNCTIONS
 //***********************************************************
  //----------------------------------------------------------------------------
 // readESREL1N
 // Description: Get the exception information for EL1
 //----------------------------------------------------------------------------
  .type readESREL1N, "function"
readESREL1N:
  // Holds syndrome information for an exception taken to EL1.
  // Read this to get exception information
  MRS       x0, ESR_EL1  //Exception Syndrome Register, EL1
  RET

 // ------------------------------------------------------------
 // EL1 normal (non secure) AArch64
 // ------------------------------------------------------------
 .type el1_normal_entry, "function"
el1_normal_entry:
  // ENTERS EL1N HERE
  // Get stack address defined in linker script
  // Needs this because it looses the stack on entry to EL1, and doesn't work without it
  ADRP x0, _stack_el1n_limit
  MOV  sp, x0
  // Ensures that all instructions complete before branching
  ISB
  // Branch to the main EL1 normal code
  B        el1nmain
