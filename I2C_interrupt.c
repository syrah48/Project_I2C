//I2C Interrupt Service Routine
//Chandler James - January 2022
#include <msp430.h>
#include "I2C.h"

char i2c_receive;
char i2c_transmit_1;
char i2c_transmit_2;
char transmit_count;

#pragma vector = EUSCI_B0_VECTOR
__interrupt void EUSCI_B0_I2C_ISR(void) {
	switch(UCB0IV){
        case 0x04://---------------------- vector 4: NACKIFG
          //implement error handling if no response from slave
          break;
    	case 0x16://---------------------- vector 22: RXIFG0
          i2c_receive = UCB0RXBUF;  //read data from Rx Buff
          break;
//---------------------------------------------------------- vector 24: TXIFG0
    	case 0x18:
        switch(transmit_count) {
        case 0: 
          UCB0TXBUF = i2c_transmit_1;
          ++transmit_count;
          break;
        case 1:
          UCB0TXBUF = i2c_transmit_2;
          ++transmit_count;
          break;
        }
/*
#if TBCNT_2
             switch the state assuming that interrupt will fire again
            after first message is ACK by the slave 
            transmit_state = WRITE_REG1;
#endif */    
          break;
//----------------------------------------------------------------------------
    	default:
          break;
        }
}
