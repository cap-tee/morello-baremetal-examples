/*
 ============================================================================
 Name        : el1nMemAccess.s
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : Functions to try to change the mmu memory settings at EL1
 Limitations :
 ============================================================================
 */

//*****************************************
// SECTION
//*****************************************
// This section needs to go into normal memory region - see linker script
  .section  .NONSECUREel1nmmusection_ass,"ax"
  .align 3

//*****************************************
// DEFINES
//*****************************************


// function to change MAIR reg
.global el1MAIRmod
// function to modify the translation table in memory
.global el1TTmod
// function to change TCR reg
.global el1TCRmod
// function to change SCTLR reg
.global el1SCTLRmod
// function to invalidate TLB
.global el1TLBmod

//********************************************
// FUNCTIONS
//*******************************************

//-------------------------------------------
// el1MAIRmod
// function to change MAIR reg
//-------------------------------------------
  .type el1MAIRmod, "function"
el1MAIRmod:
  // Set up the MAIR memory attributes for the block entries
  // Attr0 = 1110 1110 = 0xEE = Normal, Inner/Outer write back non transient as per Morello Default DRAM0
  // Attr1 = 0000 0000 = 0x00 = Device-nGnRnE
  MOV      x0, #0x00000000000000EE
  MSR      MAIR_EL1, x0

RET

//-------------------------------------------
// el1MAIRmod
// function to modify the translation table in memory
//-------------------------------------------
  .type el1TTmod, "function"
el1TTmod:
  //first, get memory address of translation table
  MRS      x0, TTBR0_EL1

RET

//-------------------------------------------
// el1MAIRmod
// function to change TCR reg
//-------------------------------------------
  .type el1TCRmod, "function"
el1TCRmod:
  MRS 	   x0, TCR_EL1
  ORR      x0, x0, #(0x1 << 7)		//EPD0[7] TTBR0 turn off
  MSR      TCR_EL1, x0
  ISB

RET

//-------------------------------------------
// el1SCTLRmod
// function to change SCTLR reg
//-------------------------------------------
  .type el1SCTLRmod, "function"
el1SCTLRmod:
  MRS 	   x0, SCTLR_EL1
  MOV      x0, #(1 << 0) //try to turn on/off mmu 1 is on, 0 is off
  MSR      SCTLR_EL1, x0
  ISB

RET

//-------------------------------------------
// el1SCTLRmod
// function to invalidate TLB
//-------------------------------------------
  .type el1TLBmod, "function"
el1TLBmod:
  TLBI     VMALLE1 //TLB invalidate by VMID, All at stage 1, EL1.
  DSB      SY
  ISB

RET
