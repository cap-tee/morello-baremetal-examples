/*
 ============================================================================
 Name        : start_el2.ld
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : linker script for image generation, copy to llvm-morello-releases/loader
 ============================================================================
*/

ENTRY(__init)

SECTIONS
{
    .text 0x14200000: { *(.text*) }
    .rodata : { *(.rodata*) }
    .data : { *(.data*) }
}
