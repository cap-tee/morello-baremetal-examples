/*
 ============================================================================
 Name        : linker-script.ld
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : linker script to define secure and non-secure memory regions
               and set up regions for the stack
 ============================================================================
 */
 
 
  /* 
  Set up memory regions and properties
  Attributes are writable (w), readable (r), or executable (x). 
  Flash memory is usually (rx), while ram is (rwx). 
  NOTE: Marking a region as non-writable does not make it write protected,
  attributes here describe the properties of the memory, not set it. 
  
  DRAM0 - 2GB is split into the following regions
  Note: to enforce this, the MMU at each EL will also have to be set up
  */
 
MEMORY
{

 lower_dram0_bot (rwx) : ORIGIN = 0x80000000, LENGTH = 0x10000000
 lower_dram0_top (rwx) : ORIGIN = 0x90000000, LENGTH = 0x30000000
 upper_dram0_all (rwx) : ORIGIN = 0xC0000000, LENGTH = 0x40000000
 
}

STACK_SIZE = 0x20000;
STACK_START_SECURE = 0xBFFD0000; /* place EL1S stack at top of secure memory*/
STACK_START_NONSECURE = 0xFFFD0000; /* place EL1N stack at top of non secure memory*/

SECTIONS
{

	/* ---------lower_dram0_bot-------------------------------*/
    .rodata : 
    { 
    	*(.rodata*) 
    } > lower_dram0_bot
   
   /* el3 program code */
   
   .text : 
   { 
	   *(.text*) 
   } > lower_dram0_bot
   
    /* Align to 8 bytes (64 bits)*/
   . = ALIGN(8);
   __bss_start__ = .;
   .bss :
    {
        *(.bss*)
    } > lower_dram0_bot
    __bss_end__ = ALIGN(8);
    
    /* ------------lower_dram0_top----------------------------*/
   
    /* el2 program code */
    
    .NONSECUREsection_el2 :
    {
    . = ALIGN(8);
    __NONSECUREsection_el2_start__ = .;
    *(.NONSECUREel2entrysection_ass_el2*)  
    *(.NONSECUREsection_c_el2*)  
    *(.NONSECUREel2nmmusection_ass_el2*)
    *(.NONSECUREttel2nsection_ass_el2*) 
    *(.NONSECURE_stg1ForEl1nmmusection_ass_el2*)
    *(.NONSECURE_stg2ForEl1nmmusection_ass_el2*)
    *(.NONSECUREvectortableel2N_ass_el2*)
    *(.NONSECUREhandlerFuncSectionEL2_c_el2*)
    *(.NONSECUREeretEL1Nsection_ass_el2*) 
    *(.NONSECUREuartFuncSection_c_el2*)
    *(.NONSECUREuartStructSection_c_el2*)  
    __NONSECUREsection_el2_end__ = .;
    } > lower_dram0_top
   
   
    /* EL1 translation tables in shared el1-RO/el2-RW memory */
     .NONSECUREel1ttSection :
    {
     . = 0xA00F0000;
     __tt_start__ = .;
     *(.NONSECUREttstg1ForEl1nsection_ass_el2*)
     *(.NONSECUREttstg2ForEl1nsection_ass_el2*)
     __tt_end__ = .;
    } > lower_dram0_top
    
     /* EL1 vector tables in shared memory, and handler funcs */
     .NONSECUREel1vtSection :
    {
     . = 0xA00F2000;
     __vt_start__ = .;
     *(.NONSECUREvectortableel1N_ass_el2*)
     *(.NONSECUREhandlerFuncSectionEL1_c_el2*)
     __vt_end__ = .;
    } > lower_dram0_top
    
    
    /* non secure stack */
    
    .stack_el2n (NOLOAD):
    {
    	/* align stack to 16 bytes */
        . = ALIGN(16);
        _stack_el2n_start = .;
        . = . + STACK_SIZE;
        . = ALIGN(8);
        _stack_el2n_limit = .;
    } > lower_dram0_top 
    
     /* --------------upper_dram0_all--------------------------*/
    
    /* el1n program code */
    
   .NONSECUREsection_el1n :
    {
    . = ALIGN(8);
    __NONSECUREsection_el1n_start__ = .;
    *(.NONSECUREel1entrysection_ass_el1*)
    *(.NONSECUREel1ntestsection_ass_el1*)
    *(.NONSECUREsection_c_el1*)
	*(.NONSECUREStringSection_c_el1*)
	*(.NONSECUREuartFuncSection_c*)
    *(.NONSECUREuartStructSection_c*)	
    __NONSECUREsection_el1n_end__ = .;
    } > upper_dram0_all
    
            
    /* non secure stack */
    
    .stack_el1n (NOLOAD):
    {
    	/*. = STACK_START_NONSECURE;*/ /*comment this line out and the stack will be placed at the end of the program section*/
        . = ALIGN(8);
        _stack_el1n_start = .;
        . = . + STACK_SIZE;
        . = ALIGN(8);
        _stack_el1n_limit = .;
    } > upper_dram0_all
    
    end = .;
}

	 /* ----------------------------------------------------*/

