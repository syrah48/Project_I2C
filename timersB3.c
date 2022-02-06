//------------------------------------------------------------------------------
//
//  Description: This file contains TimerB3 initialization and interrupts
//
//
//  Chandler James
//  Sept 2020
/*/------------------------------------------------------------------------------

#include  "functions.h"
#include  "msp430.h"
#include "macros.h"
#include "ports.h"




void Init_Timer_B3(void) {
//------------------------------------------------------------------------------
// SMCLK source, up count mode, PWM Right Side
// TB3.1 P6.0 R_FORWARD
// TB3.2 P6.1 L_FORWARD
// TB3.3 P6.2 R_REVERSE
// TB3.4 P6.3 L_REVERSE
// TB3.5 P6.4 LCD_BACKLITE
//------------------------------------------------------------------------------
 TB3CTL = TBSSEL__SMCLK; // SMCLK
 TB3CTL |= MC__UP; // Up Mode
 TB3CTL |= TBCLR; // Clear TAR
 
 TB3CCR0 = WHEEL_PERIOD; // PWM Period
 
 TB3CCTL1 = OUTMOD_7; // CCR1 reset/set
 RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM duty cycle
 
 TB3CCTL2 = OUTMOD_7; // CCR2 reset/set
 LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Left Forward PWM duty cycle
 
 TB3CCTL3 = OUTMOD_7; // CCR3 reset/set
 RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM duty cycle
 
 TB3CCTL4 = OUTMOD_7; // CCR4 reset/set
 LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Left Reverse PWM duty cycle
 
 TB3CCTL5 = OUTMOD_7;
 LCD_BACKLITE_BRIGHTNESS = DIM;
//------------------------------------------------------------------------------
} */
