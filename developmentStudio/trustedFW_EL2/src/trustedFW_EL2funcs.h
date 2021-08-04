/*
 ============================================================================
 Name        : trustedFW_EL2funcs.h
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description :  Definitions and functions to interface to the trusted firmware-A
 ============================================================================
 */
 /*
 Part of this code has been extracted from the Trusted Firmware-A tests repository

 https://git.trustedfirmware.org/TF-A/tf-a-tests.git/ d6715f10.....

 under the following license (https://spdx.org/licenses/BSD-3-Clause.html):
 */
 /*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 /* The relevant sections extracted from files are highlighted */

#ifndef __trustedFW_EL2funcs_h
#define __trustedFW_EL2funcs_h

//functions
int32_t getVersion(void);
int32_t checkService(uint32_t service_id, const char* service_text);
int stringTest(void);
unsigned int checkPSCIversion(void);
int32_t checkPSCIfeature(uint32_t psci_func_id, const char* psci_func_text);
int getRandomNumber(void);

//-------------------------------------------------------------------------------
// This section of code has been extracted/modified from Trusted Firmware-A tests
// file ft-a-tests/tftf/include/lib/utils_def.h
/*
 * For those constants to be shared between C and other sources, apply a 'U',
 * 'UL', 'ULL', 'L' or 'LL' suffix to the argument only in C, to avoid
 * undefined or unintended behaviour.
 *
 * The GNU assembler and linker do not support these suffixes (it causes the
 * build process to fail) therefore the suffix is omitted when used in linker
 * scripts and assembler files.
*/
#if defined(__LINKER__) || defined(__ASSEMBLY__)
# define   U(_x)	(_x)
# define  UL(_x)	(_x)
# define ULL(_x)	(_x)
# define   L(_x)	(_x)
# define  LL(_x)	(_x)
#else
# define   U(_x)	(_x##U)
# define  UL(_x)	(_x##UL)
# define ULL(_x)	(_x##ULL)
# define   L(_x)	(_x##L)
# define  LL(_x)	(_x##LL)
#endif
//------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// This section of code has been extracted/modified from Trusted Firmware-A tests
// file ft-a-tests/tftf/include/runtime_services/arm_arch_svc.h
//SMC arguments
//Retrieve the implemented version of the SMC Calling Convention
// parameter uint32 Function ID 0x80000000
#define SMCCC_VERSION 0x80000000 //SMC doc section 7.2
//Determine the availability and capability of Arm Architecture Service functions
// parameter uint32 Function ID 0x80000001
#define SMCCC_ARCH_FEATURES	0x80000001 //SMC doc section 7.3
//services
#define SMCCC_ARCH_SOC_ID	0x80000002
#define SMCCC_ARCH_WORKAROUND_1	0x80008000
#define SMCCC_ARCH_WORKAROUND_2	0x80007FFF
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// This section of code has been extracted/modified from Trusted Firmware-A tests
// file ft-a-tests/include/runtime_services/smcc.h
#define SMCCC_VERSION_MAJOR_SHIFT	U(16)
#define SMCCC_VERSION_MAJOR_MASK	U(0x7FFF)
#define SMCCC_VERSION_MINOR_SHIFT	U(0)
#define SMCCC_VERSION_MINOR_MASK	U(0xFFFF)
#define MAKE_SMCCC_VERSION(_major, _minor) \
	((((uint32_t)(_major) & SMCCC_VERSION_MAJOR_MASK) << \
						SMCCC_VERSION_MAJOR_SHIFT) \
	| (((uint32_t)(_minor) & SMCCC_VERSION_MINOR_MASK) << \
						SMCCC_VERSION_MINOR_SHIFT))

#define SMC_UNKNOWN			-1
#define SMC_OK				0

/* Return codes for Arm Architecture Service SMC calls */
#define SMC_ARCH_CALL_SUCCESS           0
#define SMC_ARCH_CALL_NOT_SUPPORTED     -1
#define SMC_ARCH_CALL_NOT_REQUIRED      -2
#define SMC_ARCH_CALL_INVAL_PARAM       -3
//---------------------------------------------------------------------

//-------------------------------------------------------------------------------
// This section of code has been extracted/modified from Trusted Firmware-A tests
// file ft-a-tests/include/runtime_services/psci.h
#define SMC_PSCI_VERSION			0x84000000
#define SMC_PSCI_FEATURES			0x8400000a
#define SMC_PSCI_STAT_RESIDENCY64	0xc4000010
#define SMC_PSCI_CPU_FREEZE			0x8400000b
#define SMC_PSCI_SYSTEM_SUSPEND64	0xc400000e
//version
#define PSCI_MAJOR_VER		1
#define PSCI_MINOR_VER		1
#define PSCI_MAJOR_VER_SHIFT	16
#define PSCI_MAJOR_VER_MASK	0xFFFF0000
#define PSCI_VERSION(major, minor)	((major << PSCI_MAJOR_VER_SHIFT) \
					| minor)
//error code
#define PSCI_E_NOT_SUPPORTED	-1
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// This section of code has been extracted/modified from Trusted Firmware-A tests
// file ft-a-tests/include/runtime_services/trng.h
#define SMC_TRNG_VERSION			0x84000050
//error code
#define TRNG_E_NOT_SUPPORTED	(-1)
//arch64 random number generator funcID
#define SMC_TRNG_RND				0xc4000053 //aarch64
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// This section of code has been extracted/modified from Trusted Firmware-A tests
// file ft-a-tests/tftf/include/lib/libc/stdint.h
typedef unsigned long u_register_t;
typedef unsigned int uint32_t;
//-----------------------------------------------------------------------

//-------------------------------------------------------------------------------
// This section of code has been extracted/modified from Trusted Firmware-A tests
// file ft-a-tests/tftf/include/lib/tftf_lib.h
/*
 * SMC calls take a function identifier and up to 7 arguments.
 * Additionally, few SMC calls that originate from EL2 leverage the seventh
 * argument explicitly. Given that TFTF runs in EL2, we need to be able to
 * specify it.
 */
typedef struct {
	/* Function identifier. Identifies which function is being invoked. */
	//uint32 Function ID
	uint32_t	fid;

	u_register_t	arg1;
	u_register_t	arg2;
	u_register_t	arg3;
	u_register_t	arg4;
	u_register_t	arg5;
	u_register_t	arg6;
	u_register_t	arg7;
} smc_args;

/* SMC calls can return up to 8 register values */
typedef struct {
	u_register_t	ret0;
	u_register_t	ret1;
	u_register_t	ret2;
	u_register_t	ret3;
	u_register_t	ret4;
	u_register_t	ret5;
	u_register_t	ret6;
	u_register_t	ret7;
} smc_ret_values;
//-----------------------------------------------------------------------

//-------------------------------------------------------------------------------
// This section of code has been extracted/modified from Trusted Firmware-A tests
// file ft-a-tests/tftf/lib/smc/aarch64/smc.c
/*
 * Trigger an SMC call.
 */
smc_ret_values tftf_smc(const smc_args *args);
//-----------------------------------------------------------------------

#endif

