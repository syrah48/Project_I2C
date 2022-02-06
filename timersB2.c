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


extern volatile char IOT_Char_Tx[MEDIUM_RING_SIZE];
volatile char ping_command[26] = "AT+PING=www.google.com,3";
unsigned int ping_count;
unsigned int reboot;
int IOT_connected;
char IOT_ping;
char port_config;

char IOT_port[16] = "AT+NSTCP=1030,1";

extern char command_in_progress;

unsigned int entry_time;
extern char entry_correct;

//IOT - Don't touch
unsigned int current_move_time;
extern unsigned int move_time;

//interrupt functions----------------------------------------------------------

#pragma vector = TimerB2_0CCR_Vector
__interrupt void Timer2_B0_ISR(void){
//------------------------------------------------------------------------------
// TimerB2 0 Interrupt handler
//------------------------------------------------------------------------------
//pinging google every 10 seconds
  ++ping_count;
  
  switch (ping_count) {
  case 10:  //5 seconds
    if (port_config) {
      port_config = OFF;
      
      int i;
      for (i=0; i < 26; ++i) {
        IOT_Char_Tx[i] = IOT_port[i]; //link to port
      }
      start_UCA0transmission();
    }
    break;
  case 20:  //10 seconds
    ping_count = RESET_STATE;
    
    int i;
    for (i=0; i < 26; ++i) {
      IOT_Char_Tx[i] = ping_command[i];
    }
    start_UCA0transmission();
    IOT_ping = ON;
    break;
  }
  
  if (!IOT_connected) {
    ++reboot;
    switch (reboot) { 
    case 118:  //if disconnected for 59 seconds, reboot
        P5OUT &= ~IOT_RESET;
        break;
    case 120: //turn back on after 1 second
        P5OUT |= IOT_RESET;
        reboot = RESET_STATE;
        break;
    default: break;
    }
  }
  else reboot = RESET_STATE; //keep at 0 if connected
 
  TB2CCR0 += TB2CCR0_INTERVAL; // Add Offset to TB2CCR0
//------------------------------------------------------------------------------
}


#pragma vector = TimerB2_1_2_OF_CCR_Vector
__interrupt void TIMER2_B1_ISR(void){
//----------------------------------------------------------------------------
// TimerB2 1-2, Overflow Interrupt Vector (TBIV) handler
//----------------------------------------------------------------------------
switch(__even_in_range(TB2IV,14)){
  case 0: break; // No interrupt

  case 2: //-CCR1------------------------------------------------------------
//determines movement time (increments of 200 msec)
    ++current_move_time;
    if (current_move_time >= move_time) {
      all_wheels_off();
    }
    if (current_move_time >=(move_time + 1) ) {
      move_time = RESET_STATE;
      stop_timerB2_CCR1();
      strcpy(display_line[UPPERMID], "   Idle   ");
      display_changed = ON;
    }
    
    
    TB2CCR1 += TB2CCR1_INTERVAL; // Add Offset to TBCCR1
    break;
//---------------------------------------------------------------------------
  case 4: //-CCR2------------------------------------------------------------
    ++entry_time;
    if (entry_time >= 120) { //6 seconds
      entry_correct = OFF;
      stop_timerB2_CCR2();
    }
    TB2CCR2 += TB2CCR2_INTERVAL; // Add Offset to TBCCR2
    break;
//----------------------------------------------------------------------------
  case 14: // overflow
    break;
default: break;
}
//----------------------------------------------------------------------------
}

//TB2CCR0 = TB2R + interval
//timer always counts from 0 to max regardless of interrupts enabled
//project 5 - off time timer 


//timer B2 initialization-----------------------------------------------------
void Init_Timer_B2(void) {
  TB2CTL = TBSSEL__SMCLK; // SMCLK source
  TB2CTL |= TBCLR; // Resets TB0R, clock divider, count direction
  TB2CTL |= MC__CONTINOUS; // Continuous up
  
  TB2CTL |= ID__8; // Divide clock by 8
  TB2EX0 = TBIDEX__8; // Divide clock by an additional 8
  
//  TB2CCR0 = TB2CCR0_INTERVAL; // CCR0
//  TB2CCTL0 |= CCIE; // CCR0 enable interrupt
  
//  TB2CCR1 = TB2CCR1_INTERVAL; // CCR1
//  TB2CCTL1 |= CCIE; // CCR1 enable interrupt
 
//  TB2CCR2 = TB2CCR2_INTERVAL; // CCR2
//  TB2CCTL2 |= CCIE; // CCR2 enable interrupt
  
//  TB2CTL &= ~TBIFG; // Clear Overflow Interrupt flag
//  TB2CTL |=  TBIE; // enable Overflow Interrupt for IOT RESET
} 

//start timers-----------------------------------------------------------------
void start_timerB2_CCR0(void) {
  TB2CCR0 = TB2R + TB2CCR0_INTERVAL; // CCR0
  TB2CCTL0 &= ~CCIFG; //clear any possible interrupts
  TB2CCTL0 |= CCIE; // CCR0 enable interrupt
}

void start_timerB2_CCR1(void) {
  TB2CCR1 = TB2R + TB2CCR1_INTERVAL; // CCR1
  TB2CCTL1 &= ~CCIFG; //clear any possible interrupts
  TB2CCTL1 |= CCIE; // CCR1 enable interrupt
  current_move_time = RESET_STATE;
  command_in_progress = ON;
}

void start_timerB2_CCR2(void) {
  TB2CCR2 = TB2R + TB2CCR2_INTERVAL; // CCR2
  TB2CCTL2 &= ~CCIFG; //clear any possible interrupts
  TB2CCTL2 |= CCIE; // CCR2 enable interrupt
}

void start_timerB2_OF(void) {
  TB2CTL &= ~TBIFG; // Clear Overflow Interrupt flag 
  TB2CTL |= TBIE; //enable Overflow Interrupt
}

//stop timers------------------------------------------------------------------
void stop_timerB2_CCR0(void) {
  TB2CCTL0 &= ~CCIE; // CCR0 disable interrupt
  TB2CCTL0 &= ~CCIFG; //clear any possible interrupts
}

void stop_timerB2_CCR1(void) {
  TB2CCTL1 &= ~CCIE; // CCR1 disable interrupt
  TB2CCTL1 &= ~CCIFG; //clear any possible interrupts
  current_move_time = RESET_STATE;
  command_in_progress = OFF;
}

void stop_timerB2_CCR2(void) {
  TB2CCTL2 &= ~CCIE; // CCR2 disable interrupt
  TB2CCTL2 &= ~CCIFG; //clear any possible interrupts
}

void stop_timerB2_OF(void) {
  TB2CTL &= ~TBIFG; // Clear Overflow Interrupt flag 
  TB2CTL &= ~TBIE; //enable Overflow Interrupt 
} */
  
  
