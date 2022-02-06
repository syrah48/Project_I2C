#include <msp430.h>
#include "I2C.h"

//main function for motor health monitor
//Chandler James - January 2022


//char dataArray[32] = "";
extern char acc_ready;
int runtime = 0;  //test var: how many times through the main loop


void main(void){  
  WDTCTL =WDTPW | WDTHOLD;
  
  //Init_Clocks();
  init_I2C();
  init_lis3dh();
 
  
#if USE_TIMER 
  Init_Timer_B0();
#endif
  
  while(1){
#if ACC_DEBUG
  //Debug state - can modified as needed
  if (acc_ready) {
    acc_ready = 0;
    write_I2C(ACC_SLAVE, ACC_REG1_ADDR, INIT_REG1);
    start_timerB0_CCR0();
    P1OUT |= RED_LED;
  }
#else
  //Standard operation
  read_lis3dh();
#endif
    ++runtime;
  }
}








