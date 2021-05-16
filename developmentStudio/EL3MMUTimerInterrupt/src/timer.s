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
  // this needs to go in secure memory
  .section  .SECUREtimer_ass,"ax"
  .align 3

  // setTimerPeriod - void setTimerPeriod(uint32_t value), the parameter is stored in x0
  // Sets the value of the Secure EL1 Physical Timer Value Register (CNTPS_TVAL_EL1)
  .global setTimerTicks
  .type setTimerTicks, "function"
setTimerTicks:
  MSR     CNTPS_TVAL_EL1, x0 //secure timer, NOTE: different register to the non secure timer
  ISB
  RET
   
  //enableTimerS - enables the secure timer
  .global enableTimer
  .type enableTimer, "function"
enableTimer:
  MOV    x0, #0x1            // enable and clear Mask
  MSR    CNTPS_CTL_EL1, x0  //secure timer,  NOTE: different register to the non secure timer
  ISB
  RET

  //disableTimerS - disables the secure timer
  .global disableTimer
  .type disableTimer, "function"
disableTimer:
  MSR    CNTPS_CTL_EL1, xzr // secure timer, clear the enable bit,  NOTE: different register to the non secure timer
  ISB
  RET

