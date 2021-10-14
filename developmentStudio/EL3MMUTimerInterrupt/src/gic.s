/*
 ============================================================================
 Name        : gic.s
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : Sets up the general interrupt controller (GIC-600) for timer
 			   in secure group 0
 			   The GIC needs to be set up at EL3 because the ICC_SRE_EL3 register
			   is only accessible at EL3 which sets up interrupt controller access
			   at lower levels.

			   The Morello memory mapped base addresses are
			   GICDbase,		            0x30000000
			   RDbase,	              		0x300C0000

			   uses Distributor and redistributor registers

Further work: Take out the code that is common to EL3/EL1S/EL1N and have in seperate file
              so only have extra code for each EL that is different.

 ============================================================================
 */

//*****************************************
// SECTION
//*****************************************

// should automatically go into secure section of memory
.section  .SECUREGICS_ass,"ax"
.align 3

//*****************************************
// DEFINES
//*****************************************

  // sets up gic
  .global setupGIC600
  // gets the interrupt id from interrupt ack reg - group 0
  .global getIntidAckReg0
  // sets the interrupt id in the end of interrupt reg - group 0
  .global setIntidEndReg0

// These are the Distributor registers--------------------------------
// See table 4-2 of GIC-600 doc
.equ GICDbase,		            0x30000000

// GICD_CTLR register - distributor control Register
.equ GICD_CTLRoffset,		  0x0	 //offset address of register
.equ GICD_CTLR.EnableGrp0,	  1      //enable group 0 interrupts
.equ GICD_CTLR.EnableGrp1NS,  (1<<1) //enable non secure group 1 interrupts
.equ GICD_CTLR.EnableGrp1S,	  (1<<2) //enable secure group 1 interrupts
                                     // 3 reserved
.equ GICD_CTLR.ARE_S,		  (1<<4) //Enable affinity routing secure state
.equ GICD_CTLR.ARE_NS,		  (1<<5) //Enable affinity routing non secure state
.equ GICD_CTLR.DS,		      (1<<6) //disable security - not used

// GICD_IGROUPRn - interrupt group registers
// DIDN'T USE THESE ONES - USED GICR ONES BELOW
.equ GICD_IGROUPRoffset,	  0x080
.equ GICD_ISENABLERoffset,    0x100
.equ GICD_ICENABLERoffset,    0x180
.equ GICD_ISPENDRoffset,      0x200
.equ GICD_ICPENDRoffset,      0x280
.equ GICD_IPRIORITYRoffset,   0x400
.equ GICD_ICFGRoffset,        0xC00
.equ GICD_IGRPMODRoffset,     0xD00
.equ GICD_NSACRoffset,        0xE00
.equ GICD_SGIR,               0xF00
// DIDN'T USE THESE ONES
.equ GICD_IROUTERoffset,      0x6000
.equ GICD_IROUTER.RM,        (1<<31)

// These are the Redistributor registers LPIs------------------------------
// See table 4-21 of GIC-600 doc
.equ RDbase,	                 0x300C0000 // Morello base address
// NOT USED
.equ GICR_CTLRoffset,		     0x0

// Power management control register
.equ GICR_WAKERoffset,		     0x14
.equ GICR_WAKER.ProcessorSleep,  (1<<1) //0 never asserts wake_request, 1 must assert wake_request
.equ GICR_WAKER.ChildrenAsleep,  (1<<2) //bus between cpu and redistributor is quiescent

// Morello uses GIC-600, so need power management of the Redistributor
// need to power it on
.equ GICR_PWRRoffset,				0x24
.equ GICR_PWRR.RDPD,				(1<<0) // 0bit set to 1 - Power down
.equ GICR_PWRR.RDAG,				(1<<1) // 1bit set to 1 - Apply power down to all redistributers in the group
.equ GICR_PWRR.RDGPD,				(1<<2) // 2bit set to 1 - read only - 0 going to power up, 1 going to power down
.equ GICR_PWRR.RDGPO,				(1<<3) // 3bit set to 1 - read only - 0 powered up, 1 safe to power down

 // These are the Redistributor registers SGIs / PPIs------------------------
 // See table 4-29 of GIC-600 doc
.equ SGIbase,	               (RDbase + 0x10000)
 // GICR_IGROUPRn - interrupt group registers
.equ GICR_IGROUPRoffset,	 0x080 //Interrupt group register
.equ GICR_ISENABLERoffset,   0x100 //Interrupt set-enable register
.equ GICR_ICENABLERoffset,   0x180 //interrupt clear-enable register
.equ GICR_ISPENDRoffset,     0x200 //interrupt set-pending register
.equ GICR_ICPENDRoffset,     0x280 //peripheral clear pending register
.equ GICR_IPRIORITYRoffset,  0x400 //interrupt priority registers
.equ GICR_ICFGRoffset,       0xC00 //interrupt configuration registers
.equ GICR_IGRPMODRoffset,    0xD00 //interrupt group modifier register
.equ GICR_NSACRoffset,       0xE00 //non-secure access control register

