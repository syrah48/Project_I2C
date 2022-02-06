//------------------------------------------------------------------------------
//
//  Description: This file contains TimerB1 initialization and interrupts
//
//
//  Chandler James
//  Sept 2020
/*/------------------------------------------------------------------------------

#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"
#include "ports.h"


extern volatile unsigned char update_display;
extern volatile unsigned char display_changed;
extern char display_line[ROWS][COLUMNS];
extern char display_timer;

//char display_count;
char SW1_debounce;
char SW2_debounce;
unsigned int real_time;

char turn_center = OFF;

//interrupt functions----------------------------------------------------------

#pragma vector = TimerB1_0CCR_Vector
__interrupt void Timer1_B0_ISR(void){
//------------------------------------------------------------------------------
// TimerB1 0 Interrupt handler
//------------------------------------------------------------------------------
  update_display = ON; //update display every 250 msec
  
  if (display_timer) ++real_time;
  else real_time = RESET_STATE;
  
  TB1CCR0 += TB1CCR0_INTERVAL; // Add Offset to TBCCR0
//------------------------------------------------------------------------------
}


#pragma vector = TimerB1_1_2_OF_CCR_Vector
__interrupt void TIMER1_B1_ISR(void){
//----------------------------------------------------------------------------
// TimerB1 1-2, Overflow Interrupt Vector (TBIV) handler
//----------------------------------------------------------------------------
switch(__even_in_range(TB1IV,14)){
  case 0: break; // No interrupt

  case 2: //-CCR1------------------------------------------------------------
//------------------SW1 1 second debounce------------------------------------
    ++SW1_debounce;
    
    switch (SW1_debounce) {
      case 2:
        SW1_debounce = RESET_STATE;
        P4IE |=  SW1; // SW1 interrupt enabled
        P4IFG &= ~SW1; // IFG SW1 cleared
        stop_timerB1_CCR1();
        break;
      default: break;
      }
    
    TB1CCR1 += TB1CCR1_INTERVAL; // Add Offset to TBCCR1
    break;
//---------------------------------------------------------------------------
  case 4: //-CCR2------------------------------------------------------------
//-----------------SW2 1 second debounce-------------------------------------
    ++SW2_debounce;
    
    switch (SW2_debounce) {
      case 2:
        SW2_debounce = RESET_STATE;
        P2IE |= SW2; // SW2 interrupt enabled
        P2IFG &= ~SW2; // IFG SW2 cleared
        stop_timerB1_CCR2();
        break;     
      default: break;
      }

    TB1CCR2 += TB1CCR2_INTERVAL; // Add Offset to TBCCR2
    break;
//----------------------------------------------------------------------------
  case 14: // overflow
    P5OUT  |= IOT_RESET;         //change to high after about 500 msec
    stop_timerB1_OF();           //turn off overflow interrupt
    start_timerB2_CCR0();       //start ping timer
    break;
default: break;
}
//----------------------------------------------------------------------------
}

//TB1CCR0 = TB1R + interval
//timer always counts from 0 to max regardless of interrupts enabled
//project 5 - off time timer 


//timer B1 initialization-----------------------------------------------------
void Init_Timer_B1(void) {
  TB1CTL = TBSSEL__SMCLK; // SMCLK source
  TB1CTL |= TBCLR; // Resets TB0R, clock divider, count direction
  TB1CTL |= MC__CONTINOUS; // Continuous up
  
  TB1CTL |= ID__8; // Divide clock by 8
  TB1EX0 = TBIDEX__8; // Divide clock by an additional 8
  
  TB1CCR0 = TB1CCR0_INTERVAL; // CCR0
  TB1CCTL0 |= CCIE; // CCR0 enable interrupt
  
//  TB1CCR1 = TB1CCR1_INTERVAL; // CCR1
//  TB1CCTL1 |= CCIE; // CCR1 enable interrupt
 
//  TB1CCR2 = TB1CCR2_INTERVAL; // CCR2
//  TB1CCTL2 |= CCIE; // CCR2 enable interrupt
  
  TB1CTL &= ~TBIFG; // Clear Overflow Interrupt flag
  TB1CTL |=  TBIE; // enable Overflow Interrupt for IOT RESET
} 

//start timers-----------------------------------------------------------------
void start_timerB1_CCR0(void) {
  TB1CCR0 = TB1R + TB1CCR0_INTERVAL; // CCR0
  TB1CCTL0 &= ~CCIFG; //clear any possible interrupts
  TB1CCTL0 |= CCIE; // CCR0 enable interrupt
}

void start_timerB1_CCR1(void) {
  TB1CCR1 = TB1R + TB1CCR1_INTERVAL; // CCR1
  TB1CCTL1 &= ~CCIFG; //clear any possible interrupts
  TB1CCTL1 |= CCIE; // CCR1 enable interrupt
}

void start_timerB1_CCR2(void) {
  TB1CCR2 = TB1R + TB1CCR2_INTERVAL; // CCR2
  TB1CCTL2 &= ~CCIFG; //clear any possible interrupts
  TB1CCTL2 |= CCIE; // CCR2 enable interrupt
}

void start_timerB1_OF(void) {
  TB1CTL &= ~TBIFG; // Clear Overflow Interrupt flag 
  TB1CTL |= TBIE; //enable Overflow Interrupt
}

//stop timers------------------------------------------------------------------
void stop_timerB1_CCR0(void) {
  TB1CCTL0 &= ~CCIE; // CCR0 disable interrupt
  TB1CCTL0 &= ~CCIFG; //clear any possible interrupts
}

void stop_timerB1_CCR1(void) {
  TB1CCTL1 &= ~CCIE; // CCR1 disable interrupt
  TB1CCTL1 &= ~CCIFG; //clear any possible interrupts
}

void stop_timerB1_CCR2(void) {
  TB1CCTL2 &= ~CCIE; // CCR2 disable interrupt
  TB1CCTL2 &= ~CCIFG; //clear any possible interrupts
}

void stop_timerB1_OF(void) {
  TB1CTL &= ~TBIFG; // Clear Overflow Interrupt flag 
  TB1CTL &= ~TBIE; //enable Overflow Interrupt 
} */
