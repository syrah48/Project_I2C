//general I2C code
//Chandler James - January 2022

#include <msp430.h>
#include "I2C.h"

extern char i2c_transmit_1;
extern char i2c_transmit_2;
extern volatile char transmit_count;
char i2c_wait;

void init_I2C(void) {
  //--setup B0 I2C
  //set up debugging mechanisms---------
  P1SEL0 &= ~RED_LED;     //RED_LED GPIO operation NOTICE: THE LSB COMES FIRST
  P1SEL1 &= ~RED_LED;     //RED_LED GPIO operation 
  P1OUT  &= ~RED_LED;      //set initial value to 0
  P1DIR  |=  RED_LED;       //direction = output
  //------------------------------------  
  UCB0CTLW0 |= UCSWRST;    //put into SW Reset

  UCB0CTLW0 |= UCSSEL_3;   //choose SMCLK
  
#if DEFAULT_DATA_RATE
  //set prescalar = 10: 1 MHz SMCLK / 10 = 100kHz data rate
  UCB0BRW = 10;          
#else
  //set prescalar = 5000: 1 MHz SMCLK / 5000 = 200 Hz data rate
  UCB0BRW = 5000;	 
#endif

  UCB0CTLW0 |= UCMODE_3;   // put into I2C mode
  UCB0CTLW0 |= UCMST;      //put into Master
  UCB0I2CSA= 0x18;         //slave addr =0x18 (accelerometer)
//  UCB0CTLW0 |= UCTR;             // put into write mode

#if TBCNT_2
  UCB0TBCNT = 2;	   // byte counter = 2
#else
  UCB0TBCNT = 1;
#endif
  
  UCB0CTLW1 |= UCASTP_2;   // auto STOP mode

  //---setup ports

  P1SEL1 &= ~BIT3;        //P1.3 SCL
  P1SEL0 |=  BIT3;          //P1.3 SCL

  P1SEL1 &= ~BIT2;        //P1.2 SDA
  P1SEL0 |=  BIT2;

  PM5CTL0 &= ~LOCKLPM5;	   //turns on I/O

  UCB0CTLW0 &= ~UCSWRST;  //take out of SW Reset

  //--setup IRQs

  UCB0IE |= UCTXIE0;      //TX IRQ
  UCB0IE |= UCRXIE0;	  //Rx IRQ
}

void wait_I2C(void) {
#if MY_WAIT
  while (i2c_wait) {/*unfinished */}
#else
  while((UCB0IFG &  UCSTPIFG) == 0){}  //wait for message to be sent
  UCB0IFG &= ~UCSTPIFG;                //clear the stop flag
#endif
}

void write_I2C(char slave_addr, char reg_addr, char write_data) {
  transmit_count = 0;
  //WRITE the reg address to which we will write and the data
  UCB0I2CSA = slave_addr;        //set slave address
  
  i2c_transmit_1 = reg_addr;     //send reg address we will write to
  
  i2c_transmit_2 = write_data;  
  
  UCB0CTLW0 |= UCTR;             // put into write mode  
  
  UCB0CTLW0 |= UCTXSTT;          // Gen START
  
  wait_I2C();
  
#if !TBCNT_2  
  //WRITE the data to that register
  //leave in write mode
  transmit_count = 0;
  i2c_transmit_1 = write_data;     //send data to specified reg address
  UCB0CTLW0 |= UCTR;             // put into write mode
  UCB0CTLW0 |= UCTXSTT;          // Gen START
  wait_I2C();
#endif

}

void read_I2C(char slave_addr, char reg_addr) {
  /*
  UCB0CTLW0 |= UCSWRST;    //put into SW Reset
  UCB0TBCNT = 1;	   // byte counter = 1
  UCB0CTLW0 &= ~UCSWRST;  //take out of SW Reset
  */
  
  //1. WRITE the reg address from which we will read
  transmit_count = 0;
  
  UCB0I2CSA = slave_addr;         //set slave address
  
  i2c_transmit_1 = reg_addr;       //send reg address we will read from
  
  UCB0CTLW0 |= UCTR;             // put into write mode
  
  UCB0CTLW0 |= UCTXSTT;          // Gen START
  
  wait_I2C();
/* 
  UCB0CTLW0 |= UCSWRST;    //put into SW Reset
  UCB0TBCNT = 1;	   // byte counter = 1
  UCB0CTLW0 &= ~UCSWRST;  //take out of SW Reset
*/  
  //2. switch the write bit to read and send again to receive data
  transmit_count = 0;
  
  UCB0CTLW0 &= ~UCTR;            // put into READ mode
  
  UCB0CTLW0 |= UCTXSTT;          // Gen START
  
  wait_I2C();
}


  
