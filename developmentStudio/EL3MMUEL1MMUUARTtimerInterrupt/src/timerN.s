/*
 ============================================================================
 Name        : timerN.s
 Author      : CAP-TEE
 Version     :
 Copyright   : CAP-TEE 2021
 Description : non secure timer functions
 			   The CNTP_TVAL_EL0 and CNTP_CTL_EL0 registers are for the non secure physical timer
 ============================================================================
 */

//*****************************************
// SECTION
//*****************************************
  // this needs to go in non secure memory by the linker script
  .section  .NONSECUREtimer_ass,"ax"
  .align 3

//*****************************************
// DEFINES
//*****************************************

 // sets timer length
 .global setTimerTicksN
 //startTimer - enables the non secure timer
 .global startTimerN
 //stopTimer - disables the non secure timer
 .global stopTimerN

//********************************************
// FUNCTIONS
//*******************************************
//---------------------------------------------
// setTimerTicksN - non secure timer
// NOTE: different register to the secure timer
//---------------------------------------------
  .type setTimerTicksN, "function"
setTimerTicksN:
  MSR     CNTP_TVAL_EL0, x0
  ISB
  RET

//---------------------------------------------
// startTimerN - starts the non secure timer
// NOTE: different register to the secure timer
//---------------------------------------------
  .type startTimerN, "function"
startTimerN:
  MOV    x0, #0x1
  MSR    CNTP_CTL_EL0, x0
  ISB
  RET

//---------------------------------------------
// stopTimerN - stops the non secure timer
// NOTE: different register to the secure timer
//---------------------------------------------
  .type stopTimerN, "function"
stopTimerN:
  MSR    CNTP_CTL_EL0, xzr
  ISB
  RET
