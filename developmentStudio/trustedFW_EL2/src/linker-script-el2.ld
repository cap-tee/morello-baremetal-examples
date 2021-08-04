/*
 ============================================================================
 Name        : linker-script-el2.ld
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : linker script for el2 stand alone programs
 ============================================================================
 */

 /* use all DRAM0 memory for normal world EL2 */
MEMORY
{
  nonsecure_ram      	(rwx) : ORIGIN = 0x80000000, LENGTH = 0x80000000 
}

STACK_SIZE = 0x20000;
/* place stack, address chosen to match stack address observed when running basic loop code in android stack */
/* start on a 0 for 16 byte alignment */
STACK_START = 0xBDFFFFD0; 

SECTIONS
{
    /* Align to 16 bytes (128 bits)*/
   . = ALIGN(16);
    .rodata : 
    { 
    	*(.rodata*) 
    } > nonsecure_ram
    
   . = ALIGN(16);
    .data : 
    { 
    	*(.data*) 
    } > nonsecure_ram    
   
   . = ALIGN(16);
   .text : 
   { 
	   *(.text*) 
   } > nonsecure_ram
   
   
       /*EL2 code */
        .NONSECUREsection_el2 :
    {
    . = ALIGN(16);
    __NONSECUREsection_el2_start__ = .; 
    *(.NONSECUREel2nmmusection_ass*)
    *(.NONSECUREttel2nsection_ass*)    
    __NONSECUREsection_el2_end__ = .;
    } > nonsecure_ram
   
   . = ALIGN(16);
   __bss_start__ = .;
   .bss :
    {
        *(.bss*)
    } > nonsecure_ram
    __bss_end__ = ALIGN(16);

    /*stack section*/
    . = ALIGN(16);
    .stack_el2n (NOLOAD):
    {
 	    /*. = STACK_START;*/  /*comment this line out and the stack will be placed at the end of the program section*/
        . = ALIGN(16);
        _stack_el2n_start = .;
        . = . + STACK_SIZE;
        . = ALIGN(16);
        _stack_el2n_limit = .;
    } > nonsecure_ram
       
    end = .;
}




