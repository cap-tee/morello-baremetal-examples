/*
 ============================================================================
 Name        : timerS.s
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
 .global setTimerTicksS
 //startTimer - enables the non secure timer
 .global startTimerS
 //stopTimer - disables the non secure timer
 .global stopTimerS

 //********************************************
// FUNCTIONS
//*******************************************
//---------------------------------------------
// setTimerTicksS - secure timer
// NOTE: different register to the non secure timer
//---------------------------------------------
  .type setTimerTicksS, "function"
setTimerTicksS:
  MSR     CNTPS_TVAL_EL1, x0
  ISB
  RET
   
//---------------------------------------------
// startTimerS - starts the secure timer
// NOTE: different register to the non secure timer
//---------------------------------------------
  .type startTimerS, "function"
startTimerS:
  MOV    x0, #0x1
  MSR    CNTPS_CTL_EL1, x0
  ISB
  RET

//---------------------------------------------
// stopTimerS - stops the secure timer
// NOTE: different register to the non secure timer
//---------------------------------------------
  .type stopTimerS, "function"
stopTimerS:
  MSR    CNTPS_CTL_EL1, xzr
  ISB
  RET
