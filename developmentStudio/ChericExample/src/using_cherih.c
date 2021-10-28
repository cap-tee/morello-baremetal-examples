/*
 ============================================================================
 Name        : using_cherih.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2001
 Description : Uses the cheri.h header file to use some of the cheri API functions
 	 	 	 	 to inspect capability parameters from c
 ============================================================================
 */
 
 
 
#include <stdio.h>
#include <stdlib.h>
//purecap mode
#ifdef	__CHERI_PURE_CAPABILITY__

	#include <stddef.h>	//includes ptraddr_t def, Note: vaddr_t(as specified in cheri c/c++ doc 2020 version) got renamed to ptraddr_t
	#include <cheri.h> //includes cheri API functions


	//VALID CAPABILITIES

	//1. capability teststr1
    //global pointer, and string, and variables
	char *teststr1 = "Hello World from EL3\n";
	size_t teststr1_length; //cheri c doc says use size_t, so compatible with 32/64 bit architectures
	ptraddr_t teststr1_base; //cheri c doc says use vaddr_t
	size_t teststr1_offset;//cheri c doc says use size_t
	size_t teststr1_perms;//cheri c doc says use size_t, but perms unsigned int, and offset is unsigned long int
	_Bool teststr1_tag;//cheri c doc says use _bool, so same as function return type

	//2. capability teststr2
	//global string, but local pointer and variables
	const char teststring[27] = {"abcdefghijklmnopqrstuvwxyz"};

	//3. capability teststr3
	//local string, pointer and variables

    //4. capability PCCtest
	// global pointer to program counter cheri function
	int *PCCtest;
	size_t PCCtest_length;
	ptraddr_t PCCtest_base;
	size_t PCCtest_offset;
	size_t PCCtest_perms;
	_Bool PCCtest_tag;

	//5. capability numPtr
	//test pointer to array of numbers
	int fixedNumArray[3] = {78, 6, 45};
	int *numPtr = fixedNumArray;
	size_t numPtr_length;
	ptraddr_t numPtr_base;
	size_t numPtr_offset;
	size_t numPtr_perms;
	_Bool numPtr_tag;

	//6. capability settest
	int numArray[30] = {45,67,89}; //assign size of 30: 30x4bytes=120bytes
	int *settest = numArray;

	size_t settest_length;
	ptraddr_t settest_base;
	size_t settest_offset;
	size_t settest_perms;
	_Bool settest_tag;


	//NON VALID CAPABILITIES

	//7.get root capability and see if it is valid
	//tag bit cleared - root capability has been zeroed in boot code
	void * rootCap;
	size_t rootCap_length;
	ptraddr_t rootCap_base;
	size_t rootCap_offset;
	size_t rootCap_perms;
	_Bool rootCap_tag;


	//8. point to a fixed memory address to write a character
	// this is NOT a valid capability
	// will cause a zero tag bit
	unsigned char *pointMemPut=(unsigned char *)0x800E0000;
	size_t pointMemPut_length;
	ptraddr_t pointMemPut_base;
	size_t pointMemPut_offset;
	size_t pointMemPut_perms;
	_Bool pointMemPut_tag;

#endif

