/*
 ============================================================================
 Name        : timer.s
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : secure timer functions
 			   The CNTPS_TVAL_EL1 and CNTPS_CTL_EL1 registers are for the secure timer
 ============================================================================
 */


//*****************************************
// SECTION
//*****************************************
  // this needs to go in secure memory
  .section  .SECUREtimer_ass,"ax"
  .align 3

//*****************************************
// DEFINES
//*****************************************

 // sets timer length
 .global setTimerTicks
 //startTimer - enables the non secure timer
 .global startTimer
 //stopTimer - disables the non secure timer
 .global stopTimer

 //********************************************
// FUNCTIONS
//*******************************************
//---------------------------------------------
// setTimerTicks - secure timer
// NOTE: different register to the non secure timer
//---------------------------------------------
  .type setTimerTicks, "function"
setTimerTicks:
  MSR     CNTPS_TVAL_EL1, x0
  ISB
  RET
   
//---------------------------------------------
// startTimer - starts the secure timer
// NOTE: different register to the non secure timer
//---------------------------------------------
  .type startTimer, "function"
startTimer:
  MOV    x0, #0x1
  MSR    CNTPS_CTL_EL1, x0
  ISB
  RET

//---------------------------------------------
// stopTimer - stops the secure timer
// NOTE: different register to the non secure timer
//---------------------------------------------
  .type stopTimer, "function"
stopTimer:
  MSR    CNTPS_CTL_EL1, xzr
  ISB
  RET
