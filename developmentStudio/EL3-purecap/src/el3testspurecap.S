/*
 ============================================================================
 Name        : el3testspurecap.S
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : Testing in purecap mode. Performs bounds and permissions tests
 				tests store and load capability into memory

 				Note: this file has capital S extension. This means the compiler
 				knows it needs preprocessing since it contains #ifdef to determine
 				if purecap or normal Morello code should be compiled.
 ============================================================================
 */

//*****************************************
// SECTION
//*****************************************
   // This section should automatically go into secure memory region
  .section  .SECUREel3purecaptesting_ass,"ax"
  .align 3

//*****************************************
// DEFINES
//*****************************************

// purecap defined by compiler, so no need to define here:
// morello:     	__CHERI__
// +c64:			__ARM_FEATURE_C64
// purecap:	 	__CHERI_PURE_CAPABILITY__

// Note: we are only compiling for Morello, or purecap
// we can't have purecap without __ARM_FEATURE_C64 making
// this #define redundant in this case, __CHERI__
// was for hybrid and is legacy.
// see morello-android slack discussion

// function to set DDC
.global el3DDCset

// function to do bounds tests
.global el3BoundsTest

// function to do permissions tests
.global el3PermsTest

// function to populate table
.global poptable

// function to store capability in memory
.global el3StoreCapTest

// function to load capability from memory
.global el3LoadCapTest

//********************************************
// FUNCTIONS
//*******************************************
//-------------------------------------------
// el3DDCset
// Set up the default data capability register
//-------------------------------------------
  .type el3DDCset, "function"
el3DDCset:
 // #ifdef must be lowercase
 #ifdef __CHERI_PURE_CAPABILITY__
    // set default data capability permissions from c15
    // set to maximum bounds
  	MSR       DDC, c15
 #endif
RET

//-------------------------------------------
// el3BoundsTest
// perform bounds testing on purecap
//-------------------------------------------
  .type el3BoundsTest, "function"
el3BoundsTest:
  // EL3 pointer to start of memory location
  // Get address of memory
  LDR      x1, =EL3_MEM_TABLE
  #ifdef __CHERI_PURE_CAPABILITY__
    //get default data capability permissions
  	MRS       c15, DDC
    //put x1 value into c1 with default permissions
  	SCVALUE   c1, c15, x1
  #endif
  //---------------------------------------------------------------
  // testing bounds on memory table for purecap
  #ifdef __CHERI_PURE_CAPABILITY__
    // View bounds for current capability
    // should be full range of memory space
  	// get base of capability and put into x2
  	GCBASE x2, c1
  	// get limit of capability and put into x2
  	GCLIM x2, c1
  	// get length of capability and put into x2
  	GCLEN x2, c1
  	// get permissions of capability and put into x2
  	GCPERM x2, c1

 	// change bounds
 	// set length of bounds from the base address(address of table)
 	// make sure the length is enough to write all the table values
 	// otherwise a capability exception will occur
 	// set length, 4 bytes per mem location, but writes in two blocks,
 	//    so 8 bytes, (4 entries x 8 =32 min)
 	MOV x3, #40 //above min
 	 	SCBNDSE c1,c1,x3 //set capability bounds
 	// get base of capability and put into x2 - this is the table addr
  	GCBASE x2, c1
  	// get limit of capability and put into x2 - this is table addr + length
  	GCLIM x2, c1
  	// get length of capability and put into x2 - this is length only
  	GCLEN x2, c1
  	// get permissions of capability and put into x2
  	GCPERM x2, c1

  	// try reducing bounds on same capability,
  	// this is allowed because you are making the bounds smaller
  	MOV x3, #32 // set length to min 32. Try setting to less than min to see it cause an exception
 	SCBNDSE c1,c1,x3 // set capability bounds
  	// get base of capability and put into x2 - this is the table addr
  	GCBASE x2, c1
  	// get limit of capability and put into x2 - this is table addr + length
  	GCLIM x2, c1
  	// get length of capability and put into x2 - this is length only
  	GCLEN x2, c1
  	// get permissions of capability and put into x2
  	GCPERM x2, c1

  	// try increasing bounds on same capability,
  	// this causes the tag to be cleared because can not increase bounds,
  	// you can only reduce it
  	// MOV x3, #0x40 // set length - comment this line out to run to the end of the function
 	SCBNDSE c1,c1,x3 // set capability bounds
  	// get base of capability and put into x2 - this is the table addr
  	GCBASE x2, c1
  	// get limit of capability and put into x2 - this is table addr + length
  	GCLIM x2, c1
  	// get length of capability and put into x2 - this is length only
  	GCLEN x2, c1
  	// get permissions of capability and put into x2
  	GCPERM x2, c1
  #endif
  // end of testing
  //----------------------------------------------------------------

  B poptable  // populate table in memory
  RET

//-------------------------------------------
// el3PermissionsTest
// perform permissions testing on purecap
//-------------------------------------------
  .type el3PermsTest, "function"
