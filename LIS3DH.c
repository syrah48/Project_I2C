//I2C code for the LIS3DH accelerometer
//Chandler James - January 2022

#include <msp430.h>
#include "I2C.h"

extern volatile char i2c_receive;
char acc_status;
char acc_ready;
int acc_x;
int acc_y;
int acc_z;

void read_lis3dh(void) {
  //only read from the accelerometer when the timer sets this high
#if USE_TIMER
  if (!acc_ready) return;
  acc_ready = 0;
#endif
  
  // Poll status register to see if data is ready ----------------------------
  read_I2C(ACC_SLAVE, ACC_STATUS_ADDR);
  acc_status = i2c_receive;           // Receive status reg data
  
  //ZYXDA = bit 3 in status reg.  If '1', it means that new data is available
  if (acc_status & ACC_DATA_READY) {
    acc_status = 0;  //clear status to ensure accelerometer sets it again
    
    read_I2C(ACC_SLAVE, XL_ADDR);   //least significant byte
    acc_x = 0x0000 | i2c_receive;  
    
    read_I2C(ACC_SLAVE, XH_ADDR);  //most significant byte
    acc_x |= (i2c_receive << 8);
    
    read_I2C(ACC_SLAVE, YL_ADDR);  //least significant byte
    acc_y = 0x0000 | i2c_receive;
    
    read_I2C(ACC_SLAVE, YH_ADDR);  //most significant byte
    acc_y |= (i2c_receive << 8);
    
    read_I2C(ACC_SLAVE, ZL_ADDR);  //least significant byte
    acc_z = 0x0000 | i2c_receive;
    
    read_I2C(ACC_SLAVE, ZH_ADDR);  //most significant byte
    acc_z |= (i2c_receive << 8);
  }
#if USE_TIMER
  start_timerB0_CCR0();
  P1OUT |= RED_LED;
#endif
}

void init_lis3dh(void) { //initialize accelerometer to normal mode
  /*------------------------------------------------------------------------
  if TBCNT = 2, then my assumption is that the next transmit value will
  have to be set from the interrupt, so the state change can be found there.
  I assume this because the stop bit won't be sent until after the second
  byte is sent.  This means that the stop flag we are waiting for in wait_I2C
  will not be raised until after two bytes of information are sent.
  
  if TBCNT = 1, we will set the next message here like normal.  The stop
  bit will be sent after one byte is transmitted, which will clear the
  wait status and continue this function.
  -------------------------------------------------------------------------*/
  
  write_I2C(ACC_SLAVE, ACC_REG1_ADDR, INIT_REG1); //set CTRL REG1

#if SET_CTRL_REG4 //if default value works, then no need to set it.
  write_I2C(ACC_SLAVE, ACC_REG4_ADDR, INIT_REG4);   //set CTRL REG4
#endif
  
  write_I2C(ACC_SLAVE, ACC_REG5_ADDR, INIT_REG5);  //set CTRl REG5
  write_I2C(ACC_SLAVE, ACC_FIFO_ADDR, INIT_FIFO);  //set FIFO buffer ctrl reg
  
#if TBCNT_2
  //change TBCNT to 1 for read mode
  UCB0CTLW0 |= UCSWRST;    //put into SW Reset
  UCB0TBCNT = 1;	   // byte counter = 1
  UCB0CTLW0 &= ~UCSWRST;  //take out of SW Reset
#endif
  
  acc_ready = 1;  //allow program to begin reading from accelerometer
}
