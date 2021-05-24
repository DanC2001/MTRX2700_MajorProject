#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */



void main(void) 
{

  /*int max_period = 2000;
  int min_period = 1000;
  int right_moving_flag = 1;
  int increment = 100;
  float current_period;  */
  
  
  /* put your own code here */
  
  PWMPRCLK = 0x04;       //ClockA=Fbus/2**4=24MHz/16=1.5MHz	
	PWMSCLA = 125; 	       //ClockSA=1.5MHz/2x125=6000 Hz
	PWMCLK = 0b10100000; 	 //ClockSA for channel 5 & 7
	PWMPOL = 0x10; 		     //high then low for polarity
	PWMCAE = 0x0; 		     //left aligned
	PWMCTL = 0x0;	         //8-bit chan, PWM during freeze and wait


	PWMPER5 = 100; 	       //PWM_Freq=ClockSA/100=6000Hz/100=60Hz. CHANGE THIS
	PWMDTY5 = 50; 	       //50% duty cycle           AND THIS TO SEE THE EFFECT ON MOTOR. TRY LESS THAN 10%
	PWMCNT5 = 10;		       //clear initial counter. This is optional
	PWME = 0x20; 	         //Enable channel 5 PWM
  
  
  while(1);            //stay here forever
  
  /*while (1) {
      
    // calculate values for period and duty cycle values given
  
    //periodValue = current_period * (24/(2 * 2));  // calculate period value from prescaler and period
    //periodValue = current_period/512;
 
    	PWMPER5 = 100; 	       //PWM_Freq=ClockSA/100=6000Hz/100=60Hz. CHANGE THIS
    	PWMDTY5 = 50; 	       //50% duty cycle 
  
    PWME = 0x20; // enable PWM channel 5
    
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
      
    }*/

	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
