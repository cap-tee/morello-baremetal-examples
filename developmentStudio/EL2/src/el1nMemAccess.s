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
  .section  .NONSECUREel1ntestsection_ass_el1,"ax"
  .align 3

//*****************************************
// DEFINES
//*****************************************


// function to change MAIR reg
.global el1MAIRmod
// function to modify the translation table in memory using register
.global el1TTmod
// function to modify the translation table in memory by knowing the address
.global el1TTMemMod
// function to modify the EL1N vector table in memory using register
.global el1VTmod
// function to modify the EL1N vector table in memory by knowing the address
.global el1VTMemMod
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
// el1TTMemMod
// function to modify the translation table in memory
// by having a guess / knowing where the tables are stored
//-------------------------------------------
  .type el1TTMemMod, "function"
el1TTMemMod:
  //first take an accurate guess / or know address of where
  //tables stored in memory
  LDR      x1, =0xA00f0000 //read only address set by mmu
  // write over contents at that address
  LDR      x0, =0x00000000000000444
  // OR with start address of region
  ORR      x0, x0, #0x100000000
  // put in the table
  STR      x0, [x1]
  RET

//-------------------------------------------
// el1TTMemMod
// function to modify the vector table in memory
// by having a guess / knowing where the tables are stored
//-------------------------------------------
  .type el1VTMemMod, "function"
el1VTMemMod:
  //first take an accurate guess / or know address of where
  //tables stored in memory
  LDR      x1, =0xA00f2000 //read only address set by mmu
  // write over contents at that address
  LDR      x0, =0x00000000000044444
  // put in the table
  STR      x0, [x1]
  RET


//-------------------------------------------
// el1TTmod
// function to modify the translation table in memory
// using the TTBR0 register
//-------------------------------------------
  .type el1TTmod, "function"
el1TTmod:
  //first, get memory address of translation table
  MRS      x0, TTBR0_EL1
  RET

//-------------------------------------------
// el1VTmod
// function to modify the translation table in memory
// using the VBAR_EL1 register
//-------------------------------------------
  .type el1VTmod, "function"
el1VTmod:
  //first, get memory address of vector table
  MRS      x0, VBAR_EL1
  //change where point to
  LDR 	   x0, =0x80000000
  //resave
  MSR      VBAR_EL1, x0
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
