//------------------------------------------------------------------------------
//
//  Description: This file contains TimerB0 initialization and interrupts
//
//
//  Chandler James
//  Sept 2020
//------------------------------------------------------------------------------

//#include  "functions.h"
#include <msp430.h>
//#include <string.h>
#include "I2C.h"
//#include "macros.h"
//#include "ports.h"

extern char acc_ready;

#pragma vector = TimerB0_0CCR_Vector
__interrupt void Timer0_B0_ISR(void){
//------------------------------------------------------------------------------
// TimerB0 0 Interrupt handler - 1 second
  acc_ready = 1;
  stop_timerB0_CCR0();
  P1OUT &= ~RED_LED;
  
//------------------------------------------------------------------------------
}


#pragma vector = TimerB0_1_2_OF_CCR_Vector
__interrupt void TIMER0_B1_ISR(void){
//----------------------------------------------------------------------------
// TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
//----------------------------------------------------------------------------
switch(__even_in_range(TB0IV,14)){
  case 0: break; // No interrupt
  
//CCR1 50 msec----------------------------------------------------------------
  case 2:
    
    TB0CCR1 += TB0CCR1_INTERVAL; // Add Offset to TBCCR1
    break;
//----------------------------------------------------------------------------

//CCR2 500 msec---------------------------------------------------------------    
  case 4:    
    
    TB0CCR2 += TB0CCR2_INTERVAL; // Add Offset to TBCCR2
    break;
//---------------------------------------------------------------------------
    
  case 14: // overflow
    break;
  default: break;
}
//----------------------------------------------------------------------------
}
//TB0CCR0 = TB0R + interval
//timer always counts from 0 to max regardless of interrupts enabled 

//timer B0 initialization-----------------------------------------------------
void Init_Timer_B0(void) {
  TB0CTL = TBSSEL__SMCLK; // SMCLK source
  TB0CTL |= TBCLR; // Resets TB0R, clock divider, count direction
  TB0CTL |= MC__CONTINOUS; // Continuous up
  
  TB0CTL |= ID__8; // Divide clock by 8
  TB0EX0 = TBIDEX__8; // Divide clock by an additional 8
  
//  TB0CCR0 = TB0CCR0_INTERVAL; // CCR0
//  TB0CCTL0 |= CCIE; // CCR0 enable interrupt
  
//  TB0CCR1 = TB0CCR1_INTERVAL; // CCR1
//  TB0CCTL1 |= CCIE; // CCR1 enable interrupt
 
//  TB0CCR2 = TB0CCR2_INTERVAL; // CCR2
//  TB0CCTL2 |= CCIE; // CCR2 enable interrupt
  
  TB0CTL &= ~TBIE; // Disable Overflow Interrupt
  TB0CTL &= ~TBIFG; // Clear Overflow Interrupt flag
} 

//start timers----------------------------------------------------------------
void start_timerB0_CCR0(void) {
  TB0CCR0 = TB0R + TB0CCR0_INTERVAL; // CCR0
  TB0CCTL0 &= ~CCIFG; //clear any possible interrupts
  TB0CCTL0 |= CCIE; // CCR0 enable interrupt
}

void start_timerB0_CCR1(void) {
  TB0CCR1 = TB0R + TB0CCR1_INTERVAL; // CCR1
  TB0CCTL1 &= ~CCIFG; //clear any possible interrupts
  TB0CCTL1 |= CCIE; // CCR1 enable interrupt
}

void start_timerB0_CCR2(void) {
  TB0CCR2 = TB0R + TB0CCR2_INTERVAL; // CCR1
  TB0CCTL2 &= ~CCIFG; //clear any possible interrupts
  TB0CCTL2 |= CCIE; // CCR1 enable interrupt
}

//stop timers-----------------------------------------------------------------
void stop_timerB0_CCR0(void) {
  TB0CCTL0 &= ~CCIE; // CCR0 disable interrupt
  TB0CCTL0 &= ~CCIFG; //clear any possible interrupts
}

void stop_timerB0_CCR1(void) {
  TB0CCTL1 &= ~CCIE; // CCR1 disable interrupt
  TB0CCTL1 &= ~CCIFG; //clear any possible interrupts
}

void stop_timerB0_CCR2(void) {
  TB0CCTL2 &= ~CCIE; // CCR2 disable interrupt
  TB0CCTL2 &= ~CCIFG; //clear any possible interrupts
}

