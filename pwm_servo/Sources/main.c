#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <math.h>


void main(void) {
  // pwm ch5 (clock A) and ch7 (clock B)

  float current_period, dutyCycle, dcValue;
  float periodValue; // previousPeriod, nextPeriod;
  int polarity;
  int max_period = 2000;
  int min_period = 1000;
  int right_moving_flag = 1;
  int increment = 100;
  
  polarity = 1;       // 1 or 0 to start high or low respectively
  
  // set up PWM
  PWME = 0b10100000;     // Enable PWM channel 5 and 7
  
  PWMCLK = 0;           // clock A as source clock * Note: this sets the clock for PWM channel 7 to be clock B *
  PWMPOL = polarity;    // output start at high level
  PWMCTL = 0x11;        // 8-bit PWM5
  PWMCAE = 1;           // PWM5 centre aligned
  //PWMPRCLK = 1;         // prescaler set to 2
 
  PWMPRCLK = 0;     // Try without prescaler 
  PWMSCLA = 0;
  PWMSCLB = 0;  // Set scalers to 0, in this setting clock is divided by 512 according to datasheet
  
  
  // set period and duty cycle for servo to start at 0 degrees
 // previousPeriod = 500;
  current_period = 1500;        //microseconds (0.5 ms)
 // nextPeriod = 500;
  dutyCycle = 50;     // percent
  
  while (1) {
      
    // calculate values for period and duty cycle values given
  
    //periodValue = current_period * (24/(2 * 2));  // calculate period value from prescaler and period
      periodValue = current_period/512;
  
  
    // calculate duty cycle value from duty cycle, polarity and period value
    if (polarity == 1) {
    
      dcValue = (dutyCycle * 0.01) * (periodValue);   // wtf
    }
    else if (polarity == 0) {
    
      dcValue = periodValue - ((dutyCycle * 0.01) * periodValue);
    } 
        
    
    // set the period and duty cycle values
    //PWMPER5 = periodValue;   
    PWMPER5 = periodValue; 
    PWMDTY5 = dcValue;
  
    PWME = PWME | 0x20; // enable PWM channel 5
    
    // reset period based on current and previous location
    
    // If servo is moving to right then try to increase period
    if(right_moving_flag == 1){
      
      // Check to see if increasing period any more will go outside of bounds
      if(current_period + increment > max_period){
      
        // If outside bounds, then set period to max and switch flag to left moving
        current_period = max_period;
        right_moving_flag = 0;
        
        
      } 
      // If not outside bounds than perform increase
      else  {
        
        current_period = current_period + increment;
        
      }
      
    }
    
    // Otherwise servo is left moving and period should be decremented
    else{
    
      // Check if decrementing period will go below the bound
      if(current_period - increment < min_period){
        
        // If outside bounds then set current period as min and switch flag to right moving
        current_period = min_period;
        right_moving_flag = 1;
      }
      // Otherwise decrement the period
      else{
        
        current_period = current_period - increment;
      }     
      
    }
    
    
    
    /*
    // if the servo is currently at 0 or 180, the next position should be 90
    if (currentPeriod == 500 || currentPeriod == 2500) {
      
      nextPeriod = 1500; 
    } 
    
    // if the servo is currently at 90 and was just at 0, the next position should be 180
    else if (previousPeriod == 500 && currentPeriod == 1500) {
      
      nextPeriod = 2500; 
    } 
    
    // if the servo is currently at 90 and was just at 180, the next position should be 0
    else if (previousPeriod == 2500 && currentPeriod == 1500) {
      
      nextPeriod = 500;
    } 
    
    // reset the servo to 0 if anything else has happened
    else {
    
      nextPeriod = 500;
    }
    
    previousPeriod = currentPeriod;
    currentPeriod = nextPeriod;
   */ 
     _FEED_COP();
    

    
    
  }
  
  
	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}