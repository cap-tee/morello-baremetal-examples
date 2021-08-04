/*
 ============================================================================
 Name        : trustedFW_EL2funcs.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : Functions to interface to the trusted firmware-A
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



//*************************************************
// INCLUDES
//*************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// include uart functions
#include "trustedFW_EL2funcs.h"
#include "uart_redirect.h"

//include a different printf function to redirect to uart using _putchar()
//(had problems trying to redirect the default printf function as it didn't call putc/fputc)
#include "printf.h"

//-------------------------------------------------------------------------------
// This section of code has been extracted/modified from Trusted Firmware-A tests
// file ft-a-tests/lib/smc/aarch64/smc.c
extern smc_ret_values asm_tftf_smc64(uint32_t fid,
			      u_register_t arg1,
			      u_register_t arg2,
			      u_register_t arg3,
			      u_register_t arg4,
			      u_register_t arg5,
			      u_register_t arg6,
			      u_register_t arg7);

smc_ret_values tftf_smc(const smc_args *args)
{
	return asm_tftf_smc64(args->fid,
			      args->arg1,
			      args->arg2,
			      args->arg3,
			      args->arg4,
			      args->arg5,
			      args->arg6,
			      args->arg7);
}
//----------------------------------------------------------------------------------


//************************************
// FUNCTIONS
//************************************
//------------------------------------
// getVersion
// get smccc version
//------------------------------------
int32_t getVersion(void)
{
	//-------------------------------------------------------------------------------
	// This section of code has been extracted/modified from Trusted Firmware-A tests
	// file ft-a-tests/tftf/tests/performance_tests/smc_latencies.c
	// create structure for smc arguments to pass to trusted firmware
	smc_args args;
	memset(&args, 0, sizeof(args));
	// set the function ID to get the smccc version
	args.fid = SMCCC_VERSION;
	// create structure for smc return values passed from trusted firmware
	smc_ret_values ret;
	// returned version
	int32_t ret_ver;
	// get the ret values from running the smc command with the args supplied
	ret=tftf_smc(&args);
	//---------------------------------------------------------------------------

	//print out what version exists
		printf("The smccc version is: ");
		ret_ver = (int32_t)ret.ret0;
		switch (ret_ver)
		{
		case MAKE_SMCCC_VERSION(1, 1):
			printf(" v1.1\n");
		    break;
		case MAKE_SMCCC_VERSION(1, 2):
			printf(" v1.2\n");
	        break;
		case SMC_ARCH_CALL_NOT_SUPPORTED:
			printf(" v1.0\n");
		    break;
		default:
			printf(" an error\n");
		    break;
		}

	//return the version number
	return ret_ver;
}

//------------------------------------
// checkService
// check if arm architecture service exists
//------------------------------------
int32_t checkService(uint32_t service_id, const char* service_text)
{
	//-------------------------------------------------------------------------------
	// This section of code has been extracted/modified from Trusted Firmware-A tests
	// file ft-a-tests/tftf/tests/performance_tests/smc_latencies.c
	// create structure for smc arguments to pass to trusted firmware
	smc_args args;
	memset(&args, 0, sizeof(args));
	args.fid = SMCCC_ARCH_FEATURES;
	args.arg1 = service_id;
	// create structure for smc return values passed from trusted firmware
	smc_ret_values ret;
	// returned result
	int32_t ret_result;
	// get the ret values from running the smc command with the args supplied
	ret=tftf_smc(&args);
	//---------------------------------------------------------------------------

	//print out whether service exists
	printf("The service ");
	printf(service_text);
	ret_result = (int32_t)ret.ret0;
	//check for success
	if (ret_result>=SMC_ARCH_CALL_SUCCESS)
	{
		printf(" is supported\n");
	}
	// or get the error code
	else
	{
		switch (ret_result)
		{
		case SMC_ARCH_CALL_NOT_SUPPORTED:
			printf(" is not supported\n");
			break;
		case SMC_ARCH_CALL_NOT_REQUIRED:
			printf(" is not required\n");
			break;
		case SMC_ARCH_CALL_INVAL_PARAM:
			printf(" is not valid\n");
			break;
		default:
			printf(" is an error\n");
			break;
		}
	}
	//return the result as to whether the service is available
	return ret_result;

}

//------------------------------------
// checkPSCIversion
// check PSCI version
//------------------------------------
unsigned int checkPSCIversion(void)
{
//-------------------------------------------------------------------------------
// This section of code has been extracted/modified from Trusted Firmware-A tests
// file ft-a-tests/lib/psci/psci.c
		smc_args args = { SMC_PSCI_VERSION };
		smc_ret_values ret;

		ret = tftf_smc(&args);
//-------------------------------------------------------------------------------

		int version = ret.ret0;
		uartRTransmitString("The PSCI version is: ");
		switch (version)
		{
		case PSCI_VERSION(1, 1):
			printf(" 1.1\n");
			break;
		case PSCI_VERSION(1, 0):
			printf(" 1.0\n");
			break;
		case PSCI_VERSION(0, 2):
			printf(" 0.2\n");
			break;
		case PSCI_VERSION(0, 1):
			printf(" 0.1\n");
			break;
		default:
			printf(" invalid version\n");
		}

		return version;
	}

//------------------------------------
// checkPSCIfeature
// discover whether a specific PSCI function is implemented and its features
//------------------------------------
int32_t checkPSCIfeature(uint32_t psci_func_id, const char* psci_func_text)
{
	//-------------------------------------------------------------------------------
	// This section of code has been extracted/modified from Trusted Firmware-A tests
	// file ft-a-tests/lib/psci/psci.c
	smc_args args = {SMC_PSCI_FEATURES,	psci_func_id};
	smc_ret_values ret;
	ret = tftf_smc(&args);
	//-------------------------------------------------------------------------------

	//print out whether function exists
	printf("The PSCI function ");
	printf(psci_func_text);
	if (ret.ret0 == PSCI_E_NOT_SUPPORTED)
		{
		printf(" is not supported\n");
		}
	else
		{
		printf(" is supported\n");
		}
	return ret.ret0;
}


//------------------------------------
// getRandomNumber
// get a true random number
// either return an error code (not supported), or a random number
//------------------------------------
int getRandomNumber(void)
{
//-------------------------------------------------------------------------------
// This section of code has been extracted/modified from Trusted Firmware-A tests
// file ft-a-tests/lib/trng/trng.c
//check version
	smc_args args = { SMC_TRNG_VERSION };
	smc_ret_values ret;
	ret = tftf_smc(&args);
	int32_t version = ret.ret0;

	if (version == TRNG_E_NOT_SUPPORTED)
	{
		printf("True random number generator function is not supported\n");
	}
	else
	{
		printf("True random number generator function is present\n");
		printf("Getting an 8 bit random number...\n");
		//get a random number
		uint32_t nbits = 8; //8 bit random number
		smc_args args = {SMC_TRNG_RND,nbits};
		smc_ret_values ret;
		ret = tftf_smc(&args);
//-------------------------------------------------------------------------------
		int ranNum = (int)ret.ret0;
		printf("Number is: %d\n", ranNum);
	}
return (int)ret.ret0;
}


//------------------------------------
// stringTest
// performs and prints some string tests
//------------------------------------

int stringTest(void)
{
	//uses instructions in program to create hello
	//uses registers for 6f(0) 6c6c(ll) 65(e) 48(h)
	//puts in .text section - locally with program code
	char text1[6] = "Hello";
	// stores string in .rodata section, placement determined by linker script
	// uses q0 register - quad word reg used for floating point
	// so need to disable trapping of fp register in start up code - CPTR_EL2
	char text2[27] = "abcdefghijklmnopqrstuvwxyz";
	// stores string in .rodata section, placement determined by linker script
	// uses w registers to store, not SIMD q0
    const char text3[6] = "Hello";
    // stores string in .rodata section, uses SIMD reg
	const char text4[27] = "abcdefghijklmnopqrstuvwxyz";
	// uses w registers to store, not SIMD q0
	char text5[] = "Hello1\n";
	// stores string in .rodata section, uses SIMD reg
	char text6[] = "abcdefghijklmnopqrstuvwxyz\n";
	// uses w registers to store, not SIMD q0
	const char text7[] = "Hello2\n";
	// stores string in .rodata section, uses SIMD reg
	const char text8[] = "abcdefghijklmnopqrstuvwxyz\n";

	// send string to uart
	uartRTransmitString("\nPerforming string test........\n");
	printf("printf re-direct test after uart set up\n");
	printf("This printf test prints a character, %c\na number, %d\na floating point, %f\n", 'z', 123, 456.789);
	printf((const char *)text1); printf("\n");//not const char
	printf((const char *)text2); printf("\n");//not const char
	printf(text3); printf("\n");
	printf(text4); printf("\n");
	printf((const char *)text5);
	printf((const char *)text6);
	printf(text7);
	printf(text8);
	uartRTransmitString("Finished string test........\n\n");
	return EXIT_SUCCESS;
}