el3PermsTest:
  // EL3 pointer to start of memory location
  // Get address of memory
  LDR      x1, =EL3_MEM_TABLE
  #ifdef __CHERI_PURE_CAPABILITY__
    // get default data capability permissions
  	MRS       c15, DDC
    // put x1 value into c1 with default permissions
  	SCVALUE   c1, c15, x1
  #endif
  //---------------------------------------------------------------
  // testing permissions on memory table for purecap
  #ifdef __CHERI_PURE_CAPABILITY__
    // View perms for current capability
    // should be full range of memory space
  	// get permissions of capability and put into x2
  	GCPERM x2, c1

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
 	// try changing this from load to store - will cause exception
    ORR    x2, x2, #(1 << 17) // load
 	CLRPERM c1, c1, x2 //reduce permissions and clear load

  	// get permissions of capability and put into x2 to check
  	GCPERM x2, c1
  #endif
  	B poptable  // populate table in memory

  	RET

//-------------------------------------------
// poptable
// populate table in memory
//-------------------------------------------
  .type poptable, "function"
poptable:
  // Put entries into table
  // First table entry
  MOV     x0, #0x401
  // Lower entry
  #ifdef __CHERI_PURE_CAPABILITY__
    STR      x0, [c1]
  #else
    STR      x0, [x1]
  #endif
  // Upper entry - 0x0

  // Second table entry
  MOV      x0, #0x402
  // Lower entry
  #ifdef __CHERI_PURE_CAPABILITY__
  	STR      x0, [c1, #8]
  #else
    STR      x0, [x1, #8]
  #endif
  // Upper entry - 0x0

  // Third table entry
  // Lower entry
    MOV      x0, #0x403
  // Upper entry
  ORR      x0, x0, #(1 << 60)
  ORR      x0, x0, #(1 << 61)
  #ifdef __CHERI_PURE_CAPABILITY__
  	STR      x0, [c1, #16]
  #else
    STR      x0, [x1, #16]
  #endif

  // Fourth table entry
  // lower entry
  MOV      x0, #0x404
  // Upper entry
  ORR      x0, x0, #(1 << 60)
  ORR      x0, x0, #(1 << 61)
  #ifdef __CHERI_PURE_CAPABILITY__
  	STR      x0, [c1, #24]
  #else
  	STR      x0, [x1, #24]
  #endif
  // data synchronisation - make sure all data written
  DSB      SY
  RET

//-------------------------------------------
// el3StoreCapTest
// test to store capability in memory
//-------------------------------------------
  .type el3StoreCapTest, "function"
el3StoreCapTest:
  // EL3 pointer to start of DATA memory location
  // Get address of memory
  LDR      x1, =EL3_MEM_TABLE
  #ifdef __CHERI_PURE_CAPABILITY__
    // get default data capability permissions
  	MRS       c15, DDC
    // put x1 value into c1 with default permissions
    // c1 is now capability pointer to EL3_MEM_TABLE
  	SCVALUE   c1, c15, x1
  	// we now need to store c1 in memory
  #endif

  // EL3 pointer to start of CAPABILITY memory location
  // Get address of memory
  LDR      x2, =EL3_CAP_TABLE
  #ifdef __CHERI_PURE_CAPABILITY__
    // get default data capability permissions
  	MRS       c15, DDC
  	// put x1 value into c2 with default permissions
    // c2 is now capability pointer to EL3_CAP_TABLE
  	SCVALUE   c2, c15, x2
  #endif

  #ifdef __CHERI_PURE_CAPABILITY__
  	STR      c1, [c2]
  #else
  	STR      x1, [x2]
  #endif
  // data synchronisation - make sure all data written
  DSB      SY
  RET

//-------------------------------------------
// el3LoadCapTest
// test to load capability in memory
//-------------------------------------------
  .type el3LoadCapTest, "function"
el3LoadCapTest:
  // EL3 pointer to start of CAPABILITY memory location
  // Get address of memory
  LDR      x2, =EL3_CAP_TABLE
  #ifdef __CHERI_PURE_CAPABILITY__
    // get default data capability permissions
  	MRS       c15, DDC
  	// put x1 value into c2 with default permissions
    // c2 is now capability pointer to EL3_CAP_TABLE
  	SCVALUE   c2, c15, x2
  #endif

  // clear x1 (and c1)
  MOV x1, #0

  // load capability into c1 from memory
  #ifdef __CHERI_PURE_CAPABILITY__
  	LDR      c1, [c2]
  #else
  	LDR      x1, [x2]
  #endif
  // data synchronisation - make sure all data written
  DSB      SY
  RET

  //*****************************************
  // SECTION
  //*****************************************
  // ------------------------------------------------------------
  // Table data
  // ------------------------------------------------------------
  // This section should automatically go into secure memory region
  .section  .SECUREel3purecaptable_ass,"ax"
  .align 12

// DATA
  .global EL3_MEM_TABLE
EL3_MEM_TABLE:
//  .fill 4096 , 1 , 0
  .space 4096 , 0

// CAPABILITIES
    .global EL3_CAP_TABLE
EL3_CAP_TABLE:
//  .fill 4096 , 1 , 0
  .space 4096 , 0
