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
  // this needs to go in non secure memory by the linker script
  .section  .NONSECUREtimer_ass,"ax"
  .align 3

  // setTimerTicksN - void setTimerTicks(uint32_t value), the parameter is stored in x0
  // Sets the value of the non Secure EL1 Physical Timer Value Register (CNTP_TVAL_EL1)
  .global setTimerTicksN
  .type setTimerTicksN, "function"
setTimerTicksN:
  MSR     CNTP_TVAL_EL0, x0 //non secure timer - NOTE: different register to the secure timer
  ISB
  RET
   
  //enableTimerN - enables the non secure timer
  .global enableTimerN
  .type enableTimerN, "function"
enableTimerN:
  MOV    x0, #0x1          //Set Enable bit, and clear Mask bit
  MSR    CNTP_CTL_EL0, x0  //non secure timer NOTE: different register to the secure timer
  ISB
  RET

  //disableTimerN - disables the non secure timer
  .global disableTimerN
  .type disableTimerN, "function"
disableTimerN:
  MSR    CNTP_CTL_EL0, xzr //non secure timer clear enable bit NOTE: different register to the secure timer
  ISB
  RET
