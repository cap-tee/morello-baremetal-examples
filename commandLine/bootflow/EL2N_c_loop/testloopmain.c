/*
 ============================================================================
 Name        : testloopmain.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : loop test from c
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
extern void testloop(void); 

int main(void) 
{
	testloop();
	return EXIT_SUCCESS;
}