int using_cherih(void) {

//test cheri specific functions
#ifdef	__CHERI_PURE_CAPABILITY__

	 //VALID CAPABILITIES

	 //1. capability teststr1
	 teststr1_length = (size_t)cheri_length_get((void*)teststr1); //return type is unsigned long int, so typecast to size_t to be sure compatible across architectures
	 teststr1_base = (ptraddr_t)cheri_base_get((void*)teststr1); //return type is unsigned long int
	 teststr1_perms = (size_t)cheri_perms_get((void*)teststr1); //return type is unsigned int
	 teststr1_tag = cheri_tag_get((void*)teststr1); //return type is _bool
	 teststr1_offset = (size_t)cheri_offset_get((void*)teststr1); //return type is unsigned long int
	 printf(".............................................\n");
	 printf("1. capability teststr1\n");
	 printf("teststr1 is: %s\n", teststr1);
	 printf("address of capability teststr1 in memory is: %p\n\n", &teststr1);
	 printf("base address of capability teststr1 is: 0x%lx\n", teststr1_base);
	 printf("length of capability teststr1 in bytes is: %lu\n", teststr1_length);
	 printf("offset from base address of capability teststr1 is: 0x%lx\n", teststr1_offset);
	 printf("value of capability teststr1 is: 0x%lx\n", teststr1_offset+teststr1_base);
	 printf("permissions of capability teststr1 is: 0x%lx\n", teststr1_perms);
	 printf("tag of capability teststr1 is: 0x%01x\n", teststr1_tag);
	 printf(".............................................\n\n\n");

	 //2. capability teststr2
	 const char *teststr2 = teststring;
	 size_t teststr2_length;
	 ptraddr_t teststr2_base;
	 size_t teststr2_offset;
	 size_t teststr2_perms;
	 _Bool teststr2_tag;
	 teststr2_length = (size_t)cheri_length_get((void*)teststr2);
	 teststr2_base = (ptraddr_t)cheri_base_get((void*)teststr2);
	 teststr2_perms = (size_t)cheri_perms_get((void*)teststr2);
	 teststr2_tag = cheri_tag_get((void*)teststr2);
	 teststr2_offset = (size_t)cheri_offset_get((void*)teststr2);
	 printf(".............................................\n");
	 printf("2. capability teststr2\n");
	 printf("teststr2 is: %s\n", teststr2);
	 printf("address of capability teststr2 in memory is: %p\n\n", &teststr2);
	 printf("base address of capability teststr2 is: 0x%lx\n", teststr2_base);
	 printf("length of capability teststr2 in bytes is: %lu\n", teststr2_length);
	 printf("offset from base address of capability teststr2 is: 0x%lx\n", teststr2_offset);
	 printf("value of capability teststr2 is: 0x%lx\n", teststr2_offset+teststr2_base);
	 printf("permissions of capability teststr2 is: 0x%lx\n", teststr2_perms);
	 printf("tag of capability teststr2 is: 0x%01x\n", teststr2_tag);
	 printf(".............................................\n\n\n");

	 //3. capability teststr3
	 //local
	 char teststr3[8] = {'E', 'L', '3', 'S', '\n', '\0'};
	 size_t teststr3_length;
	 ptraddr_t teststr3_base;
	 size_t teststr3_offset;
	 size_t teststr3_perms;
	 _Bool teststr3_tag;
	 teststr3_length = (size_t)cheri_length_get((void*)teststr3);
	 teststr3_base = (ptraddr_t)cheri_base_get((void*)teststr3);
	 teststr3_perms = (size_t)cheri_perms_get((void*)teststr3);
	 teststr3_tag = cheri_tag_get((void*)teststr3);
	 teststr3_offset = (size_t)cheri_offset_get((void*)teststr3);
	 printf(".............................................\n");
	 printf("3. capability teststr3\n");
	 printf("teststr3 is: %s\n", teststr3);
	 printf("capability defined locally and held in register only\n");
	 printf("address of teststr3 character array is: %p\n\n", &teststr3);
	 printf("base address of capability teststr3 is: 0x%lx\n", teststr3_base);
	 printf("length of capability teststr3 in bytes is: %lu\n", teststr3_length);
	 printf("offset from base address of capability teststr3 is: 0x%lx\n", teststr3_offset);
	 printf("value of capability teststr3 is: 0x%lx\n", teststr3_offset+teststr3_base);
	 printf("permissions of capability teststr3 is: 0x%lx\n", teststr3_perms);
	 printf("tag of capability teststr3 is: 0x%01x\n", teststr3_tag);
	 printf(".............................................\n\n\n");

	 //4.capability PCCtest
	 PCCtest = cheri_program_counter_get(); //cheri.h function
	 PCCtest_length = (size_t)cheri_length_get((void*)PCCtest);
	 PCCtest_base = (ptraddr_t)cheri_base_get((void*)PCCtest);
	 PCCtest_perms = (size_t)cheri_perms_get((void*)PCCtest);
	 PCCtest_tag = cheri_tag_get((void*)PCCtest);
	 PCCtest_offset = (size_t)cheri_offset_get((void*)PCCtest);
	 printf(".............................................\n");
	 printf("4. capability PCCtest\n");
	 printf("address of cheri_program_counter_get function: %p\n", PCCtest);
	 printf("address of capability PCCtest in memory is: %p\n", &PCCtest);
	 //bounds of program counter span sections .rodata, .text, .init, .fini, .data.rel.ro, .init_array, __cap_relocs, .got
	 //before .data and .bss
	 printf("\nbase address of capability PCCtest is: 0x%lx\n", PCCtest_base);
	 printf("length of capability PCCtest is: 0x%lx\n", PCCtest_length);
	 printf("offset from base address of capability PCCtest is: 0x%lx\n", PCCtest_offset);
	 printf("value of capability PCCtest is: 0x%lx\n", PCCtest_offset+PCCtest_base);
	 printf("permissions of capability PCCtest is: 0x%lx\n", PCCtest_perms);
	 printf("tag of capability PCCtest is: 0x%01x\n", PCCtest_tag);
	 printf(".............................................\n\n\n");

	 //5.capability numPtr
     //test pointer to array of numbers
	 numPtr_length = (size_t)cheri_length_get((void*)numPtr);
	 numPtr_base = (ptraddr_t)cheri_base_get((void*)numPtr);
	 numPtr_perms = (size_t)cheri_perms_get((void*)numPtr);
	 numPtr_tag = cheri_tag_get((void*)numPtr);
	 numPtr_offset = (size_t)cheri_offset_get((void*)numPtr);
 	 printf(".............................................\n");
 	 printf("5. capability numPtr\n");
 	 printf("address of capability numPtr in memory is: %p\n", &numPtr);
 	 printf("address of integer array in memory is: 0x%08lx\n", (long int)numPtr);
 	 printf("value of integer pointing to is: %i\n", (int)*fixedNumArray);
 	 printf("value of integer pointing to is: %i\n", (int)numPtr[0]);
 	 printf("value of integer pointing to is: %i\n", (int)*numPtr);
 	 printf(".............................................\n");
 	 printf("base address of capability numPtr is: 0x%lx\n", numPtr_base);
 	 printf("length of capability numPtr is: 0x%lx\n", numPtr_length);
 	 printf("offset from base address of capability numPtr is: 0x%lx\n", numPtr_offset);
 	 printf("value of capability numPtr is: 0x%lx\n", numPtr_offset+numPtr_base);
 	 printf("permissions of capability numPtr is: 0x%lx\n", numPtr_perms);
 	 printf("tag of capability numPtr is: 0x%01x\n", numPtr_tag);
 	 printf(".............................................\n");
 	 printf("value of capability numPtr in memory is: %i\n", (int)numPtr[0]);
 	 printf("value of capability numPtr in memory is: %i\n", (int)numPtr[1]);
 	 printf("value of capability numPtr in memory is: %i\n", (int)numPtr[2]);
     //or
 	 printf("value of capability numPtr in memory is: %i\n", (int)numPtr[0]);
 	 printf("value of capability numPtr in memory is: %i\n", (int)*numPtr); //same
 	 numPtr++;
 	 printf("value of capability numPtr in memory is: %i\n", (int)numPtr[0]);
 	 printf("value of capability numPtr in memory is: %i\n", (int)*numPtr); //same
 	 numPtr++;
 	 printf("value of capability numPtr in memory is: %i\n", (int)numPtr[0]);
 	 printf("value of capability numPtr in memory is: %i\n", (int)*numPtr); //same
 	 printf(".............................................\n\n\n");

	 //6. capability settest
	 settest_length = cheri_length_get((void*)settest);
	 settest_base = cheri_base_get((void*)settest);
	 settest_perms = cheri_perms_get((void*)settest);
	 settest_tag = cheri_tag_get((void*)settest);
	 settest_offset = cheri_offset_get((void*)settest);
	 printf(".............................................\n");
	 printf("6. capability settest\n");
	 printf("\nbase address of capability settest is: 0x%lx\n", settest_base);
	 printf("length of capability settest in bytes is: %lu\n", settest_length);
	 printf("offset from base address of capability settest is: 0x%lx\n", settest_offset);
	 printf("value of capability settest is: 0x%lx\n", settest_offset+settest_base);
	 printf("permissions of capability settest is: 0x%lx\n", settest_perms);
	 printf("tag of capability settest is: 0x%01x\n", settest_tag);
 	 printf(".............................................\n");
 	 printf("value of capability settest 0 in memory is: %i\n", (int)settest[0]);
 	 printf("value of capability settest 1 in memory is: %i\n", (int)settest[1]);
 	 printf("value of capability settest 2 in memory is: %i\n", (int)settest[2]);
 	 printf("value of capability settest 3 in memory is: %i\n", (int)settest[3]);
 	 printf("value of capability settest 4 in memory is: %i\n", (int)settest[4]);
 	 printf("value of capability settest 5 in memory is: %i\n", (int)settest[5]);
 	 printf("I can print lots of empty values\n");
 	 printf(".............................................\n");
	 printf("do some changes.............\n");
	 printf(".............................................\n");
	 //MAKE SOME CHANGES!
	 //reduce bounds (4 per int)
	 settest = (int*)cheri_bounds_set((void*)settest,(unsigned long int)12); //12 means 3 values
	 settest_length = cheri_length_get((void*)settest);
	 settest_tag = cheri_tag_get((void*)settest);
	 printf("NEW length of capability settest in bytes is: %lu\n", settest_length);
	 printf("tag of capability settest is: 0x%01x\n", settest_tag);
 	 printf("value of capability settest 0 in memory is: %i\n", (int)settest[0]);
 	 printf("value of capability settest 1 in memory is: %i\n", (int)settest[1]);
 	 printf("value of capability settest 2 in memory is: %i\n", (int)settest[2]);
 	 //comment in the line below to cause a "Terminated by exception"
 	 //printf("value of capability settest in memory is: %i\n", (int)settest[3]);
 	 printf("I can now only print 3 values\n");
 	 printf(".............................................\n");
	 //change offset
	 settest = (int*)cheri_offset_set((void*)settest,(unsigned long int)4);
	 settest_offset = cheri_offset_get((void*)settest);
	 settest_tag = cheri_tag_get((void*)settest);
	 printf("NEW offset from base address of capability settest is: 0x%lx\n", settest_offset);
	 printf("tag of capability settest is: 0x%01x\n", settest_tag);
	 printf("value of capability settest 0 in memory is: %i\n", (int)settest[0]);
 	 printf("value of capability settest 1 in memory is: %i\n", (int)settest[1]);
 	 //comment in the line below to cause a "Terminated by exception"
 	 //printf("value of capability settest 2 in memory is: %i\n", (int)settest[2]);
 	 printf("I can now only print 2 values\n");
 	 printf(".............................................\n\n\n");


 	 //NON VALID CAPABILITIES

 	 //7.get root capability and see if it is valid
	 //tag bit cleared - root capability has been zeroed in boot code
 	 rootCap = cheri_global_data_get();
 	 rootCap_length = (size_t)cheri_length_get((void*)rootCap);
 	 rootCap_base = (ptraddr_t)cheri_base_get((void*)rootCap);
 	 rootCap_perms = (size_t)cheri_perms_get((void*)rootCap);
 	 rootCap_tag = cheri_tag_get((void*)rootCap);
 	 rootCap_offset = (size_t)cheri_offset_get((void*)rootCap);
	 printf(".............................................\n");
	 printf("7. capability rootCap\n");
	 printf("address of capability rootCap in memory is: %p\n", &rootCap);
	 printf("value of capability rootCap in memory is: 0x%08lx\n", (long int)rootCap);
	 printf("\nbase address of capability rootCap is: 0x%lx\n", rootCap_base);
	 printf("length of capability rootCap is: 0x%lx\n", rootCap_length);
	 printf("offset from base address of capability rootCap is: 0x%lx\n", rootCap_offset);
	 printf("value of capability rootCap is: 0x%lx\n", rootCap_offset+rootCap_base);
	 printf("permissions of capability rootCap is: 0x%lx\n", rootCap_perms);
	 printf("tag of capability rootCap is: 0x%01x\n", rootCap_tag);
	 printf("OOPS! TAG BIT NOT VALID!!!\n");
	 printf(".............................................\n\n\n");


	 //8. pointMemPut
	 //tag bit gets cleared - not a valid capability example
	 pointMemPut_length = (size_t)cheri_length_get((void*)pointMemPut);
	 pointMemPut_base = (ptraddr_t)cheri_base_get((void*)pointMemPut);
	 pointMemPut_perms = (size_t)cheri_perms_get((void*)pointMemPut);
	 pointMemPut_tag = cheri_tag_get((void*)pointMemPut);
	 pointMemPut_offset = (size_t)cheri_offset_get((void*)pointMemPut);
	 printf(".............................................\n");
	 printf("8. capability pointMemPut\n");
	 printf("address of capability pointMemPut in memory is: %p\n", &pointMemPut);
	 printf("value of capability pointMemPut in memory is: 0x%08lx\n", (long int)pointMemPut);
	 printf("\nbase address of capability pointMemPut is: 0x%lx\n", pointMemPut_base);
	 printf("length of capability pointMemPut is: 0x%lx\n", pointMemPut_length);
	 printf("offset from base address of capability pointMemPut is: 0x%lx\n", pointMemPut_offset);
	 printf("value of capability pointMemPut is: 0x%lx\n", pointMemPut_offset+pointMemPut_base);
	 printf("permissions of capability pointMemPut is: 0x%lx\n", pointMemPut_perms);
	 printf("tag of capability pointMemPut is: 0x%01x\n", pointMemPut_tag);
	 printf("OOPS! TAG BIT CLEARED!!!\n");
	 printf(".............................................\n\n\n");
	 //try to write a character to the memory location
	 //comment in to cause a program "Terminated by exception"
     //*pointMemPut = 'A';


#endif

	return EXIT_SUCCESS;
}
 
