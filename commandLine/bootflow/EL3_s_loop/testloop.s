// ============================================================================
// Name        : testloop.s
// Author      : CAP-TEE
// Version     :
// Copyright   : CAP-TEE 2021
// Description : test loop
//=============================================================================

  .global main
  .type main, @function
main:

// do some random stuff and sit in a loop
  LOOP:
  MOV      x0, #0x43
  NOP
  NOP
  B LOOP
  RET
