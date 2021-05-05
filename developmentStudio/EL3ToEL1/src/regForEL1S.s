/*
 ============================================================================
 Name        : regForEL1S.s
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : setting up the registers to go to EL1 secure and performing an ERET
 ============================================================================
 */
// Program state
.equ AArch64_EL1_SP1, 0x05 // EL1S

  .global ERETtoEL1S
  .type ERETtoEL1S, "function"
ERETtoEL1S:

  // Disable trapping of CPTR_EL3 accesses or use of Adv.SIMD/FPUl
  MSR      CPTR_EL3, xzr // Clear all trap bits

  // SET SECURE WORLD HERE
  // Set SCR_EL3 - secure configuration register to determine what happens at lower exception levels
  MOV      w1, #0              //  NS (#0-EL0/EL1 are in Secure state, #1 is non secure)
  ORR 	   w1, w1, #(1 << 11)  // set ST bit (disable trapping of timer control registers)
  ORR	   w1, w1, #(1 << 10)  // set RW bit (next lower EL in aarch64)
  ORR      w1, w1, #(1 << 3)   // Set EA bit (SError routed to EL3)
  ORR      w1, w1, #(1 << 2)   // Set FIQ bit (FIQs routed to EL3)
  ORR      w1, w1, #(1 << 1)   // Set IRQ bit (IRQs routed to EL3)
  MSR      SCR_EL3, x1
  ISB

  // Clear interrupt masks
  MSR      DAIFClr, #0xF

  // Initialize SCTLR_EL1 before we can enter EL1
  MSR 	   SCTLR_EL1, xzr

  LDR      x0, =el1_secure_entry_aarch64 // where to branch to when exception completes
  LDR	   x1, =AArch64_EL1_SP1 //the program state
  MSR 	   ELR_EL3, x0 // where to branch to when exception completes
  MSR      SPSR_EL3, x1 // set the program state for this point to a known value
  ISB
  ERET  //This is ERET - LEAVES EL3

  // ------------------------------------------------------------
  // EL1 secure AArch64
  // ------------------------------------------------------------
  .align 3                   // Align to 2^3 byte boundary
  .global el1_secure_entry_aarch64
  .type el1_secure_entry_aarch64, "function"

el1_secure_entry_aarch64:
  // ENTERS EL1S HERE
  // Get stack address defined in linker script
  // Needs this because it looses the stack on entry to EL1, and doesn't work without it
  ADRP x0, _stack_el1s_limit
  MOV  sp, x0

  // We must ensure that floating point register accesses are not trapped
  // since the c library for AArch64-v8A uses them
  MOV      x0, #(0x3 << 20)
  MSR      CPACR_EL1, x0

  // Ensures that all instructions complete before branching
  ISB
  // Branch to the main EL1 secure code
  B        el1smain
// ------------------------------------------------------------
// End of file
// ------------------------------------------------------------
