/*
 ============================================================================
 Name        : el3mmusetup.s
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : Basic translation table for EL3 to include memory and peripheral access
 Limitations : When running on real hardware need to change this code to prefill
 			   table with faults
 ============================================================================
 */

//*****************************************
// SECTION
//*****************************************
   // This section should automatically go into secure memory region
  .section  .SECUREel3mmusection_ass_el3,"ax"
  .align 3

//*****************************************
// DEFINES
//*****************************************
// There are three types of table entry: (1) fault, (2) block, (3) next table
// Define lower and upper table entry attributes to use
// Assuming MAIR register entries are:
// Attr0 = 1110 1110 = 0xEE = Normal, Inner/Outer write back non transient as per Morello Default DRAM0
// Attr1 = 0000 0000 = 0x00 = Device-nGnRnE
// Lower Block entries
.equ LOWBLK_FAULT,           0x0
.equ LOWBLK_NORMAL_NON_TRANS, 0x00000000000000401    // Index = 0 (Attr0), AF=1
.equ LOWBLK_DEVICE_nGnRnE,   0x00000000000000405    // Index = 1 (Attr1), AF=1

// Upper Block entries
// Upper bits set in the Morello default setup, so include here
.equ UPPBLK_HW60,			(1 << 60)	//Hardware Implementation defined bit 60
.equ UPPBLK_HW61,			(1 << 61)   //Hardware Implementation defined bit 61


// function to set up mmu in EL3
.global el3mmu

//********************************************
// FUNCTIONS
//*******************************************
//-------------------------------------------
// el3mmu
// Set up the mmu for EL3
//-------------------------------------------
  .type el3mmu, "function"
