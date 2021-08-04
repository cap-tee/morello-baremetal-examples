/*
 ============================================================================
 Name        : trustedFW_EL2.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : Basic test program to interface to the trusted firmware as a
               stand-a-lone program from EL2 - normal world.
               Basic set up tests, including printf re-direct
               Basic tests to the trusted firmware using SMCCC include:
               SMCCC_VERSION - get SMCC version - function ID 0x80000000
               check if the service function SMCCC_ARCH_WORKAROUND_1 is implemented
               check if the service function SMCCC_ARCH_WORKAROUND_2 is implemented
               check if the service function SMCCC_ARCH_SOC_ID is implemented
               see the Arm SMC Calling convention document for more details
               Check the PSCI version
               Check if the service SMC_PSCI_STAT_RESIDENCY64 is implemented
               Check if the service SMC_PSCI_CPU_FREEZE is implemented
               Check if the service SMC_PSCI_SYSTEM_SUSPEND64 is implemented
               Check if the TRNG service is implemented
               If TRNG exists, obtain 3 random numbers
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//include a different printf function to redirect to uart using _putchar()
//(had problems trying to redirect the default printf function as it didn't call putc/fputc)
#include "printf.h"

// include uart functions
#include "trustedFW_EL2funcs.h"
#include "uart_redirect.h"

extern void smcCall(void);
extern void el2nmmu(void);
extern void fptest(void);
extern void looptest(void);

int main(void) {

	//-------------------------------------------------------------------
	// SET UP
	// set up to use all of the DRAM0 memory and peripherals
	el2nmmu();
	//-------------------------------------------------------------------
	// BASIC TESTS
	// set up the memory mapped uart pl011 standard setup at 0x1C090000
	uartRSetup((void*)(0x1C090000));
	// print string test to uart
	stringTest();
	printf("Performing basic tests.............\n");
	// basic smc call test into the trusted firmware
	smcCall();
	printf("smc call test .... done\n");
	// floating point register test
	fptest();
	printf("floating point register test .... done\n");
	printf("Basic tests complete.............\n\n");
	//--------------------------------------------------------------------
	// TRUSTED FIRMWARE TESTS
	printf("Performing trusted firmware tests.............\n");
	// call into the trusted firmware, get and print the smccc version
	int32_t ret_ver = getVersion();
	// if the version is 1.1 or later we can determine the availability
	// of an Arm Architecture Service function (Morello-Android uses
	// version 1.2)
	// check to see if the service function SMCCC_ARCH_WORKAROUND_1 is implemented
	int32_t ret_resultWK1 = checkService(SMCCC_ARCH_WORKAROUND_1, "SMCCC_ARCH_WORKAROUND_1");
	// check to see if the service function SMCCC_ARCH_WORKAROUND_2 is implemented
    int32_t ret_resultWK2 = checkService(SMCCC_ARCH_WORKAROUND_2, "SMCCC_ARCH_WORKAROUND_2");
	// check to see if the service function SMCCC_ARCH_SOC_ID is implemented
    int32_t ret_resultSOC = checkService(SMCCC_ARCH_SOC_ID, "SMCCC_ARCH_SOC_ID");
    //check PSCI features - part of standard service call
    int ret_PSCIversion = checkPSCIversion();
    int32_t ret_PSCIres = checkPSCIfeature(SMC_PSCI_STAT_RESIDENCY64, "SMC_PSCI_STAT_RESIDENCY64");
    int32_t ret_PSCIfreeze = checkPSCIfeature(SMC_PSCI_CPU_FREEZE, "SMC_PSCI_CPU_FREEZE");
    int32_t ret_PSCIsuspend = checkPSCIfeature(SMC_PSCI_SYSTEM_SUSPEND64, "SMC_PSCI_SYSTEM_SUSPEND64");
    // if the random number generator function is present, get 3 random 8 bit numbers
    for (int i=1; i<=3; i++)
    {
    	int ret_ranNum = getRandomNumber();
    	if (ret_ranNum == TRNG_E_NOT_SUPPORTED) break;
    }
    printf("Trusted firmware tests complete.............\n");
	//--------------------------------------------------------------------
	//stop here
	looptest();


	return EXIT_SUCCESS;
}
