#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <math.h>

void main(void) {

  // pwm ch5 (clock A) and ch7 (clock B)

  int period, dutyCycle, dcValue ;
  int periodValue;
  int polarity;
  
  polarity = 1;       // 1 or 0 to start high or low respectively
  period = 20;        //microseconds
  dutyCycle = 60;     // percent
  
  // set up PWM
  PWMCLK = 0;           // clock A as source clock
  PWMPOL = polarity;    // output start at high level
  PWMCTL = 0x11;        // 8-bit PWM5
  PWMCAE = 1;           // PWM5 centre aligned
  PWMPRCLK = 1;         // prescaler set to 2
  
  
  // calculate values for period and duty cycle values given
  
  periodValue = period * (24/(2 * 2));  // calculate period value from prescaler and period
  
  
  // calculate duty cycle value from duty cycle, polarity and period value
  if (polarity = 1) {
    
    dcValue = (dutyCycle * 0.01) * (periodValue);   // wtf
  }
  else if (polarity = 0) {
    
    dcValue = periodValue - ((dutyCycle * 0.01) * periodValue);
  }
  
    
  // set the period and duty cycle values
  PWMPER5 = periodValue;    
  PWMDTY5 = dcValue;
  
  PWME = PWME | 0x20; // enable PWM channel 5
  
  
	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
