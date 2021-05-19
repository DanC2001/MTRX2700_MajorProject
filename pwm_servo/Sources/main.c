#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <math.h>


void main(void) {
  // pwm ch5 (clock A) and ch7 (clock B)

  int period, dutyCycle, dcValue;
  int periodValue, previousPeriod;
  int polarity;
  
  polarity = 1;       // 1 or 0 to start high or low respectively
  
  // set up PWM
  PWMCLK = 0;           // clock A as source clock
  PWMPOL = polarity;    // output start at high level
  PWMCTL = 0x11;        // 8-bit PWM5
  PWMCAE = 1;           // PWM5 centre aligned
  PWMPRCLK = 1;         // prescaler set to 2 
  
  
  // set period and duty cycle for servo to start at 0 degrees
  
  period = 500;        //microseconds (0.5 ms)
  dutyCycle = 50;     // percent
  
  while (1) {
      
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
    
    // record last period
    previousPeriod = period;
    
    // reset period based on current and previous location
    
    // if the servo is currently at 0 or 180, the next position should be 90
    if (period == 500 || period == 2500) {
      
      period = 1500; 
    } 
    
    // if the servo is currently at 90 and was just at 0, the next position should be 180
    else if (previousPeriod == 500 && period == 1500) {
      
      period = 2500; 
    } 
    
    // if the servo is currently at 90 and was just at 180, the next position should be 0
    else if (previousPeriod == 2500 && period == 1500) {
      
      period = 500;
    } 
    
    // reset the servo to 0 if anything else has happened
    else {
    
      period = 500;
    }
    

    
    
  }
  
  
	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}