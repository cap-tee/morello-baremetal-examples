/*
 ============================================================================
 Name        : regForEL1S.S
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : setting up the registers to go to EL1 secure and performing an ERET
 ============================================================================
 */

 //****************************************************************************
 // SECTION AND DEFINES
 //****************************************************************************

 // goes into secure memory region by default
 .section .SECURERegEL1Ssection_ass, "ax"
 .align 4  // Align to 128bit/16byte boundary

 // Program state
 .equ PS0_EL1S, 0x05 // EL1S

 // function to set up registers and ERET to EL1S
  .global ERETtoEL1S

 //***********************************************************
 // FUNCTIONS
 //***********************************************************
 //-----------------------------------------------------------
 // ERETtoEL1N
 //-----------------------------------------------------------
  .type ERETtoEL1S, "function"
ERETtoEL1S:

  // Disable trapping CPTR_EL3
  // after start up CPTR_EL3 = 0x200, EC[9]
  // TCPAC[31] (0)=no trap some EL2/EL1 reg
  // TAM[30] (0)=no trap activity monitor
  // TTA[20] (0)= no trap trace reg
  // TFP[10] (0)= no trap SIMD/FP
  //--opposite traps
  // EC[9] (1)=no trap of morello instructions
  // EZ[8] (0)=trap of SVE
  #ifdef __CHERI_PURE_CAPABILITY__
  	// CPTR_EL3.EC [9] traps morello instructions at all EL (0)trap (1)no trap
  	// already set to no trap (1) in start up code, so do nothing
  #else
  	// Trapping at CPTR_EL3
    // Enable trapping of morello instructions - does not work otherwise
  	MSR      CPTR_EL3, xzr
  	ISB
  #endif

  // Disable trapping CPTR_EL2
  // Clearing all trap bits stops capability from working - so don't do this
  // disable trapping of floating point reg q0 - needed for long strings
  // disable trapping of morello specific instructions (uses a reserved armv8 bit) - needed for printf function
  // TFP, bit [10] when set to 1 Traps execution of EL2 instructions which access the Advanced SIMD and floating-point functionality,
  // TC, bit [9] when set to 1 traps morello instructions at EL2/EL1/EL0 like using capability registers which printf function uses
  MRS      x0, CPTR_EL2  // Read
  // create mask 1111 1001 1111 1111
  MOV x2, #0xF9FF
  MOVK x2, #0xFFFF,LSL #16
  // and mask with contents of register to set bit[10] and bit[9] to zero
  AND      x0, x0, x2 // disable
  MSR      CPTR_EL2, x0 // Write
  ISB

  // SET SECURE WORLD HERE
  // Set SCR_EL3 - secure configuration register to determine what happens at lower exception levels
  MSR      SCR_EL3, xzr
  MOV      w1, #0              //  NS (#0-EL0/EL1 are in Secure state, #1 is non secure)
  ORR 	   w1, w1, #(1 << 11)  // set ST bit (disable trapping of timer control registers)
  ORR	   w1, w1, #(1 << 10)  // set RW bit (next lower EL in aarch64)
  ORR      w1, w1, #(1 << 3)   // Set EA bit (SError routed to EL3)
  ORR      w1, w1, #(1 << 2)   // Set FIQ bit (FIQs routed to EL3)
  ORR      w1, w1, #(1 << 1)   // Set IRQ bit (IRQs routed to EL3)
  MSR      SCR_EL3, x1
  ISB
  //**ADDED THIS SECTION TO REMOVE DEBUGGER MMU WARNINGS*******************
//ADDED TO REMOVE DEBUGGER MMU WARNINGS FOR TTBR1 EL1 AND VTTBR EL2
// Although not using stage 2 translation, the debugger MMU complains if VTTBR is not set with sensible values
 //*********************************************************
 // Set SCTLR to safe values
 MSR     SCTLR_EL2, xzr
 //set VTTBR_EL2, and TTBR1_EL1 with dummy empty tables
 MSR     VTTBR_EL2, xzr
 LDR      x0, =tt_l1_base3
 MSR     VTTBR_EL2, x0
 MSR    TTBR1_EL1, x0
 ISB
