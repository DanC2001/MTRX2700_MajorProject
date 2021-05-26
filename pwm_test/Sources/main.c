#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

/** The minimuim duty cycle allowed by the servos */
#define MIN_DUTY_CYCLE 2700
/** The maximium duty cycle allowed by the servos */
#define MAX_DUTY_CYCLE 6300
/** The period of the PWM signal */
#define PERIOD 60000

// Function to move servos
unsigned char move_servos(float tilt, float pan){
	
	unsigned int tilt_duty_cycle , pan_duty_cycle;
	
	if( tilt>180.0 || tilt<0.0 || pan>180.0 || pan<0.0 ) return 0;
	tilt_duty_cycle = MIN_DUTY_CYCLE + (20 * tilt);
	pan_duty_cycle = MIN_DUTY_CYCLE + (20 * pan);
		
//	assert(elevationDutyCycle <= MAX_DUTY_CYCLE && elevationDutyCycle >= MIN_DUTY_CYCLE && azimuthDutyCycle <= MAX_DUTY_CYCLE && azimuthDutyCycle >= MIN_DUTY_CYCLE);
		
	PWMDTY45 = tilt_duty_cycle;
	PWMDTY67 = pan_duty_cycle;
	return 1;
}

// Function to initialise servos
void initialise_servos(void){

  // Configure Port P data direction register for output
	DDRP = 0xFF;
	
	// Set each concatenated period channel to the calculated period value
	PWMPER45 = PERIOD;
	PWMPER67 = PERIOD;

  // Set each duty cycle to be minimum plus 90 degrees so that servos initially start from central location
	PWMDTY45 = MIN_DUTY_CYCLE + (20 * 90);
	PWMDTY67 = MIN_DUTY_CYCLE + (20 * 90);
}

// Function to initialise PWM settings
void initialise_PWM(int clock_A , int clock_B){
  
  // Set the prescalers of clock A and B
	PWMPRCLK = clock_A + (clock_B << 4);
  
  // Concatenate channels 4 and 5 to create a 16 bit channel
  PWMCTL_CON45 = 0x01;
  
  // Concatenate channels 6 and 7 to create a 16 bit channel
  PWMCTL_CON67 = 0x01;
  
  // Configure pulse to be left aligned
	PWMCAE = 0x00;
	
	// Set polarity to 1
	PWMPOL = 0xFF;
	
	// Enable PWM channels
	PWME = 0xF0;
}

// Function causes time delay of 1 millisecond * input parameter
void ms_delay(unsigned int time_ms){

    // Define counter variables to count back from
    // Volatile helps ensure this is not optimised out by compiler
    volatile int i; 
    volatile int j;
    
    // Define length of delay required for 1 ms
    volatile int ms_delay = 4000;
    
    // Waste time for number of inputted milliseconds
    for(i = 0; i < time_ms ; i++) {
      
      for(j = 0 ; j <ms_delay ; j++){
      }
    }
}

void main(void) {
    
	
  	// Define tilt and pan scan bounds passed of design requirements, includes calibration adjustments
  	unsigned char min_tilt = 60;
  	unsigned char max_tilt = 100;
  	unsigned char min_pan = 90;
  	unsigned char max_pan = 130;
  	
  	// Define increment to increase pan and tilt by
  	unsigned char increment = 2;
  	unsigned char i,j;
  	
  	
  	// Initialise PWM module with a prescaler of 2^3 for both clock A and clock B
  	initialise_PWM(3,3);
  	
  	// Initialise servos
  	initialise_servos();
  		
  	// Enable interrupts for lidar
  	EnableInterrupts;
	
  	// Scan indefinitely
  	while(1){ 	  
    
      // Loop through tilt angles
      for(i = 0; i < 5; i++){
      
        // Loop through panning angles
        for(j = 0; j < 5; j++){
          
          // Move servo to calculated position
          move_servos(min_tilt +  i*increment, max_tilt + j*increment);
          
          // Pause for half a second
          ms_delay(100);
          
        }
        
      }
      
      // Feed the dog after each scan to avoid timeout reset
      _FEED_COP();
      
  	}
    
 
  /*int max_period = 2000;
  int min_period = 1000;
  int right_moving_flag = 1;
  int increment = 100;
  float current_period;
  
      
  
  
  int duty_cycle = 50;
  int target_freq = 800; //Hz //15000 should give resulting freq of 800hz = 1.3ms, 20000 should give 600hz = 1.7ms
  int clock_freq = 12*1000*1000; // Clock is 24Mhz/prescaler = 24e6/2
  
  PWMPRCLK = 0x01; //Prescaler 	
//	PWMSCLA = 125; 	       //ClockSA=1.5MHz/2x125=6000 Hz
  //PWMSCLA = 1;
	PWMCLK = 0b00000000; 	// select clock A as the source
	PWMPOL = 0b11111111; 		     //high then low for polarity
	PWMCAE = 0x00; 		     //left aligned
	PWMCTL = 0x0C;	         //8-bit chan, PWM during freeze and wait

	PWMPER5 = clock_freq/target_freq; 	       //PWM_Freq=ClockSA/100=6000Hz/100=60Hz. CHANGE THIS
	PWMDTY5 = clock_freq/target_freq/2; 	       //50% duty cycle           AND THIS TO SEE THE EFFECT ON MOTOR. TRY LESS THAN 10%

  PWMCNT5 = 0x0;         // reset counter
  PWME = 0b11111111; 	         //Enable channel 5 PWM

  //while(1);            //stay here forever
  
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
    
    



  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
