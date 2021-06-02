#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <math.h>

void main(void) {

  // pwm ch5 (clock A) and ch7 (clock B)

  int period, dutyCycle, dcValue ;
  int output_period;
  int polarity;
  //float clock_period;
  
  
  polarity = 1;       // 1 or 0 to start high or low respectively
  period = 1000;        //microseconds
  dutyCycle = 50;     // percent
  
  // set up PWM
  PWMCLK = 0;           // clock A as source clock
  PWMPOL = polarity;    // output start at high level
  PWMCTL = 0x11;        // 8-bit PWM5
  PWMCAE = 1;           // PWM5 centre aligned
  PWMPRCLK = 1;         // prescaler set to 2
  
  output_period = period;
  
  
  // calculate values for period and duty cycle values given
  
  //periodValue = period * (24/(2 * 2));  // calculate period value from prescaler and period
  
  
  // calculate duty cycle value from duty cycle, polarity and period value
  if (polarity == 1) {
    
    dcValue = (dutyCycle * 0.01) * (period);   // wtf
   // dcValue = (dutyCycle/periodValue)*0.01;
  }
  else if (polarity == 0) {
    
    dcValue = period - ((dutyCycle * 0.01) * period);
    //dcValue = (periodValue-dutyCycle)/periodValue*0.01;
  }
  
    
  // set the period and duty cycle values
  PWMPER5 = output_period;    
  PWMDTY5 = dcValue;
  
  PWME = PWME | 0x20; // enable PWM channel 5
  
  //PWMPER5 = 1500;
  
  
  
	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
