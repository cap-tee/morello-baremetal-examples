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
 //enableTimerN - enables the non secure timer
 .global enableTimerS
 //disableTimerN - disables the non secure timer
 .global disableTimerS

 //********************************************
// FUNCTIONS
//*******************************************
//---------------------------------------------
// setTimerTicksS
//---------------------------------------------
  // setTimerTicksS - void setTimerTicks(uint32_t value), the parameter is stored in x0
  // Sets the value of the Secure EL1 Physical Timer Value Register (CNTPS_TVAL_EL1)
  .type setTimerTicksS, "function"
setTimerTicksS:
  MSR     CNTPS_TVAL_EL1, x0 //secure timer, NOTE: different register to the non secure timer
  ISB
  RET
   
//---------------------------------------------
//enableTimerS - enables the secure timer
//---------------------------------------------
  .type enableTimerS, "function"
enableTimerS:
  MOV    x0, #0x1           // Set enable bit, and clear mask bit
  MSR    CNTPS_CTL_EL1, x0  //secure timer,  NOTE: different register to the non secure timer
  ISB
  RET

//---------------------------------------------
//disableTimerS - disables the secure timer
//---------------------------------------------
  .type disableTimerS, "function"
disableTimerS:
  MSR    CNTPS_CTL_EL1, xzr // secure timer, clear the enable bit,  NOTE: different register to the non secure timer
  ISB
  RET

