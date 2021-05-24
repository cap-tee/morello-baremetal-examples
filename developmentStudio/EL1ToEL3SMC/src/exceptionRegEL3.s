/*
 ============================================================================
 Name        : exceptionRegEL3.s
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : Functions to manipulate Exception registers at EL3

 ============================================================================
 */

 //****************************************************************************
 // SECTION AND DEFINES
 //****************************************************************************

 // should automatically go into secure section of memory
 .section  .SECUREexceptionRegEL3_ass,"ax"
 .align 3

 // function to read the ESR_EL3 exception register - used by exceptionHandlerFuncsEL3
 .global readESREL3

 //****************************************************************************
 // FUNCTIONS
 //****************************************************************************

 //----------------------------------------------------------------------------
 // readESREL3
 // Description: Get the exception information for EL3 so can obtain the SMC ID
 //----------------------------------------------------------------------------
  .type readESREL3, "function"
readESREL3:
  // Holds syndrome information for an exception taken to EL3.
  // Read this to get SMC ID information
  MRS       x0, ESR_EL3  //Exception Syndrome Register, EL3
  RET