//set up VTCR_EL2, so there are no conflicts between TGO,SLO and T0SZ, to please the debugger
 MOV      x0, #0x19                        // T0SZ=0b011001 Limits VA space to 39 bits, translation starts @ l1
 ORR      x0, x0, #(0x1 << 6)              // SLO[7:6] 01
								            //TGO 00
  MSR      VTCR_EL2, x0
  ISB
//*****************************************************************

 // Initialize SCTLR_EL1 before we can enter EL1
  MSR 	   SCTLR_EL1, xzr

  //#ifdef __CHERI_PURE_CAPABILITY__
	// set DDC to maximum bounds - don't need this as already set
	// MSR       DDC, c15
  //#endif

  // where to branch to when exception completes
  LDR      x0, =EL1S_secure_entry
  #ifdef __CHERI_PURE_CAPABILITY__
    //get default data capability permissions
  	MRS       c15, DDC
    //put x0 value into c0 with default permissions
  	SCVALUE   c0, c15, x0
    MSR 	   CELR_EL3, c0 // where to branch to when exception completes
  #else
    MSR 	   ELR_EL3, x0 // where to branch to when exception completes
  #endif

  // set the program state when the exception completes
  LDR	   x1, =PS0_EL1S //the program state
  #ifdef __CHERI_PURE_CAPABILITY__
  // Need to manually set c64 mode on an ERET, otherwise will be set to a64
  ORR x1,x1, #(1 << 26)
  #endif
  MSR      SPSR_EL3, x1
  ISB

  ERET  //This is ERET - LEAVES EL3

 //****************************************************************************
 // SECTION AND DEFINES
 //****************************************************************************

.align 4 //128 bit/16 byte boundary

  // function to initialise entry to EL1S, sets up stack and
  // registers and branches to main EL1S c code
  .global EL1S_secure_entry

   //***********************************************************
  // FUNCTIONS
  //***********************************************************
  // ------------------------------------------------------------
  // EL1 trusted (secure) AArch64
  // ------------------------------------------------------------
  // don't declare EL1S_normal_entry as a function because in
  // Morello-purecap causes the program counter to become missaligned by 1 byte
  // when retrieving the function label and causes an exception
  //.type EL1S_normal_entry, "function"

