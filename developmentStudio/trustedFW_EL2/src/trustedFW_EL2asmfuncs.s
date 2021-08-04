/*
 ============================================================================
 Name        : trustedFW_EL2funcs.s
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : functions for interfacing to the trusted firmware
 ============================================================================
 */
  /*
 Part of this code has been extracted from the Trusted Firmware-A tests repository

 https://git.trustedfirmware.org/TF-A/tf-a-tests.git/ d6715f10.....

 under the following license (https://spdx.org/licenses/BSD-3-Clause.html):
 */
/*
 * Copyright (c) 2013-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 /* The relevant sections extracted from files are highlighted */


 //****************************************************************************
 // SECTION AND DEFINES
 //****************************************************************************
  .align 16

   // smc call to trusted fimware
  .global asm_tftf_smc64
  .type asm_tftf_smc64, @function

  // basic smc call function
  .global smcCall
  .type smcCall, @function

    // fp reg test
  .global fptest
  .type fptest, @function

   // stay in loop
  .global looptest
  .type looptest, @function

  //***********************************************************
 // TRUSTED FIRMWARE TESTS FUNCTIONS
 //***********************************************************

 //-------------------------------------------------------------------------------
// This section of code has been extracted/modified from Trusted Firmware-A tests
// file ft-a-tests/lib/smc/aarch64/asm_smc.S
.section        .text, "ax"

	.macro smccc_conduit _conduit

	/*
	 * According to the AAPCS64, x8 is the indirect result location
	 * register. It contains the address of the memory block that the caller
	 * has reserved to hold the result, i.e. the smc_ret_values structure
	 * in our case.
	 * x8 might be clobbered across the SMC call so save it on the stack.
	 * Although x8 contains an 8 byte value, we are allocating 16bytes on the stack
	 * to respect 16byte stack-alignment.
	 */
	str	x8, [sp, #-16]!

	/* "Conduit" arguments are already stored in x0-x7 */
	\_conduit	#0

	/* Pop x8 into a caller-saved register */
	ldr	x9, [sp], #16

	/*
	 * Return values are stored in x0-x7, put them in the 'smc_ret_values'
	 * return structure
	 */
	stp	x0, x1, [x9, #0]
	stp	x2, x3, [x9, #16]
	stp	x4, x5, [x9, #32]
	stp	x6, x7, [x9, #48]

	.endm
 //
 /* ---------------------------------------------------------------------------
 * smc_ret_values asm_tftf_smc64(uint32_t fid,
 *				 u_register_t arg1,
 *				 u_register_t arg2,
 *				 u_register_t arg3,
 *				 u_register_t arg4,
 *				 u_register_t arg5,
 *				 u_register_t arg6,
 *				 u_register_t arg7);
 * ---------------------------------------------------------------------------
 */
asm_tftf_smc64:
	smccc_conduit smc
	RET
//-----------------------------------------------------------------------------


  //***********************************************************
 // CAP-TEE FUNCTIONS
 //***********************************************************
 //-----------------------------------------------------------
 // smcCall
 // smc test with immediate value
 // test just to see if correctly goes into EL3 and back to EL2N
 //-----------------------------------------------------------
  smcCall:
  SMC 0x02
  RET

 //-----------------------------------------------------------
 // fptest
 // test floating point registers can be loaded without causing an exception
 //-----------------------------------------------------------
  fptest:
  MOV x0, #0x01ca
  MOVK x0, #0x8000,LSL #16
  MOV x1, #0x01e0
  MOVK x1, #0x8000,LSL #16
  LDR q0,[x0]
  LDR q1,[x1]

  RET

 //-----------------------------------------------------------
 //  looptest
 // sit in a loop forever
 //-----------------------------------------------------------

  looptest:
  LOOP:
  B LOOP
  RET



