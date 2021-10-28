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

/*stack size can be set to 0x1FFFC for SCBNDS instruction, but limit will be set to 0xC0030000 if base is C0010000, even though pointer to C002FFF0 start*/
/*stack size can be set to 0x20000 for SCBNDSE instruction of `EXACT` bounds setting, and limit will be set to 0xC0030000 if base is C0010000, and pointer to C0030000 start*/
STACK_SIZE = 0x20000;
STACK_START_SECURE = 0xBFFD0000; /* place EL1S stack at top of secure memory*/
STACK_START_NONSECURE = 0xFFFD0000; /* place EL1N stack at top of non secure memory*/
/*For setting EL1S/EL1N root bounds*/
_secure_ram_ORIGIN = 0x80000000;
_secure_ram_LENGTH = 0x40000000;
_nonsecure_ram_ORIGIN = 0xC0000000;
_nonsecure_ram_LENGTH = 0x40000000;


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
    /* align to 0x10000 (2^16 bits) boundary because of stack size*/
    /* and setting bounds of stack capability with `EXACT` due to bounds compression */
    /* If you do not align, the tag bit gets cleared when setting the bounds */    
    .stack_el1s (NOLOAD): ALIGN(65536)
    {
 	    /*. = STACK_START_SECURE; */ /*comment this line out and the stack will be placed at the end of the program section*/
        _stack_el1s_limit = .;
        . = . + STACK_SIZE;
        _stack_el1s_base = .;
    } > secure_ram
    
    /*NON-SECURE SECTIONS IF THEY EXIST*/
    
    /*el1 read only data section*/
    
    NONSECUREsection_el1_rodata :
    {
    . = ALIGN(16);
    __NONSECUREsection_el1_rodata_start__ = .;
    *(.el1_rodata*)
    __NONSECUREsection_el1_rodata_end__ = .;
    } > nonsecure_ram
    
    /*Entry into EL1N assembly*/
    NONSECUREsection_ass :
    {
    . = ALIGN(16);
    __NONSECUREsection_ass_start__ = .;
    *(.NONSECUREel1entrysection_ass*)
    *(TT_DUMMYN*)
    __NONSECUREsection_ass_end__ = .;
    } > nonsecure_ram
    
    /* c code EL1Nmain function*/
     .NONSECUREsection_c :
  {
    . = ALIGN(16);
    __NONSECUREsection_c_start__ = .;
	*(.NONSECUREsection_c*)
    __NONSECUREsection_c_end__ = .;
  } > nonsecure_ram
  
   /*el1nmmu assembly*/
       NONSECUREsection_mmu :
    {
    . = ALIGN(16);
    __NONSECUREsection_mmu_start__ = .;
    *(.NONSECUREel1nmmusection_ass*)
    *(.NONSECUREttel1nsection_ass*)
    __NONSECUREsection_mmu_end__ = .;
    } > nonsecure_ram
       
    /* non secure stack */
    /* align to 0x10000 (2^16 bits) boundary because of stack size*/
    /* and setting bounds of stack capability due to bounds compression */
    /* If you do not align, the tag bit gets cleared when setting the bounds */
    .stack_el1n (NOLOAD): ALIGN(65536)
    {
    	/*. = STACK_START_NONSECURE;*/ /*comment this line out and the stack will be placed at the end of the program section*/
        _stack_el1n_limit = .;
        . = . + STACK_SIZE;
        _stack_el1n_base = .;
    } > nonsecure_ram
    
    end = .;
     __end__ = .; /* added for Morello-purecap -doesn't compile without this*/
}




