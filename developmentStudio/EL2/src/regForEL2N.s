/*
 ============================================================================
 Name        : regForEL2N.s
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : setting up the registers to go to EL2 non-secure hypervisor
 			   performing an ERET
 			   In addition, functions to manipulate EL2 registers
 			   The first half is in secure memory, the second half is in non
 			   secure memory
 ============================================================================
 */

 //****************************************************************************
 // SECTION AND DEFINES
 //****************************************************************************
   // the first half of this file goes into secure memory region by default
 .section .SECURERegEL2Nsection_ass, "ax"
 .align 3                   // Align to 2^3 byte boundary

 // Program state
 //.equ PS0_EL2, 	0x08    // EL2t  this doesn't load stack into SP reg in this state
 .equ PS2_EL2,	0x09    // EL2h  use this state as it loads the stack pointer addr into SP reg

 // function to set up registers and ERET to EL2N hypervisor
 .global ERETtoEL2N

 //***********************************************************
 // FUNCTIONS
 //***********************************************************
 //-----------------------------------------------------------
 // ERETtoEL2N
 //-----------------------------------------------------------
 .type ERETtoEL2N, "function"
ERETtoEL2N:

  // Disable trapping
  MSR      CPTR_EL3, xzr // Clear all trap bits
  MSR      CPTR_EL2, xzr // Clear all trap bits

  // SET NORMAL WORLD HERE
  // Set SCR_EL3 - secure configuration register to determine what happens at lower exception levels
  MOV      w1, #1              //  NS (#0-is Secure state, #1 is non secure  - hypervisor)
  ORR      w1, w1, #(1 << 8)    // HCE=1         HVC Hypervisor call instructions are enabled
  ORR 	   w1, w1, #(1 << 11)  // set ST bit (disable trapping of timer control registers)
  ORR	   w1, w1, #(1 << 10)  // set RW bit (next lower EL in aarch64 mode)
  							   // [7] = 0 SMC instructions are enabled at EL3, EL2 and EL1.
  //we do not want exceptions to be routed to EL3
  //we would like the timer interrupt to be routed to EL1N vector table
  //so do not set these exception bits:
  ORR      w1, w1, #(1 << 3)   // Set EA bit (SError routed to EL3)
  ORR      w1, w1, #(1 << 2)   // Set FIQ bit (FIQs routed to EL3)
  ORR      w1, w1, #(1 << 1)   // Set IRQ bit (IRQs routed to EL3)
  MSR      SCR_EL3, x1
  ISB

  // Set SCTLRs for EL1/2 to safe values before set up of EL2 and EL1
  // ------------------------------------
  MSR     SCTLR_EL2, xzr //reg turns on/off MMU EL2
  MSR     SCTLR_EL1, xzr //reg turns on/off MMU EL1

  LDR      x0, =EL2N_hypervisor_entry // where to branch to when exception completes
  LDR	  x1, =PS2_EL2 		          //the program state
  MSR 	  ELR_EL3, x0 			      // where to branch to when exception completes
  MSR      SPSR_EL3, x1 			  // set the program state for this point to a known value
  ISB
  ERET  //This is ERET - LEAVES EL3

 //****************************************************************************
 // SECTION AND DEFINES
 //****************************************************************************
 // need to put into non secure memory region by linker script
 .section .NONSECUREel2entrysection_ass, "ax"
 .align 3                   // Align to 2^3 byte boundary

 // function to initialise entry to EL2N, sets up stack and registers and branches
 // to main EL2N c code
 .global EL2N_hypervisor_entry
 //install vector table for EL2
 .global installVectorsEL2N
 // function to stop EL1 from changing the mmu/memory settings
.global disableEL1N_mmuChange
// function to get the exception information for EL2
.GLOBAL readESREL2

 //***********************************************************
 // FUNCTIONS
 //***********************************************************

 //----------------------------------------------------------------------------
 // readESREL2
 // Description: Get the exception information for EL2
 //----------------------------------------------------------------------------
  .type readESREL2, "function"
readESREL2:
  // Holds syndrome information for an exception taken to EL2.
  // Read this to get SMC ID information
  MRS       x0, ESR_EL2  //Exception Syndrome Register, EL2
  RET

 // ------------------------------------------------------------
 // installVectorsEL2N  - install vectore table for EL2N hypervisor
 // ------------------------------------------------------------
 .type installVectorsEL2N, "function"
installVectorsEL2N:

  // Install EL2N vector table here for exceptions and interrupts
  // VBAR_EL2 points to the vector table vectorsEL2N
  .global vectorsEL2N
  LDR  x0, =vectorsEL2N
  MSR  VBAR_EL2, x0
  ISB
  RET

 //-------------------------------------------
 // disableEL1N_mmuChange
 // function to stop EL1 from changing the mmu/memory settings
 //-------------------------------------------
  .type disableEL1N_mmuChange, "function"
disableEL1N_mmuChange:

  MRS X0, HCR_EL2  //get current register value

  // trap EL1 memory operations to EL2 - so EL1 can't mess with memory
  // check this setting, check if other traps needed
  // bit[26]  TVM - the following registers are trapped to EL2 and reported using EC syndrome value 0x18:
  // SCTLR_EL1, TTBR0_EL1, TTBR1_EL1, TCR_EL1, ESR_EL1, FAR_EL1, AFSR0_EL1, AFSR1_EL1, MAIR_EL1, AMAIR_EL1, CONTEXTIDR_EL1.
  ORR      x0, x0, #(1 << 26)                 // bit[26]  TVM traps writes
  ORR      x0, x0, #(1 << 30)					// TRVM, bit [30] traps reads

  MSR      HCR_EL2, x0
  ISB

  RET

 // ------------------------------------------------------------
 // EL2 hypervisor (non secure) AArch64
 // ------------------------------------------------------------
 .type EL2N_hypervisor_entry, "function"
EL2N_hypervisor_entry:
  // ENTERS EL2N HERE
  // Get stack address defined in linker script
  // Needs this because it looses the stack on entry to EL2, and doesn't work without it
  ADRP x0, _stack_el2n_limit
  MOV  sp, x0

  // Ensures that all instructions complete before branching
  ISB
  // Branch to the main EL2 hypervisor code
  B        el2nmain


