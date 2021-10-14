/*
 ============================================================================
 Name        : vectorTableEL1S.s
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : The vector table here is for EL1S
  			   All exceptions routed to EL1S come here. The exception type, and where it
 			   has come from determines which entry in the table it is routed to.
 			   This follows the standard vector table format for the AArch64 setup
 			   see https://developer.arm.com/documentation/100933/0100/AArch64-exception-vector-table
 			   Each exception level (EL3, EL1N, EL1S) has its own vector table.
			   The base address for the EL1 table is given by VBAR_EL1, which in this project is defined in regForEL1S.s
			   Each entry has a defined offset from this base address of 128 bytes.
			   Additionally, each table has a defined number of entries in a defined format of 4 sets of 4 entries to
			   cover the different types, and where the exception is being taken from.
			   The table contains branch instructions to an exception handler function, but it is possible to write
               up to 32 instructions here for each entry.
               In the specific table defined here, entries not used are branch to self (B .)
               Only one label is defined here fiqFirstLevelHandler, which branches off to the fiqHandlerEL1S function
               defined in exceptionHandlerFuncsEL1S
 ============================================================================
 */

 //****************************************************************************
 // SECTION AND DEFINES
 //****************************************************************************
 // Define a secure memory region for the vector table
 // This section goes into the secure memory region
  .section  .SECUREvectortableel1S_ass,"ax"
  .align 12
 
  // Vector Table function for EL1S - used outside this file
  .global vectorsEL1S
  // Exception handler function, which is defined in the exceptionHandlerFuncsEL1S.c
  .global fiqHandlerEL1S

 //****************************************************************************
 // FUNCTIONS
 //****************************************************************************

 // ------------------------------------------------------------
 // vectorsEL1S
 // Description: Vector table entries
 // ------------------------------------------------------------
  .type vectorsEL1S, "function"
vectorsEL1S:


// ------------------------------------------------------------
// Current EL with SP0 - program state 0  - 0X04 EL1S
// This block of four entries are associated with exceptions
// that have been triggered from EL1S whilst in program state 0
// ------------------------------------------------------------

  .balign 128
sp0_currentEL_SYNC:
  B        .                    //        Synchronous

  .balign 128
sp0_currentEL_IRQ:
  B        .                    //        IRQ

  .balign 128
sp0_currentEL_FIQ:
  B        .				    //        FIQ

  .balign 128
sp0_currentEL_SError:
  B        .                    //        SError

// ------------------------------------------------------------
// Current EL with SPx - program state 1  - 0x05 EL1S
// This block of four entries are associated with exceptions
// that have been triggered from EL1S whilst in program state 1
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
  B        FIQVectorHandler 	//        FIQ Handler for timer

  .balign 128
spx_currentEL_SError:
  B        .                    //        SError

// ------------------------------------------------------------
// Lower EL using AArch64
// This block of four entries are associated with exceptions
// that have been triggered from a lower EL, which would be EL0
// using AArch64.
// The current example code does not use EL0.
// ------------------------------------------------------------

  .balign 128
aarch64_lowerEL_SYNC:
  B        .                    //		Synchronous

  .balign 128
aarch64_lowerEL_IRQ:
  B        .                    //        IRQ

  .balign 128
aarch64_lowerEL_FIQ:
  B        .				    //        FIQ

  .balign 128
aarch64_lowerEL_SError:
  B        .                    //        SError

// ------------------------------------------------------------
// Lower EL using AArch32
// This block of four entries are associated with exceptions
// that have been triggered from a lower EL, which would be EL0
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
// fiqVectorHandler
// Description: Saves the stack before jumping off to process the
// actual exception. On return, restores the stack
//****************************************************************************
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

  // go to the exceptionHandlerFuncsEL1S functions to process the exception
  BL       fiqHandlerEL1S

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