el3mmu:

  // Set up the MAIR memory attributes for the block entries
  // Attr0 = 1110 1110 = 0xEE = Normal, Inner/Outer write back non transient as per Morello Default DRAM0
  // Attr1 = 0000 0000 = 0x00 = Device-nGnRnE
  MOV      x0, #0x00000000000000EE
  MSR      MAIR_EL3, x0

  // EL3 translation table for Morello
  // Get address of level 1 table for TTBR0_EL3
  LDR      x1, =TABLE_ADDR_EL3
  // CAP-TEE Morello - peripheral and expansion regions - device memory
  // (0) 0x0000 0000 - 0x3FFF FFFF
  // Lower Block
  LDR      x0, =LOWBLK_DEVICE_nGnRnE
  // address is 0 - 0x0
  STR      x0, [x1]
  // Upper Block - 0x0

  // CAP-TEE Morello - peripheral and expansion regions - device memory
  // (1) 0x4000 0000 - 0x7FFF FFFF
  // Lower Block
  LDR      x0, =LOWBLK_DEVICE_nGnRnE
  ORR      x0, x0, #0x40000000               // OR with start address of region
  STR      x0, [x1, #8]
  // Upper Block is 0 0x0

  //CAP-TEE Morello - lower DRAM0 region
  // (2) 0x8000 0000 - 0xBFFF FFFF
  // Lower Block
  LDR      x0, =LOWBLK_NORMAL_NON_TRANS       // Entry template - normal memory
  ORR      x0, x0, #0x80000000               // 'OR' template with base physical address
  // Upper block
  ORR      x0, x0, #UPPBLK_HW60      		 // 'OR' Hardware Implementation defined bit 60
  ORR      x0, x0, #UPPBLK_HW61      		 // 'OR' Hardware Implementation defined bit 61                                            // AP=0b00, EL1 RW, EL0 No Access
  STR      x0, [x1, #16]

  // CAP-TEE Morello - upper DRAM0 region
  // (3) 0xC000 0000 - 0xFFFF FFFF
  // Lower Block
  LDR      x0, =LOWBLK_NORMAL_NON_TRANS       // Entry template - normal memory
  ORR      x0, x0, #0xC0000000               // 'OR' template with base physical address
  // Upper block
  ORR      x0, x0, #UPPBLK_HW60      		 // 'OR' Hardware Implementation defined bit 60
  ORR      x0, x0, #UPPBLK_HW61      		 // 'OR' Hardware Implementation defined bit 61
  STR      x0, [x1, #24]
  // data synchronisation - make sure all data written
  DSB      SY

  // Set up TCR_EL3 - Translation Control Register
  // morello default setup for mmu EL3
  // The TCR_EL3 is the control register for stage 1 of the translation.
  // The default for Morello is 0x9F820F19, this is replicated here.
  MOV      x0, #0x1					//T0SZ[5:0] - size offset of memory region
  ORR      x0, x0, #(0x1 << 3)		//Limits virtual addr to 39 bits
  ORR      x0, x0, #(0x1 << 4)
  									//Res[7:6]
  ORR      x0, x0, #(0x1 << 8)		//IRGN0[9:8] 0b11 Normal memory, Inner Write-Back Read-Allocate No Write-Allocate Cacheable.
  ORR      x0, x0, #(0x1 << 9)
  ORR      x0, x0, #(0x1 << 10)		//ORGN0[11:10] 0b11 Normal memory, Outer Write-Back Read-Allocate No Write-Allocate Cacheable.
  ORR      x0, x0, #(0x1 << 11)
  									// SH0[13:12] 0b00, Non-shareable.
  									// TG0[15:14] 0b00  4KB granule for TTBR0
  ORR      x0, x0, #(0x1 << 17)		// PS, bits [18:16] Physical Address Size.0b010	40 bits, 1TB.
  									//Res[19]
  									//TBI[20]
  									//HA[21] Hardware access flag
  									//HD[22] Hardware Management
  ORR      x0, x0, #(0x1 << 23)		//Res1[23] EPD1=0b1      Disable table walks from TTBR1
  ORR      x0, x0, #(0x1 << 24)     //HPD[24]Hierarchical permissions are disabled, bits 25 to 28 are not ignored (i.e bit 59 to 62 of block can be used by hardware).
  ORR      x0, x0, #(0x1 << 25)     //HWU59[25] Bit[59] of each stage 1 translation table Block/Page entry can be used by hardware
  ORR      x0, x0, #(0x1 << 26)     //HWU60[26] Bit[60] of each stage 1 translation table Block/Page entry can be used by hardware
  ORR      x0, x0, #(0x1 << 27)     //HWU61[27] Bit[61] of each stage 1 translation table Block/Page entry can be used by hardware
  ORR      x0, x0, #(0x1 << 28) 	//HWU62[28] Bit[62] of each stage 1 translation table Block/Page entry can be used by hardware
  									//TBID[29] 0b0	TCR_EL3.TBI applies to Instruction and Data accesses.
  									//TCMA[30] 0b0 This control has no effect on the generation of Unchecked accesses.
  ORR      x0, x0, #(0x1 << 31)		//Res[31]
  									//DS[32]
  									//Res[63:33]

  MSR      TCR_EL3, x0
  // Ensure changes to system register are visible
  ISB


  // Invalidate TLBs
  // https://developer.arm.com/documentation/101811/0101/Translation-Lookaside-Buffer-maintenance
  // https://developer.arm.com/documentation/den0024/a/The-Memory-Management-Unit/The-Translation-Lookaside-Buffer
  // To change a translation table entry from a previous setup you need to invalidate
  // the Translation Lookaside Buffers (TLBs) otherwise the cache might reuse recently
  // used translations.
  // we change the table from the default set up
  // This needs to happen before the new base address of the translation table is set
  // otherwise causes a termination
  TLBI     ALLE3 //Invalidate all stage 1 translations used at EL3
  DSB      SY
  ISB

  // Set base address of level 1 table for TTBR0_EL3
  LDR      x0, =TABLE_ADDR_EL3
  MSR      TTBR0_EL3, x0

  // Enable MMU - set the System Control Register
  MOV      x0, #(1 << 0)                     // bit[0]  M=1    Enable MMU
  ORR      x0, x0, #(1 << 2)                 // bit[2]  C=1    Enable data and caches
  ORR      x0, x0, #(1 << 3)                 // bit[3]  SA 	   Alignment check enabled
  ORR      x0, x0, #(1 << 12)                // bit[12] I=1    Enable instruction fetches for caches
                                             // bit[19] WXN=0
                                             // bit[21] IESB=0
                                             // bit[25] EE=0   little endian
  MSR      SCTLR_EL3, x0
  ISB

  RET

  //*****************************************
  // SECTION
  //*****************************************
  // ------------------------------------------------------------
  // Translation table for EL3
  // This is where the EL3 table is stored in memory
  // ------------------------------------------------------------
  // This section should automatically go into secure memory region
  .section  .SECUREttel3section_ass_el3,"ax"
  .align 12

  .global TABLE_ADDR_EL3
TABLE_ADDR_EL3:
  .fill 4096 , 1 , 0