EL1S_secure_entry:
  // ENTERS EL1S HERE

   // set up root capabilitY for EL1
  // DDC - means DDC at current EL
  // DDC_EL1 - specifically means DDC at EL1
  // can't access DDC_EL1 directly, only through DDC at current EL
  #ifdef __CHERI_PURE_CAPABILITY__
    MRS     c15, DDC
    // 	Restrict bounds as defined in linker script
    //WARNING! - THIS DOESN'T INCUDE DEVICE MEMORY
    LDR	   x0, =_secure_ram_ORIGIN
    LDR	   x1, =_secure_ram_LENGTH
    // ensure aligned to 16B boundary
 	AND  x0, x0, ~15
   	AND  x1, x1, ~15
    // Set capability to point to start
    // put x0 value into c0 with default permissions
  	SCVALUE   c0, c15, x0
  	// set bounds to - `EXACT`
  	// for non-exact, use SCBNDS
  	// see linker script for additional bounds notes
 	SCBNDSE c0,c0,x1
 	// mov to root capability for EL1
  	MSR     DDC, c0
  	//--check stack pointer bounds and permissions
  	    // get value of capability and put into x4
  		GCVALUE x4, c0
       	// get base of capability and put into x4
  		GCBASE x4, c0
  		// get limit of capability and put into x4
  		GCLIM x4, c0
  		// get length of capability and put into x4
  		GCLEN x4, c0
  		// get permissions of capability and put into x4
  		GCPERM x4, c0
 #endif

 // Get stack address defined in linker script
  // Needs this because it looses the stack on entry to EL1, and doesn't work without it
  // STACK_SIZE = 0x20000;
  #ifdef __CHERI_PURE_CAPABILITY__
    // must use c0 for ADRP, stack not defined without linker
    //ADRP c0, _stack_el1s_base // if use this, sign extends to FFFFFFFF C0020000 and points to wrong place

    // Stack starts at base (upper memory) and grows down (lower memory)
    // However we have to set up the capability the other way around to
    // set the correct bounds (cannot set a negative bounds),
    // and then change the value to start at the base
    LDR	   x0, =_stack_el1s_limit //get unsigned stack limit address (lower point in memory)
    LDR	   x1, =_stack_el1s_base //get unsigned stack base address (upper point in memory)
    // ensure sp aligned to 16B boundary
    AND  x0, x0, ~15
    AND  x1, x1, ~15
    //Work out size of stack: base - limit to get a positive length
  	SUB     x3, x1, x0
    // get default bounds/permissions
    MRS       c15, DDC //get default bounds/permissions
    // Set capability to point to limit first
    // put x0 value into c0 with default permissions
  	SCVALUE   c0, c15, x0
  	// set bounds to stack size - `EXACT`
  	// for non-exact, use SCBNDS
  	// see linker script for additional bounds notes
 	SCBNDSE c0,c0,x3
 	// move the capability so it now points to the base
 	// where the stack needs to start from
 	// (Add the length to the limit)
 	ADD c0,c0,x3
	// Clear the execute permission bit as we don't want anything
 	// executing in the stack space
 	// change permissions
 	// [17] load
 	// [16] store
 	// [15] execute
 	// [14] load cap
 	// [13] store cap
 	// [12] store local cap
 	// [11] seal
 	// [10] unseal
 	// [9] system
 	// [8] branch sealed pair
 	// [7] compartment ID
 	// [6] mutable load
 	// [5:2] User[4]
 	MOV    x2, #0 //only set the bit to clear
    ORR    x2, x2, #(1 << 15) // execute
 	CLRPERM c0, c0, x2 //reduce permissions and clear execute
  	// mov to capability stack pointer
  	MOV  csp, c0
  	//--check stack pointer bounds and permissions
  	    // get value of capability and put into x4
  		GCVALUE x4, csp
       	// get base of capability and put into x4
  		GCBASE x4, csp
  		// get limit of capability and put into x4
  		GCLIM x4, csp
  		// get length of capability and put into x4
  		GCLEN x4, csp
  		// get permissions of capability and put into x4
  		GCPERM x4, csp
  	//--
  #else
    // get stack address from linker script
    // starts at base and grows down to limit
    ADRP x0, _stack_el1s_base
    // ensure sp aligned to 16B boundary
    AND  x0, x0, ~15
    // mov to stack pointer
    MOV  sp, x0
  #endif

  // We must ensure that floating point register accesses are not trapped
  // since the c library for AArch64-v8A uses them
  // make sure do not change any other bits
  MRS      x0, CPACR_EL1  // Read
  ORR      x0, x0, #(0x3 << 20)
  MSR      CPACR_EL1, x0 // Write

  //set up the mmu in the boot code here for EL1N
  //before the DDC_EL1 is nulled in purecap
  BL el1smmu

  //clear capability registers before branching
  #ifdef __CHERI_PURE_CAPABILITY__
    //leave root capability so can set up el1mmu
    //needs fixing - don't do in practice!
    //mmu needs setting up before getting to c code
    //keep like this for now as easier to see examples
	//MSR	ddc, czr
	MOV	x1, xzr
	MOV	x2, xzr
	MOV	x3, xzr
	MOV	x4, xzr
	MOV	x5, xzr
	MOV	x6, xzr
	MOV	x7, xzr
	MOV	x8, xzr
	MOV	x9, xzr
	MOV	x10, xzr
	MOV	x11, xzr
	MOV	x12, xzr
	MOV	x13, xzr
	MOV	x14, xzr
	MOV	x15, xzr
	MOV	x16, xzr
	MOV	x17, xzr
	MOV	x18, xzr
	MOV	x19, xzr
	MOV	x20, xzr
	MOV	x21, xzr
	MOV	x22, xzr
	MOV	x23, xzr
	MOV	x24, xzr
	MOV	x25, xzr
	MOV	x26, xzr
	MOV	x27, xzr
	MOV	x28, xzr
	MOV	x29, xzr
	MOV	x30, xzr
  #endif

  // Ensures that all instructions complete before branching
  ISB

  // Branch to the main EL1 secure code
  B        el1smain


  //**ADDED THIS SECTION TO REMOVE DEBUGGER MMU WARNINGS*******************
  .section  TT_DUMMY2,"ax"
  .align 12

  .global tt_l1_base3
tt_l1_base3:
  .fill 4096 , 1 , 0


