/*
 ============================================================================
 Name        : using_cheriintrinh.c
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2001
 Description : Uses the cheriintrin.h header file to use some of the cheri built-in
  	  	  	  	  compiler functions to inspect capability parameters from c
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef	__CHERI_PURE_CAPABILITY__

	#include <stddef.h>	//includes ptraddr_t def, Note: vaddr_t(as specified in cheri c/c++ doc 2020 version) got renamed to ptraddr_t
	#include <cheriintrin.h>


	//VALID CAPABILITIES

	//1. capability teststr1
    //global pointer, and string, and variables
	char *teststr1_i = "Hello World from EL3\n";
	size_t teststr1_length_i; //cheri c doc says use size_t, so compatible with 32/64 bit architectures
	ptraddr_t teststr1_base_i; //cheri c doc says use vaddr_t
	size_t teststr1_offset_i;//cheri c doc says use size_t
	size_t teststr1_perms_i;//cheri c doc says use size_t, but perms unsigned int, and offset is unsigned long int
	_Bool teststr1_tag_i;//cheri c doc says use _bool, so same as function return type

	//2. capability teststr2
	//global string, but local pointer and variables
	const char teststring_i[27] = {"abcdefghijklmnopqrstuvwxyz"};

	//3. capability teststr3
	//local string, pointer and variables

    //4. capability PCCtest
	// global pointer to program counter cheri function
	int *PCCtest_i;
	size_t PCCtest_length_i;
	ptraddr_t PCCtest_base_i;
	size_t PCCtest_offset_i;
	size_t PCCtest_perms_i;
	_Bool PCCtest_tag_i;

	//5. capability numPtr
	//test pointer to array of numbers
	int fixedNumArray_i[3] = {78, 6, 45};
	int *numPtr_i = fixedNumArray_i;
	size_t numPtr_length_i;
	ptraddr_t numPtr_base_i;
	size_t numPtr_offset_i;
	size_t numPtr_perms_i;
	_Bool numPtr_tag_i;

	//6. capability settest
	int numArray_i[30] = {45,67,89}; //assign size of 30: 30x4bytes=120bytes
	int *settest_i = numArray_i;

	size_t settest_length_i;
	ptraddr_t settest_base_i;
	size_t settest_offset_i;
	size_t settest_perms_i;
	_Bool settest_tag_i;


	//NON VALID CAPABILITIES

	//7.get root capability and see if it is valid
	//tag bit cleared - root capability has been zeroed in boot code
	void * rootCap_i;
	size_t rootCap_length_i;
	ptraddr_t rootCap_base_i;
	size_t rootCap_offset_i;
	size_t rootCap_perms_i;
	_Bool rootCap_tag_i;


	//8. point to a fixed memory address to write a character
	// this is NOT a valid capability
	// will cause a zero tag bit
	unsigned char *pointMemPut_i=(unsigned char *)0x800E0000;
	size_t pointMemPut_length_i;
	ptraddr_t pointMemPut_base_i;
	size_t pointMemPut_offset_i;
	size_t pointMemPut_perms_i;
	_Bool pointMemPut_tag_i;

#endif

int using_cheriintrinh(void) {

//test cheri specific functions
#ifdef	__CHERI_PURE_CAPABILITY__

	 //VALID CAPABILITIES

	 //1. capability teststr1
	 teststr1_length_i = (size_t)cheri_length_get((void*)teststr1_i); //return type is unsigned long int, so typecast to size_t to be sure compatible across architectures
	 teststr1_base_i = (ptraddr_t)cheri_base_get((void*)teststr1_i); //return type is unsigned long int
	 teststr1_perms_i = (size_t)cheri_perms_get((void*)teststr1_i); //return type is unsigned int
	 teststr1_tag_i = cheri_tag_get((void*)teststr1_i); //return type is _bool
	 teststr1_offset_i = (size_t)cheri_offset_get((void*)teststr1_i); //return type is unsigned long int
	 printf(".............................................\n");
	 printf("1. capability teststr1\n");
	 printf("teststr1 is: %s\n", teststr1_i);
	 printf("address of capability teststr1 in memory is: %p\n\n", &teststr1_i);
	 printf("base address of capability teststr1 is: 0x%lx\n", teststr1_base_i);
	 printf("length of capability teststr1 in bytes is: %lu\n", teststr1_length_i);
	 printf("offset from base address of capability teststr1 is: 0x%lx\n", teststr1_offset_i);
	 printf("value of capability teststr1 is: 0x%lx\n", teststr1_offset_i+teststr1_base_i);
	 printf("permissions of capability teststr1 is: 0x%lx\n", teststr1_perms_i);
	 printf("tag of capability teststr1 is: 0x%01x\n", teststr1_tag_i);
	 printf(".............................................\n\n\n");

	 //2. capability teststr2
	 const char *teststr2_i = teststring_i;
	 size_t teststr2_length_i;
	 ptraddr_t teststr2_base_i;
	 size_t teststr2_offset_i;
	 size_t teststr2_perms_i;
	 _Bool teststr2_tag_i;
	 teststr2_length_i = (size_t)cheri_length_get((void*)teststr2_i);
	 teststr2_base_i = (ptraddr_t)cheri_base_get((void*)teststr2_i);
	 teststr2_perms_i = (size_t)cheri_perms_get((void*)teststr2_i);
	 teststr2_tag_i = cheri_tag_get((void*)teststr2_i);
	 teststr2_offset_i = (size_t)cheri_offset_get((void*)teststr2_i);
	 printf(".............................................\n");
	 printf("2. capability teststr2\n");
	 printf("teststr2 is: %s\n", teststr2_i);
	 printf("address of capability teststr2 in memory is: %p\n\n", &teststr2_i);
	 printf("base address of capability teststr2 is: 0x%lx\n", teststr2_base_i);
	 printf("length of capability teststr2 in bytes is: %lu\n", teststr2_length_i);
	 printf("offset from base address of capability teststr2 is: 0x%lx\n", teststr2_offset_i);
	 printf("value of capability teststr2 is: 0x%lx\n", teststr2_offset_i+teststr2_base_i);
	 printf("permissions of capability teststr2 is: 0x%lx\n", teststr2_perms_i);
	 printf("tag of capability teststr2 is: 0x%01x\n", teststr2_tag_i);
	 printf(".............................................\n\n\n");

	 //3. capability teststr3
	 //local
	 char teststr3_i[8] = {'E', 'L', '3', 'S', '\n', '\0'};
	 size_t teststr3_length_i;
	 ptraddr_t teststr3_base_i;
	 size_t teststr3_offset_i;
	 size_t teststr3_perms_i;
	 _Bool teststr3_tag_i;
	 teststr3_length_i = (size_t)cheri_length_get((void*)teststr3_i);
	 teststr3_base_i = (ptraddr_t)cheri_base_get((void*)teststr3_i);
	 teststr3_perms_i = (size_t)cheri_perms_get((void*)teststr3_i);
	 teststr3_tag_i = cheri_tag_get((void*)teststr3_i);
	 teststr3_offset_i = (size_t)cheri_offset_get((void*)teststr3_i);
	 printf(".............................................\n");
	 printf("3. capability teststr3\n");
	 printf("teststr3 is: %s\n", teststr3_i);
	 printf("capability defined locally and held in register only\n");
	 printf("address of teststr3 character array is: %p\n\n", &teststr3_i);
	 printf("base address of capability teststr3 is: 0x%lx\n", teststr3_base_i);
	 printf("length of capability teststr3 in bytes is: %lu\n", teststr3_length_i);
	 printf("offset from base address of capability teststr3 is: 0x%lx\n", teststr3_offset_i);
	 printf("value of capability teststr3 is: 0x%lx\n", teststr3_offset_i+teststr3_base_i);
	 printf("permissions of capability teststr3 is: 0x%lx\n", teststr3_perms_i);
	 printf("tag of capability teststr3 is: 0x%01x\n", teststr3_tag_i);
	 printf(".............................................\n\n\n");

	 //4.capability PCCtest
	 //CHERIINTRIN CHANGE FUNCTION
	 //PCCtest = cheri_program_counter_get(); //cheri.h function
	 PCCtest_i = cheri_pcc_get(); //cheriintrin.h function

	 PCCtest_length_i = (size_t)cheri_length_get((void*)PCCtest_i);
	 PCCtest_base_i = (ptraddr_t)cheri_base_get((void*)PCCtest_i);
	 PCCtest_perms_i = (size_t)cheri_perms_get((void*)PCCtest_i);
	 PCCtest_tag_i = cheri_tag_get((void*)PCCtest_i);
	 PCCtest_offset_i = (size_t)cheri_offset_get((void*)PCCtest_i);
	 printf(".............................................\n");
	 printf("4. capability PCCtest\n");
	 printf("address of cheri_program_counter_get function: %p\n", PCCtest_i);
	 printf("address of capability PCCtest in memory is: %p\n", &PCCtest_i);
	 //bounds of program counter span sections .rodata, .text, .init, .fini, .data.rel.ro, .init_array, __cap_relocs, .got
	 //before .data and .bss
	 printf("\nbase address of capability PCCtest is: 0x%lx\n", PCCtest_base_i);
	 printf("length of capability PCCtest is: 0x%lx\n", PCCtest_length_i);
	 printf("offset from base address of capability PCCtest is: 0x%lx\n", PCCtest_offset_i);
	 printf("value of capability PCCtest is: 0x%lx\n", PCCtest_offset_i+PCCtest_base_i);
	 printf("permissions of capability PCCtest is: 0x%lx\n", PCCtest_perms_i);
	 printf("tag of capability PCCtest is: 0x%01x\n", PCCtest_tag_i);
	 printf(".............................................\n\n\n");

	 //5.capability numPtr
     //test pointer to array of numbers
	 numPtr_length_i = (size_t)cheri_length_get((void*)numPtr_i);
	 numPtr_base_i = (ptraddr_t)cheri_base_get((void*)numPtr_i);
	 numPtr_perms_i = (size_t)cheri_perms_get((void*)numPtr_i);
	 numPtr_tag_i = cheri_tag_get((void*)numPtr_i);
	 numPtr_offset_i = (size_t)cheri_offset_get((void*)numPtr_i);
 	 printf(".............................................\n");
 	 printf("5. capability numPtr\n");
 	 printf("address of capability numPtr in memory is: %p\n", &numPtr_i);
 	 printf("address of integer array in memory is: 0x%08lx\n", (long int)numPtr_i);
 	 printf("value of integer pointing to is: %i\n", (int)*fixedNumArray_i);
 	 printf("value of integer pointing to is: %i\n", (int)numPtr_i[0]);
 	 printf("value of integer pointing to is: %i\n", (int)*numPtr_i);
 	 printf(".............................................\n");
 	 printf("base address of capability numPtr is: 0x%lx\n", numPtr_base_i);
 	 printf("length of capability numPtr is: 0x%lx\n", numPtr_length_i);
 	 printf("offset from base address of capability numPtr is: 0x%lx\n", numPtr_offset_i);
 	 printf("value of capability numPtr is: 0x%lx\n", numPtr_offset_i+numPtr_base_i);
 	 printf("permissions of capability numPtr is: 0x%lx\n", numPtr_perms_i);
 	 printf("tag of capability numPtr is: 0x%01x\n", numPtr_tag_i);
 	 printf(".............................................\n");
 	 printf("value of capability numPtr in memory is: %i\n", (int)numPtr_i[0]);
 	 printf("value of capability numPtr in memory is: %i\n", (int)numPtr_i[1]);
 	 printf("value of capability numPtr in memory is: %i\n", (int)numPtr_i[2]);
     //or
 	 printf("value of capability numPtr in memory is: %i\n", (int)numPtr_i[0]);
 	 printf("value of capability numPtr in memory is: %i\n", (int)*numPtr_i); //same
 	 numPtr_i++;
 	 printf("value of capability numPtr in memory is: %i\n", (int)numPtr_i[0]);
 	 printf("value of capability numPtr in memory is: %i\n", (int)*numPtr_i); //same
 	 numPtr_i++;
 	 printf("value of capability numPtr in memory is: %i\n", (int)numPtr_i[0]);
 	 printf("value of capability numPtr in memory is: %i\n", (int)*numPtr_i); //same
 	 printf(".............................................\n\n\n");

	 //6. capability settest
	 settest_length_i = cheri_length_get((void*)settest_i);
	 settest_base_i = cheri_base_get((void*)settest_i);
	 settest_perms_i = cheri_perms_get((void*)settest_i);
	 settest_tag_i = cheri_tag_get((void*)settest_i);
	 settest_offset_i = cheri_offset_get((void*)settest_i);
	 printf(".............................................\n");
	 printf("6. capability settest\n");
	 printf("\nbase address of capability settest is: 0x%lx\n", settest_base_i);
	 printf("length of capability settest in bytes is: %lu\n", settest_length_i);
	 printf("offset from base address of capability settest is: 0x%lx\n", settest_offset_i);
	 printf("value of capability settest is: 0x%lx\n", settest_offset_i+settest_base_i);
	 printf("permissions of capability settest is: 0x%lx\n", settest_perms_i);
	 printf("tag of capability settest is: 0x%01x\n", settest_tag_i);
 	 printf(".............................................\n");
 	 printf("value of capability settest 0 in memory is: %i\n", (int)settest_i[0]);
 	 printf("value of capability settest 1 in memory is: %i\n", (int)settest_i[1]);
 	 printf("value of capability settest 2 in memory is: %i\n", (int)settest_i[2]);
 	 printf("value of capability settest 3 in memory is: %i\n", (int)settest_i[3]);
 	 printf("value of capability settest 4 in memory is: %i\n", (int)settest_i[4]);
 	 printf("value of capability settest 5 in memory is: %i\n", (int)settest_i[5]);
 	 printf("I can print lots of empty values\n");
 	 printf(".............................................\n");
	 printf("do some changes.............\n");
	 printf(".............................................\n");
	 //MAKE SOME CHANGES!
	 //reduce bounds (4 per int)
	 settest_i = (int*)cheri_bounds_set((void*)settest_i,(unsigned long int)12); //12 means 3 values
	 settest_length_i = cheri_length_get((void*)settest_i);
	 settest_tag_i = cheri_tag_get((void*)settest_i);
	 printf("NEW length of capability settest in bytes is: %lu\n", settest_length_i);
	 printf("tag of capability settest is: 0x%01x\n", settest_tag_i);
 	 printf("value of capability settest 0 in memory is: %i\n", (int)settest_i[0]);
 	 printf("value of capability settest 1 in memory is: %i\n", (int)settest_i[1]);
 	 printf("value of capability settest 2 in memory is: %i\n", (int)settest_i[2]);
 	 //comment in the line below to cause a "Terminated by exception"
 	 //printf("value of capability settest in memory is: %i\n", (int)settest_i[3]);
 	 printf("I can now only print 3 values\n");
 	 printf(".............................................\n");
	 //change offset
	 settest_i = (int*)cheri_offset_set((void*)settest_i,(unsigned long int)4);
	 settest_offset_i = cheri_offset_get((void*)settest_i);
	 settest_tag_i = cheri_tag_get((void*)settest_i);
	 printf("NEW offset from base address of capability settest is: 0x%lx\n", settest_offset_i);
	 printf("tag of capability settest is: 0x%01x\n", settest_tag_i);
	 printf("value of capability settest 0 in memory is: %i\n", (int)settest_i[0]);
 	 printf("value of capability settest 1 in memory is: %i\n", (int)settest_i[1]);
 	 //comment in the line below to cause a "Terminated by exception"
 	 //printf("value of capability settest 2 in memory is: %i\n", (int)settest_i[2]);
 	 printf("I can now only print 2 values\n");
 	 printf(".............................................\n\n\n");


 	 //NON VALID CAPABILITIES

 	 //7.get root capability and see if it is valid
	 //tag bit cleared - root capability has been zeroed in boot code
 	//CHERIINTRIN CHANGE FUNCTION
 	 //rootCap = cheri_global_data_get();
 	 rootCap_i = (void *)cheri_ddc_get();
 	 rootCap_length_i = (size_t)cheri_length_get((void*)rootCap_i);
 	 rootCap_base_i = (ptraddr_t)cheri_base_get((void*)rootCap_i);
 	 rootCap_perms_i = (size_t)cheri_perms_get((void*)rootCap_i);
 	 rootCap_tag_i = cheri_tag_get((void*)rootCap_i);
 	 rootCap_offset_i = (size_t)cheri_offset_get((void*)rootCap_i);
	 printf(".............................................\n");
	 printf("7. capability rootCap\n");
	 printf("address of capability rootCap in memory is: %p\n", &rootCap_i);
	 printf("value of capability rootCap in memory is: 0x%08lx\n", (long int)rootCap_i);
	 printf("\nbase address of capability rootCap is: 0x%lx\n", rootCap_base_i);
	 printf("length of capability rootCap is: 0x%lx\n", rootCap_length_i);
	 printf("offset from base address of capability rootCap is: 0x%lx\n", rootCap_offset_i);
	 printf("value of capability rootCap is: 0x%lx\n", rootCap_offset_i+rootCap_base_i);
	 printf("permissions of capability rootCap is: 0x%lx\n", rootCap_perms_i);
	 printf("tag of capability rootCap is: 0x%01x\n", rootCap_tag_i);
	 printf("OOPS! TAG BIT NOT VALID!!!\n");
	 printf(".............................................\n\n\n");


	 //8. pointMemPut
	 //tag bit gets cleared - not a valid capability example
	 pointMemPut_length_i = (size_t)cheri_length_get((void*)pointMemPut_i);
	 pointMemPut_base_i = (ptraddr_t)cheri_base_get((void*)pointMemPut_i);
	 pointMemPut_perms_i = (size_t)cheri_perms_get((void*)pointMemPut_i);
	 pointMemPut_tag_i = cheri_tag_get((void*)pointMemPut_i);
	 pointMemPut_offset_i = (size_t)cheri_offset_get((void*)pointMemPut_i);
	 printf(".............................................\n");
	 printf("8. capability pointMemPut\n");
	 printf("address of capability pointMemPut in memory is: %p\n", &pointMemPut_i);
	 printf("value of capability pointMemPut in memory is: 0x%08lx\n", (long int)pointMemPut_i);
	 printf("\nbase address of capability pointMemPut is: 0x%lx\n", pointMemPut_base_i);
	 printf("length of capability pointMemPut is: 0x%lx\n", pointMemPut_length_i);
	 printf("offset from base address of capability pointMemPut is: 0x%lx\n", pointMemPut_offset_i);
	 printf("value of capability pointMemPut is: 0x%lx\n", pointMemPut_offset_i+pointMemPut_base_i);
	 printf("permissions of capability pointMemPut is: 0x%lx\n", pointMemPut_perms_i);
	 printf("tag of capability pointMemPut is: 0x%01x\n", pointMemPut_tag_i);
	 printf("OOPS! TAG BIT CLEARED!!!\n");
	 printf(".............................................\n\n\n");
	 //try to write a character to the memory location
	 //comment in to cause a program "Terminated by exception"
     //*pointMemPut = 'A';


#endif

	return EXIT_SUCCESS;
}

