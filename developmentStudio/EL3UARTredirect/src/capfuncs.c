/*
 ============================================================================
 Name        : capfuncs.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2001
 Description : supporting functions for purecap mode
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <cheriintrin.h>
#include "printf.h" //uses embedded printf function to direct to uart
//-----------------------------------------------------------------
// print out capability parameters using printf
//-----------------------------------------------------------------
void printcapabilityPar(void * cap)
{
  #ifdef __CHERI_PURE_CAPABILITY__
	 printf("\nbase address of capability is: 0x%lx\n", cheri_base_get(cap));
	 printf("length of capability is: 0x%lx\n", cheri_length_get(cap));
	 printf("offset from base address of capability is: 0x%lx\n", cheri_offset_get(cap));
	 printf("value of capability is: 0x%lx\n", cheri_address_get(cap));
	 printf("permissions of capability is: 0x%x\n", cheri_perms_get(cap));
	 printf("tag of capability is: 0x%01x\n", cheri_tag_get(cap));
	 printf(".............................................\n\n\n");
  #endif
}

