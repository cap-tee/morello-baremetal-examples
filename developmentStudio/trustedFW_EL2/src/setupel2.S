/*
 ============================================================================
 Name        : setupel2.S
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : basic set up at EL2, this does not include any capability registers
 				vector tables, or setting up of the mmu
 ============================================================================
 */
 //****************************************************************************
 // SECTION AND DEFINES
 //****************************************************************************
  .align 16

  // set up function for EL2
  .global _startel2
  .type _startel2, @function


 //***********************************************************
 // FUNCTIONS
 //***********************************************************
 //-----------------------------------------------------------
 // _startel2
 //-----------------------------------------------------------
  _startel2:
  // Get stack address defined in linker script
  ADRP x0, _stack_el2n_limit
  // ensure sp aligned to 16B boundary
  AND  x0, x0, ~15
  MOV  sp, x0

  // disable trapping of floating point reg q0 - needed for long strings
  // disable trapping of morello specific instructions (uses a reserved armv8 bit) - needed for printf function
  // TFP, bit [10] when set to 1 Traps execution of instructions which access the Advanced SIMD and floating-point functionality,
  // TC, bit [9] when set to 1 traps morello instructions like using capability registers which printf function uses
  MRS      x0, CPTR_EL2  // Read
  // create mask 1111 1001 1111 1111
  MOV x2, #0xF9FF
  MOVK x2, #0xFFFF,LSL #16
  // and mask with contents of register to set bit[10] and bit[9] to zero
  AND      x0, x0, x2 // disable
  MSR      CPTR_EL2, x0 // Write

  ISB

  // set up c libraries and branch to main
  .global main
  B main


