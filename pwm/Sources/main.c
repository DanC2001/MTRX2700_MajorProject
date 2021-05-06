#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include "pwm.h"

char pwm_state; 	// PWM low or high
int high_time;

#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void TOC2_ISR(void)
{
   if (pwm_state == LOW) {
   TC2 = TC2 + high_time; 	// stay high for duty cycle
   TCTL2 = (TCTL2 & ~0x10); 	// set pin 2 to go low on next compare
   pwm_state = HIGH;
   }
   else {
   TC2 = TC2 + (PERIOD - high_time);    // stay low until period ends
   TCTL2 = (TCTL2 | 0x30); 		// set pin 2 to go high on next compare
   pwm_state = LOW;
   }
   TFLG1 = 0x04;	// reset pin 2 flag
}

void main(double period, double duty) {
    
  // PWM signal on pin 2
  // Freq = 1Hz
   
  // initialise variables to calculate duty cycle
  int pot_value, voltage, duty_cycle;
  TCTL2 = 1;
   
  //ATD1CTL5 = 0x87;     // A-D conversion
  
  //pot_value = ATD1DR0;  // potentiometer value
   
  //voltage = pot_value * 5000/1024;   // voltage of potentiometer
   
  //duty_cycle = duty;//(voltage * 13)/66 * 100;   // calculate duty cycle

  disable();
  TSCR1 = 0x80; 		// enable timer
  TSCR2 = 0x00; 		// set prescaler to 1
  TIOS = TIOS | 0x04; 		// set pin 2 as output compare
  TCTL2 = (TCTL2 | 0x30); 	// set pin 2 to go high on compare
  TFLG1 = 0x04; 		// clear channel 2 flag
  TIE = TIE | 0x04; 			// enable interrupt for pin 2
  pwm_state = LOW; 			// start with low PWM
  high_time = duty * period; 	// calculate how long PWM should remain high
  enable();




	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
