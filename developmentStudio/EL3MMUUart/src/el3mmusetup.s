/*
 ============================================================================
 Name        : el3mmusetup.s
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : Basic translation table for EL3 to include memory and peripheral access
 ============================================================================
 */
  .section  .SECUREel3mmusection_ass,"ax"
  .align 3

// ------------------------------------------------------------
// Translation Table block entries templates
// Assuming MAIR register entries
// Attr0 = 1110 1110 = 0xEE = Normal, Inner/Outer write back non transient as per Morello Default DRAM0
// Attr1 = 0000 0000 = 0x00 = Device-nGnRnE
// Lower Block entries
.equ TT_S1_FAULT,           0x0
.equ TT_S1_NORMAL_NON_TRANS, 0x00000000000000401    // Index = 0 (Attr0), AF=1
.equ TT_S1_DEVICE_nGnRnE,   0x00000000000000405    // Index = 1 (Attr1), AF=1, (needs PXN=1, UXN=1 in Upper Block)

//Upper Block entries
//Upper bits set in the Morello default setup, so include here
.equ TT_S1_HW60,			(1 << 60)	//Hardware Implementation defined bit 60
.equ TT_S1_HW61,			(1 << 61)   //Hardware Implementation defined bit 61


//define function
.global el3mmu
  .type el3mmu, "function"
el3mmu:
  //***************************
  //
  // MMU setup
  //
  //***************************

  // Set the Base address
  // ---------------------
  LDR      x0, =tt_l1_base_el3                  // Get address of level 1 for TTBR0_EL3
  MSR      TTBR0_EL3, x0

  // Set up memory attributes
  // -------------------------
  // Attr0 = 1110 1110 = 0xEE = Normal, Inner/Outer write back non transient as per Morello Default DRAM0
  // Attr1 = 0000 0000 = 0x00 = Device-nGnRnE
  MOV      x0, #0x00000000000000EE
  MSR      MAIR_EL3, x0

  // Set up TCR_EL3
  // ---------------
  //morello default setup for mmu EL3
  //The TCR_EL3 is the control register for stage 1 of the translation.
  //The default for Morello is 0x9F820F19, this is replicated here.
  MOV      x0, #0x1					//T0SZ[5:0] - size offset of memory region
  ORR      x0, x0, #(0x1 << 3)		//Limits VA space to 39 bits, translation starts @ l1
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
  // Ensure changes to system register are visible before MMU enabled
  ISB

  // Invalidate TLBs
  // ----------------
  TLBI     VMALLE1
  DSB      SY
  ISB

  //*******************************************************************************
  // Generate Translation Table
  // ---------------------------
  // FOR HARDWARE - AS NOTED BY ARM
  // First fill table with faults
  // NOTE: The way the space for the tables is reserved pre-fills it with zeros
  // When loading the image into a simulation this saves time.  On real hardware
  // you would want this zeroing loop.
  //  LDR      x1, =tt_l1_base                   // Address of L1 table
  //  MOV      w2, #512                          // Number of entries
  //1:
  //  STP      xzr, xzr, [x1], #16               // 0x0 (Fault) into table entries
  //  SUB      w2, w2, #2                        // Decrement count by 2, as we are writing two entries at once
  //  CBNZ     w2, 1b
  //-----------------------------

  //
  // EL3 basic translation table
  //
  LDR      x1, =tt_l1_base_el3                   // Address of L1 table
  //CAP-TEE Morello - peripheral and expansion regions
  // [0]: 0x0000,0000 - 0x3FFF,FFFF
  //Lower Block
  LDR      x0, =TT_S1_DEVICE_nGnRnE          // Entry template
                                             // Don't need to OR in address, as it is 0
  STR      x0, [x1]

  //CAP-TEE Morello - peripheral and expansion regions
  // [1]: 0x4000,0000 - 0x7FFF,FFFF
  //Lower Block
  LDR      x0, =TT_S1_DEVICE_nGnRnE          // Entry template
  ORR      x0, x0, #0x40000000               // 'OR' template with base physical address
  STR      x0, [x1, #8]

  //CAP-TEE Morello - lower DRAM0 region
  // [2]: 0x8000,0000 - 0xBFFF,FFFF
  //Lower Block
  LDR      x0, =TT_S1_NORMAL_NON_TRANS       // Entry template
  ORR      x0, x0, #0x80000000               // 'OR' template with base physical address
  //Upper block
  ORR      x0, x0, #TT_S1_HW60      		 // 'OR' Hardware Implementation defined bit 60
  ORR      x0, x0, #TT_S1_HW61      		 // 'OR' Hardware Implementation defined bit 61                                            // AP=0b00, EL1 RW, EL0 No Access
  STR      x0, [x1, #16]

  //CAP-TEE Morello - upper DRAM0 region
  // [3]: 0xC000,0000 - 0xFFFF,FFFF
  //Lower Block
  LDR      x0, =TT_S1_NORMAL_NON_TRANS       // Entry template
  ORR      x0, x0, #0xC0000000               // 'OR' template with base physical address
  //Upper block
  ORR      x0, x0, #TT_S1_HW60      		 // 'OR' Hardware Implementation defined bit 60
  ORR      x0, x0, #TT_S1_HW61      		 // 'OR' Hardware Implementation defined bit 61
  STR      x0, [x1, #24]

  DSB      SY

  // Enable MMU
  // -----------
  MOV      x0, #(1 << 0)                     // M=1           Enable the stage 1 MMU
  ORR      x0, x0, #(1 << 2)                 // C=1           Enable data and unified caches
  ORR      x0, x0, #(1 << 3)                 //SP Alignment check enabled
  ORR      x0, x0, #(1 << 12)                // I=1           Enable instruction fetches to allocate into unified caches
                                             // A=0           Strict alignment checking disabled
                                             // SA=0          Stack alignment checking disabled
                                             // WXN=0         Write permission does not imply XN
                                             // EE=0          EL3 data accesses are little endian
  MSR      SCTLR_EL3, x0
  ISB

  //
  // MMU is now enabled
  //
  RET


  // ------------------------------------------------------------
  // Translation tables
  // ------------------------------------------------------------

  .section  .SECUREttel3section_ass,"ax"
  .align 12

  .global tt_l1_base_el3
tt_l1_base_el3:
  .fill 4096 , 1 , 0

// ------------------------------------------------------------
// End of file
// ------------------------------------------------------------