// system GIC register bits ---------------
.equ ICC_SRE_ELn.Enable,     (1<<3) //3 bit set to 1
.equ ICC_SRE_ELn.SRE,        (1)

//********************************************
// FUNCTIONS
//*******************************************
//---------------------------------------------
// set up GIC-600 for Morello, and enable timer interrupt
//---------------------------------------------

  .type setupGIC600, "function"
setupGIC600:
  // Set up the Distributor
  // first get the base address for Morello, and then set up
  MOV      x0, #GICDbase
  ADD      x1, x0, #GICD_CTLRoffset
  MOV      x2,     #GICD_CTLR.ARE_NS
  ORR      x2, x2, #GICD_CTLR.ARE_S
  STR      w2, [x1] // w2 - least 32 bits of x2 64 bits

  ORR      x2, x2, #GICD_CTLR.EnableGrp0
  ORR      x2, x2, #GICD_CTLR.EnableGrp1S
  ORR      x2, x2, #GICD_CTLR.EnableGrp1NS
  STR      w2, [x1]
  DSB      SY

  //********************************************************************************
  // GIC-600 which includes power management of the redistributor
  // Power on the Redistributor(method adapted from gic-x00.c in trusted firmware from c code)
  // wait until group not transitioning RDGPD==RDGP0
  // 1. get base address
  // 2. get offset address
  // 3. combine the address to get full adress of GICR_PWRR and store in x1
  // 4. load contents of memory location [x1] into w0 (32 bits of x0)
  // 5. OR the bits GICR_PWRR.RDGPD and GICR_PWRR.RDGPO together to make
  //    a mask and a compare check
  // 6. Mask the bits out to check them
  // 8. find out if bits are same (C or 0) not same (4 or 8)
  // 9. loop if not same (loop until RDGPD==RDGP0)
  MOV      x0, #RDbase
  MOV      x1, #GICR_PWRRoffset
  ADD      x1, x1, x0
l2:
  LDR      w0, [x1]
  //or the bits together to make a mask and a compare check
  MOV      x2, #GICR_PWRR.RDGPD
  MOV      x3, #GICR_PWRR.RDGPO
  ORR      x3, x3, x2
//mask the bits out to check them x2 is value, x3 is mask
  AND      x2, x0, x3
//And value with mask to see if same
  AND      x4, x2, x3
 //IF zero (i.e both bits 0) carry on to l3 else do next check
  CBZ      w4, l3
 //SUB value from mask
  SUB      x4, x3, x2
 //if zero carry on (i.e both bits 1), if not zero (i.e both bits not 1) loop back
  CBNZ      w4, l2
l3:
  //power on redistributor set RDPD=0
  //if RDGPD==RDGP0, can now set RDPD bit to 0
  //get base address,get offset address, & combine to get full adress of GICR_PWRR and store in x1
  MOV      x0, #RDbase
  MOV      x1, #GICR_PWRRoffset
  ADD      x1, x1, x0
  //load contents of register
  LDR      w0, [x1]
  //mask lower bit to zero
  MOV      x2, #0xFFFFFFFE
  //and mask with contents of register to set bottom bit to zero
  AND      x3, x0, x2
  //put value back into contents of register
  STR      w3, [x1]
  DSB      SY

  //wait until the power on state is reflected, read RDPD and check ==0
  //load contents of register
l4:  LDR      w0, [x1]
  //make a mask for RPDP
  MOV      x2, #GICR_PWRR.RDPD
  //AND with mask,
  AND      x3, x2, x0
  //if not zero must still be set, so wait
  CBNZ     w3, l4
  //***********************************************************************************************

  // setup Redistributor
  // Clear ProcessorSleep signals core is awake
  MOV      x0, #RDbase
  MOV      x1, #GICR_WAKERoffset
  ADD      x1, x1, x0
  STR      wzr, [x1]
  DSB      SY
