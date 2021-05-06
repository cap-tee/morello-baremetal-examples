/*
 ============================================================================
 Name        : el1nmmusetup.s
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : Basic translation table for EL1N to include upper SDRAM0 only
               non secure memory and peripheral access
 ============================================================================
 */
 /*This section needs to go into normal memory region - see linker script*/
  .section  BOOT_EL1NMMU,"ax"
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
.global el1nmmu
  .type el1nmmu, "function"
el1nmmu:
  //***************************
  //
  // MMU setup
  //
  //***************************

  // Set the Base address
  // ---------------------
  LDR      x0, =tt_l1_base_el1n                  // Get address of level 1 for TTBR0_EL1
  MSR      TTBR0_EL1, x0

  // Set up memory attributes
  // -------------------------
  // Attr0 = 1110 1110 = 0xEE = Normal, Inner/Outer write back non transient as per Morello Default DRAM0
  // Attr1 = 0000 0000 = 0x00 = Device-nGnRnE
  MOV      x0, #0x00000000000000EE
  MSR      MAIR_EL1, x0

  // Set up TCR_EL1
  // ---------------
  //morello default setup for mmu EL1
  //The TCR_EL1 is the control register for stage 1 of the translation.
  MOV      x0, #0x1					//T0SZ[5:0] - size offset of memory region
  ORR      x0, x0, #(0x1 << 3)		//Limits VA space to 39 bits, translation starts @ l1
  ORR      x0, x0, #(0x1 << 4)
  									//Res[6]
  									//TTBR0--
  									//EPD0[7] TTBR0 turned on
  ORR      x0, x0, #(0x1 << 8)		//IRGN0[9:8] 0b11 Normal memory, Inner Write-Back Read-Allocate No Write-Allocate Cacheable.
  ORR      x0, x0, #(0x1 << 9)
  ORR      x0, x0, #(0x1 << 10)		//ORGN0[11:10] 0b11 Normal memory, Outer Write-Back Read-Allocate No Write-Allocate Cacheable.
  ORR      x0, x0, #(0x1 << 11)
  //-----------------------------------
  									// SH0[13:12] 0b00, Non-shareable.
  									// TG0[15:14] 0b00  4KB granule for TTBR0
  									//TTBR1--
//---ADDED TO REMOVE DEBUGGER MMU WARNINGS---
   ORR      x0, x0, #(0x1 << 16) 			//T1SZ[21:16] #0X19
   ORR      x0, x0, #(0x1 << 19)
   ORR      x0, x0, #(0x1 << 20)
//---------------------------------------
  									//A1[22]
  ORR      x0, x0, #(0x1 << 23)		//EPD1[23] disable TTBR1
  									//IRGN1[25:24]
  									//ORGN1[27:26]
  									//SH1[29:28]
//---ADDED TO REMOVE DEBUGGER MMU WARNINGS---
  ORR      x0, x0, #(0x1 << 31)     //TGI[31:30]  //TTBR1 - granule size 4kB as gives warning in MMU debugger map --- BBB
//------------------------------------------//--
   ORR      x0, x0, #(0x1 << 33)    //IPS[34:32] Physical Address Size.0b010	40 bits, 1TB.
  									//RES[35]
  									//AS[36]
  									//TB10[37]
  									//TB11[38]
  									//HA[39]
  									//HD[40]
 ORR      x0, x0, #(0x1 << 41)		//HPD0[41]Hierarchical permissions are disabled, bits 25 to 28 are not ignored (i.e bit 59 to 62 of block can be used by hardware).
  									//HPD1[42]
  ORR      x0, x0, #(0x1 << 43) 	//HWU059[43]Bit[59] of each stage 1 translation table Block/Page entry can be used by hardware
  ORR      x0, x0, #(0x1 << 44)		//HWU060[44]Bit[60] of each stage 1 translation table Block/Page entry can be used by hardware
  ORR      x0, x0, #(0x1 << 45)		//HWU061[45]Bit[61] of each stage 1 translation table Block/Page entry can be used by hardware
  ORR      x0, x0, #(0x1 << 46)		//HWU062[46]Bit[62] of each stage 1 translation table Block/Page entry can be used by hardware
  									//HWU159[47]
  									//HWU160[48]
  									//HWU161[49]
  									//HWU162[50]
  									//TB1D0[51]
  									//TB1D1[52]
  									//NFD0[53]
  //---ADDED TO REMOVE DEBUGGER MMU WARNINGS---
   ORR      x0, x0, #(0x1 << 54)    //NFD1[54]    //Disable table walks from TTBR1
   //------------------------
  									//EOPD0[55]
  									//EOPD1[56]
  									//TCMA[57]
  									//TCMA1[58]
  									//DS[59]
  									//RES0[63:60]

  MSR      TCR_EL1, x0
  ISB


  // Invalidate TLBs
  // ----------------
  TLBI     VMALLE1
  DSB      SY
  ISB


  // Generate Translation Table
  // ---------------------------
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


  //
  // EL1N basic translation table
  //
  LDR      x1, =tt_l1_base_el1n                   // Address of L1 table
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

  //This block is in secure memory, so don't include here in the MMU translation
  ////CAP-TEE Morello - lower DRAM0 region
  //// [2]: 0x8000,0000 - 0xBFFF,FFFF
  ////Lower Block
  //LDR      x0, =TT_S1_NORMAL_NON_TRANS       // Entry template
  LDR      x0, =TT_S1_FAULT       // Entry template  - Ignore this block
  ORR      x0, x0, #0x80000000               // 'OR' template with base physical address
  ////Upper block
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
  //STR      x0, [x1, #16]	//change offset	//STR      x0, [x1, #24]
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
  MSR      SCTLR_EL1, x0
  ISB

  //
  // MMU is now enabled
  //
  RET


 // ------------------------------------------------------------
// Translation tables
// ------------------------------------------------------------
//put in non secure memory - see linker script
  .section  TT_EL1N,"ax"
  .align 12

  .global tt_l1_base_el1n
tt_l1_base_el1n:
  .fill 4096 , 1 , 0

// ------------------------------------------------------------
// End of file
// ------------------------------------------------------------

