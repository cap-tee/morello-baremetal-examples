/*
 ============================================================================
 Name        : vectorTableEL1S.s
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : The vector table here is for EL1S
 			   This follows the standard vector table format for the AArch64 setup
 			   see https://developer.arm.com/documentation/100933/0100/AArch64-exception-vector-table
 			   Each exception level (EL3, EL2, EL1) has its own vector table.
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
 /*This section needs to go into secure memory region */
  .section  .SECUREvectortableel1S_ass,"ax"
  .align 12
 
  .global vectorsEL1S
  .type vectorsEL1S, "function"
vectorsEL1S:

// exception handler function, which is defined in the exceptionHandlerFuncsEL1S.c
  .global fiqHandlerEL1S

// ------------------------------------------------------------
// Current EL with SP0 - program state 0  - 0X04 EL1
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
// Current EL with SPx - program state 1  - 0x05 EL1
// ------------------------------------------------------------

  .balign 128
spx_currentEL_SYNC:
  B        .                    //        Synchronous

  .balign 128
spx_currentEL_IRQ:
  B        .                    //        IRQ

  .balign 128
spx_currentEL_FIQ:
  B        fiqFirstLevelHandler //        FIQ

  .balign 128
spx_currentEL_SError:
  B        .                    //        SError

// ------------------------------------------------------------
// Lower EL using AArch64
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


// ------------------------------------------------------------

fiqFirstLevelHandler:

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

  BL       fiqHandlerEL1S

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

