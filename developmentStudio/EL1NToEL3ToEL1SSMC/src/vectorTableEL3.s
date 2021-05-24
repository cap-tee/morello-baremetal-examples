/*
 ============================================================================
 Name        : vectorTableEL3.s
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : The vector table here is for EL3
  			   All exceptions routed to EL3 come here. The exception type, and where it
 			   has come from determines which entry in the table it is routed to.
 			   This follows the standard vector table format for the AArch64 setup
 			   see https://developer.arm.com/documentation/100933/0100/AArch64-exception-vector-table
 			   Each exception level (EL3, EL1N, EL1S) has its own vector table.
			   The base address for the EL3 table is given by VBAR_EL3, which in this project is defined in setRegTables.s
			   Each entry has a defined offset from this base address of 128 bytes.
			   Additionally, each table has a defined number of entries in a defined format of 4 sets of 4 entries to
			   cover the different types, and where the exception is being taken from.
			   The table contains branch instructions to an exception handler function, but it is possible to write
               up to 32 instructions here for each entry.
               In the specific table defined here, entries not used are branch to self (B .)
               Two labels are defined here: FIQVectorHandler - timer interrupt, which branches off to the fiqHandler function
               defined in exceptionHandlerFuncsEL3, and SYNCVectorHandler - SMC exception
               which branches off to the syncHandlerEL3 function defined in exceptionHandlerFuncsEL3
 ============================================================================
 */

 //****************************************************************************
 // SECTION AND DEFINES
 //****************************************************************************
 // Define a secure memory region for the vector table
 // This section goes into the secure memory region
  .section  .SECUREvectortableel3_ass,"ax"
  .align 12
 
 // Vector Table function for EL3 - used outside this file
  .global vectorsEL3
 // Exception handler functions for EL3, located in exceptionHandlerFuncsEL3.c
  .global fiqHandlerEL3
  .global syncHandlerEL3

 //****************************************************************************
 // FUNCTIONS
 //****************************************************************************

 // ------------------------------------------------------------
 // vectorsEL3
 // Description: Vector table entries
 // ------------------------------------------------------------
  .type vectorsEL3, "function"
vectorsEL3:

// ------------------------------------------------------------
// Current EL with SP0
// This block of four entries are associated with exceptions
// that have been triggered from EL3 whilst in program state 0
// ------------------------------------------------------------

  .balign 128
sp0_currentEL_SYNC:
  B        .                    //        Synchronous

  .balign 128
sp0_currentEL_IRQ:
  B        .                    //        IRQ

  .balign 128
sp0_currentEL_FIQ:
  B        .					 //        FIQ

  .balign 128
sp0_currentEL_SError:
  B        .                    //        SError

// ------------------------------------------------------------
// Current EL with SPx
// This block of four entries are associated with exceptions
// that have been triggered from EL3 whilst in program state 1
// The current example code uses program state 1.
// The secure timer interrupt is set up to generate an fiq exception.
// ------------------------------------------------------------

  .balign 128
spx_currentEL_SYNC:
  B        .                    //        Synchronous

  .balign 128
spx_currentEL_IRQ:
  B        .                    //        IRQ

  .balign 128
spx_currentEL_FIQ:
  B        FIQVectorHandler     //        FIQ handler for timer

  .balign 128
spx_currentEL_SError:
  B        .                    //        SError

// ------------------------------------------------------------
// Lower EL using AArch64
// This block of four entries are associated with exceptions
// that have been triggered from a lower EL, which would be EL1N
// or EL1S using AArch64.
// ------------------------------------------------------------

  .balign 128
aarch64_lowerEL_SYNC:
  B        SYNCVectorHandler    //        Synchronous handler for SMC from EL1N or EL1S

  .balign 128
aarch64_lowerEL_IRQ:
  B        .                    //        IRQ

  .balign 128
aarch64_lowerEL_FIQ:
  B        .					 //        FIQ

  .balign 128
aarch64_lowerEL_SError:
  B        .                    //        SError

// ------------------------------------------------------------
// Lower EL using AArch32
// This block of four entries are associated with exceptions
// that have been triggered from a lower EL, which would be EL1N
// or EL1S using AArch32.
// ------------------------------------------------------------

  .balign 128
aarch32_lowerEL_SYNC:
  B        .					//        Synchronous

  .balign 128
aarch32_lowerEL_IRQ:
  B        .                    //        IRQ

  .balign 128
aarch32_lowerEL_FIQ:
  B        .					 //        FIQ

  .balign 128
aarch32_lowerEL_SError:
  B        .                    //        SError

//*****************************************************************************

// ------------------------------------------------------------
// fiqVectorHandler
// Description: function to capture the timer interrupt.
// Saves the stack before jumping off to process the
// actual exception. On return, restores the stack
// ------------------------------------------------------------
FIQVectorHandler:
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

  // go to the exceptionHandlerFuncsEL3 functions
  BL       fiqHandlerEL3

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

// ------------------------------------------------------------
// SYNCVectorHandler
// Description: function to capture the SMC call from EL1
// Saves the stack before jumping off to process the
// actual exception. On return, restores the stack
// ------------------------------------------------------------
SYNCVectorHandler:
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

  // go to the exceptionHandlerFuncsEL3 functions
  BL       syncHandlerEL3

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
