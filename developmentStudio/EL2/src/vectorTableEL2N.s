/*
 ============================================================================
 Name        : vectorTableEL2N.s
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : The vector table here is for EL2N - hypervisor
 			   All exceptions routed to EL2N come here. The exception type, and where it
 			   has come from determines which entry in the table it is routed to.
 			   This follows the standard vector table format for the AArch64 setup
 			   see https://developer.arm.com/documentation/100933/0100/AArch64-exception-vector-table
 			   Each exception level (EL3, EL2N, EL1S/N) has its own vector table.
			   The base address for the EL2 table is given by VBAR_EL2, which in this project is defined in regForEL1N.s
			   Each entry has a defined offset from this base address of 128 bytes.
			   Additionally, each table has a defined number of entries in a defined format of 4 sets of 4 entries to
			   cover the different types, and where the exception is being taken from.
			   The table contains branch instructions to an exception handler function, but it is possible to write
               up to 32 instructions here for each entry.
               In the specific table defined here, entries not used are branch to self (B .)
               Only one label is defined here syncVectorHandler, which branches off to the syncHandlerEL2N function
               defined in exceptionHandlerFuncsEL2N
 ============================================================================
 */

 //****************************************************************************
 // SECTION AND DEFINES
 //****************************************************************************
 // Define a non secure memory region for the vector table
 // This section goes into the non secure memory region by the linker script
  .section  .NONSECUREvectortableel2N_ass,"ax"
  .align 12

 // Vector Table function for EL2N - used outside this file
  .global vectorsEL2N
 // Exception handler function for EL2N - located in exceptionHandlerFuncsEL1N.c
  .global syncHandlerEL2N
 
 //****************************************************************************
 // FUNCTIONS
 //****************************************************************************

 //****************************************************************************
 // vectorsEL2N
 // Description: Vector table entries
 //****************************************************************************
  .type vectorsEL2N, "function"
vectorsEL2N:

// ------------------------------------------------------------
// Current EL with PS0_EL2, 	0x08
// This block of four entries are associated with exceptions
// that have been triggered from EL2N whilst in program state 0
// ------------------------------------------------------------

  .balign 128
sp0_currentEL_SYNC:
  B        .                    //        Synchronous

  .balign 128
sp0_currentEL_IRQ:
  B        .                    //        IRQ

  .balign 128
sp0_currentEL_FIQ:
  B        . 					//        FIQ

  .balign 128
sp0_currentEL_SError:
  B        .                    //        SError

// ------------------------------------------------------------
// Current EL with SPx - PS2_EL2,	0x09
// This block of four entries are associated with exceptions
// that have been triggered from EL2N whilst in program state 1
// The current example code uses program state 1.
// The non secure timer interrupt is set up to generate an irq exception.
// ------------------------------------------------------------

  .balign 128
spx_currentEL_SYNC:
  B        .                    //        Synchronous

  .balign 128
spx_currentEL_IRQ:
  B        .				     //        IRQ

  .balign 128
spx_currentEL_FIQ:
  B        . 					//        FIQ

  .balign 128
spx_currentEL_SError:
  B        .                    //        SError

// ------------------------------------------------------------
// Lower EL using AArch64
// This block of four entries are associated with exceptions
// that have been triggered from a lower EL, which would be EL1
// using AArch64.
// The current example code does not use EL0.
// ------------------------------------------------------------

  .balign 128
aarch64_lowerEL_SYNC:
  B        syncVectorHandler                    //		  Synchronous exceptions from EL1 memory accesses come here

  .balign 128
aarch64_lowerEL_IRQ:
  B        .                    //        IRQ

  .balign 128
aarch64_lowerEL_FIQ:
  B        . 					//        FIQ

  .balign 128
aarch64_lowerEL_SError:
  B        .                    //        SError

// ------------------------------------------------------------
// Lower EL using AArch32
// This block of four entries are associated with exceptions
// that have been triggered from a lower EL, which would be EL1
// using AArch32.
// The current example code does not use EL0.
// ------------------------------------------------------------

  .balign 128
aarch32_lowerEL_SYNC:
  B        .					//		Synchronous

  .balign 128
aarch32_lowerEL_IRQ:
  B        .                    //        IRQ

  .balign 128
aarch32_lowerEL_FIQ:
  B        .				    //        FIQ

  .balign 128
aarch32_lowerEL_SError:
  B        .                    //        SError

//*****************************************************************************

//****************************************************************************
// synqVectorHandler
// Description: Saves the stack before jumping off to process the
// actual exception. On return, restores the stack
//****************************************************************************
syncVectorHandler:
  //save the stack
  STP      x29, x30, [sp, #-16]!
  STP      x18, x19, [sp, #-16]!
  STP      x16, x17, [sp, #-16]!
  STP      x14, x15, [sp, #-16]!
  STP      x12, x13, [sp, #-16]!
  STP      x10, x11, [sp, #-16]!
  STP      x8, x9, [sp, #-16]!
  STP      x6, x7, [sp, #-16]!
  STP      x4, x5, [sp, #-16]!
  STP      x2, x3, [sp, #-16]!
  STP      x0, x1, [sp, #-16]!

  // go to the exceptionHandlerFuncsEL2N functions to process the exception
  BL       syncHandlerEL2N

  //restore the stack
  LDP      x0, x1, [sp], #16
  LDP      x2, x3, [sp], #16
  LDP      x4, x5, [sp], #16
  LDP      x6, x7, [sp], #16
  LDP      x8, x9, [sp], #16
  LDP      x10, x11, [sp], #16
  LDP      x12, x13, [sp], #16
  LDP      x14, x15, [sp], #16
  LDP      x16, x17, [sp], #16
  LDP      x18, x19, [sp], #16
  LDP      x29, x30, [sp], #16

  ERET
//*****************************************************************************
