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
    
    
    end = .;
     __end__ = .; /* added for Morello-purecap -doesn't compile without this*/
}