1:// wait for sleep to read 0
  LDR      w0, [x1]
  MOV      x2, #GICR_WAKER.ProcessorSleep
  MOV      X3, #GICR_WAKER.ChildrenAsleep
  ORR      X2, X2, X3
  AND      w0, w0, w2
  CBNZ     w0, 1b

  // set up CPU interface
  // set SRE bits for each EL for access to the interrupt controller registers
  // Setting the enable bit does the following:
  //   a.Secure EL1 accesses to Secure ICC_SRE_EL1 do not trap to EL3.
  //   b.Non-secure EL1 accesses to ICC_SRE_EL1 do not trap to EL3
  //   c.EL2 accesses to Non-secure ICC_SRE_EL1 and ICC_SRE_EL2 do not trap to EL3.
  // This means we can use the interupts at lower exception levels and doesn't go to EL3
  // Setting the SRE bit means the Interrupt controller System register interface for EL3 is enabled.
  MOV      x0, #ICC_SRE_ELn.Enable
  ORR      x0, x0, ICC_SRE_ELn.SRE
  MSR      ICC_SRE_EL3, x0
  ISB

  //need for exception layers.............
 //Setting the SRE bit means the Interrupt controller System register interface for EL1 is enabled.
  MOV      x0, #ICC_SRE_ELn.SRE
  MSR      ICC_SRE_EL1, x0
  ISB
  //MSR      ICC_SRE_EL2, x0

  MOV      w0, #0xFF
  MSR      ICC_PMR_EL1, x0 // Set PMR to lowest priority

  //***Enable the interrupts for different groups ************************
  // Interrupt Controller Interrupt Group 1 Enable register (EL3)
  // Controls whether Group 1 interrupts are enabled or not.
  // bit[1] 0-secure group 1 disabled, 1-secure group 1 enabled
  // bit[0] 0-non secure group 1 disabled, 1-non secure group 1 enabled
  MOV      w0, #3
  MSR      ICC_IGRPEN1_EL3, x0  // enable secure and non secure group 1
  // Interrupt Controller Interrupt Group 1 Enable register (EL1)
  // Controls whether Group 1 interrupts are enabled for the current Security state.
  // bit[0] 0-disabled, 1-enabled
  MOV      w0, #1
  MSR      ICC_IGRPEN1_EL1, x0 //Enables Group 1 interrupts
  // Interrupt Controller Interrupt Group 0 Enable register (EL1)
  // Controls whether Group 0 interrupts are enabled or not.
  // bit[0] 0-disabled, 1-enabled
  MSR      ICC_IGRPEN0_EL1, x0 //Enables Group 0 interrupts
  //*********************************************************************

  // ***Set secure physical timer (ID 29)*****************
  // This system is secure aware (GICD_CTLR.DS==0 )
  MOV      x0, #SGIbase       // Address of Redistributor registers
  // Interrupt group register(same as GICD_IGROUPRn?)
  // each bit represents an interrupt ID
  // if GICD_CTLR.DS==0 (secure aware)
  //     - a 0 in the bit means it is in the Secure group (group 0 secure or group 1 secure determined below),
  //     - a 1 means non secure group 1
  // if GICD_CTLR.DS==1 (not secure aware)
  //     - a 0 means group 0
  //     - a 1 means group 1
  ADD      x1, x0, #GICR_IGROUPRoffset
  STR      wzr, [x1]          // Mark ID 0..31 as Secure

  // Interrupt Group Modifier Registers (same as GICD_IGRPMODR?)
  // each bit represents an interrupt ID

  // If affinity routing is enabled: (otherwise this register is reserved)for the Security state of an interrupt,
  // the bit is concatenated with above register to form a 2-bit field that defines an interrupt group.
  // if GICD_CTLR.DS==0 (secure aware)
  // modbit(0)  groupbit(0) - secure group 0
  // modbit(0)  groupbit(1) - Non secure group 1
  // modbit(1)  groupbit(0) - Secure group 1
  // modbit(1)  groupbit(1) - Reserved treated as non secure group 1

  // If affinity not enabled:
  // if GICD_CTLR.DS==0 (secure aware)
  // modbit(reserved) groupbit(0) - secure group 0
  // modbit(reserved) groupbit(1) - Non secure group 1
  ADD      x1, x0, #GICR_IGRPMODRoffset
  STR      wzr, [x1]          // Mark ID 0..31 as Secure Group 0

  // Interrupt Set-Enable Registers
  // added for secure interupt source TIMER IS 29
  // Enables forwarding of the corresponding PPI to the CPU interfaces.
  ADD      x1, x0, #GICR_ISENABLERoffset
  MOV      w2, #(1 << 29)     // Enable INTID 29
  STR      w2, [x1]           // Enable interrupt source
  ISB
  //*********************************************************

  RET

//---------------------------------------------
// gets the interrupt id from interrupt ack reg
//---------------------------------------------
  .type getIntidAckReg0, "function"
getIntidAckReg0:
   // Check interrupt ID for Group 0
  // EL3 can access EL1 registers, but EL1 can not access EL3 registers
  // this is group 0 register
  // Read interrupt acknowledge register into x0
  MRS       x0, ICC_IAR0_EL1
  RET

//---------------------------------------------
// sets the interrupt id in the end of interrupt reg
//---------------------------------------------
  .type setIntidEndReg0, "function"
setIntidEndReg0:
  // signal end of interrupt for Group 0
  // EL3 can access EL1 registers, but EL1 can not access EL3 registers
  // this is group 0 register Interrupt Controller End Of Interrupt Register
  //// Write x0 to end of interrupt reg
  MSR        ICC_EOIR0_EL1, x0
  RET

