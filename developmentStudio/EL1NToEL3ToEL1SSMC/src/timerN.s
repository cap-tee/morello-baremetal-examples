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
 //startTimerN - starts the non secure timer
 .global startTimerN
 //stopTimerN - stops the non secure timer
 .global stopTimerN

//********************************************
// FUNCTIONS
//*******************************************
//---------------------------------------------
// setTimerTicksN
//---------------------------------------------
  // setTimerTicksN - void setTimerTicks(uint32_t value), the parameter is stored in x0
  // Sets the value of the non Secure EL1 Physical Timer Value Register (CNTP_TVAL_EL1)
  .type setTimerTicksN, "function"
setTimerTicksN:
  MSR     CNTP_TVAL_EL0, x0 //non secure timer - NOTE: different register to the secure timer
  ISB
  RET

//---------------------------------------------
//startTimerN - start the non secure timer
//---------------------------------------------
  .type startTimerN, "function"
startTimerN:
  MOV    x0, #0x1          //Set Enable bit, and clear Mask bit
  MSR    CNTP_CTL_EL0, x0  //non secure timer NOTE: different register to the secure timer
  ISB
  RET

//---------------------------------------------
//stopTimerN - stops the non secure timer
//---------------------------------------------
  .type stopTimerN, "function"
stopTimerN:
  MSR    CNTP_CTL_EL0, xzr //non secure timer clear enable bit NOTE: different register to the secure timer
  ISB
  RET
