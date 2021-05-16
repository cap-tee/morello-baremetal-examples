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
  0x80000000 - 1GB secure
  0xC0000000 - 1GB non secure
  Note: to enforce this, the MMU at each EL will also have to be set up
  */
 
MEMORY
{
  secure_ram      	(rwx) : ORIGIN = 0x80000000, LENGTH = 0x40000000 
  nonsecure_ram 	(rwx) : ORIGIN = 0xC0000000, LENGTH = 0x40000000
}

STACK_SIZE = 0x20000;
STACK_START_SECURE = 0xBFFD0000; /* place EL1S stack at top of secure memory*/
STACK_START_NONSECURE = 0xFFFD0000; /* place EL1N stack at top of non secure memory*/

SECTIONS
{
	/*SECURE SECTIONS*/
    .rodata : 
    { 
    	*(.rodata*) 
    } > secure_ram
   
   .text : 
   { 
	   *(.text*) 
   } > secure_ram
   
    /* Align to 8 bytes (64 bits)*/
   . = ALIGN(8);
   __bss_start__ = .;
   .bss :
    {
        *(.bss*)
    } > secure_ram
    __bss_end__ = ALIGN(8);

    /*secure stack section*/
    .stack_el1s (NOLOAD):
    {
 	    /*. = STACK_START_SECURE; */ /*comment this line out and the stack will be placed at the end of the program section*/
        . = ALIGN(8);
        _stack_el1s_start = .;
        . = . + STACK_SIZE;
        . = ALIGN(8);
        _stack_el1s_limit = .;
    } > secure_ram
    
    /*NON-SECURE SECTIONS IF THEY EXIST*/
    
    /*Entry into EL1N assembly*/
    .NONSECUREsection_ass :
    {
    . = ALIGN(8);
    __NONSECUREsection_ass_start__ = .;
    *(.NONSECUREel1entrysection_ass*)
    *(.NONSECUREttdummysection_ass*)
    __NONSECUREsection_ass_end__ = .;
    } > nonsecure_ram
    
    /* c code EL1Nmain function*/
     .NONSECUREsection_c :
  {
    . = ALIGN(8);
    __NONSECUREsection_c_start__ = .;
	*(.NONSECUREsection_c*)
	*(.NONSECUREStringSection_c*)
	*(.NONSECUREflagSection_c*)
    __NONSECUREsection_c_end__ = .;
  } > nonsecure_ram
  
   /*el1nmmu assembly*/
       .NONSECUREsection_mmu :
    {
    . = ALIGN(8);
    __NONSECUREsection_mmu_start__ = .;
    *(.NONSECUREel1nmmusection_ass*)
    *(.NONSECUREttel1nsection_ass*)
    __NONSECUREsection_mmu_end__ = .;
    } > nonsecure_ram
    
    /*non secure uart code*/
     .NONSECUREuartSection_c :
    {
    . = ALIGN(8);
    __NONSECUREuartSection_c_start__ = .;
    *(.NONSECUREuartFuncSection_c*)
    *(.NONSECUREuartStructSection_c*)
    __NONSECUREuartSection_c_end__ = .;
    } > nonsecure_ram
     
     /*non secure exception Handler code*/
     .NONSECUREhandlerSection_c :
    {
    . = ALIGN(8);
    __NONSECUREhandlerSection_c_start__ = .;
    *(.NONSECUREhandlerFuncSection_c*)
    *(.NONSECUREhandlerGlobalSection_c*)
    __NONSECUREhandlerSection_c_end__ = .;
    } > nonsecure_ram    
   
    /*non secure timer code*/
     .NONSECUREtimerSection_c :
    {
    . = ALIGN(8);
    __NONSECUREtimerSection_c_start__ = .;
    *(.NONSECUREtimer_ass*)
    *(.NONSECUREgicsection_ass*)
    __NONSECUREtimerSection_c_end__ = .;
    } > nonsecure_ram   
  
      /*non secure vector table code*/
     .NONSECUREvectorSection_c :
    {
    . = ALIGN(8);
    __NONSECUREvectorSection_c_start__ = .;
    *(.NONSECUREvectortableel1N_ass*)
    __NONSECUREvectorSection_c_end__ = .;
    } > nonsecure_ram    
       
    /* non secure stack */
    .stack_el1n (NOLOAD):
    {
    	/*. = STACK_START_NONSECURE;*/ /*comment this line out and the stack will be placed at the end of the program section*/
        . = ALIGN(8);
        _stack_el1n_start = .;
        . = . + STACK_SIZE;
        . = ALIGN(8);
        _stack_el1n_limit = .;
    } > nonsecure_ram
    
    end = .;
}




