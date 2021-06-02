#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

//#include "Servo.h"
// The minimuim duty cycle allowed by the servos
#define MIN_DUTY_CYCLE 2700

// The maximium duty cycle allowed by the servos
#define MAX_DUTY_CYCLE 6300

// The period of the PWM signal
#define PERIOD 60000

// Include header file so that assert function is defined
#include <assert.h>


// This function takes in a tilt and pan angle and moves the servo to the desired position
// A one is returned on success and a 0 is returned on failure if angle is out of range
void move_servos(float tilt, float pan){
	
	// Define duty cycle
	unsigned int tilt_duty_cycle , pan_duty_cycle;
	
	// If values are beyond mechincal stops then set back to 90,90
	if( tilt>180.0 || tilt<0.0 || pan>180.0 || pan<0.0 ){
	  
	  tilt_duty_cycle = MIN_DUTY_CYCLE + (20 * 90);
	  pan_duty_cycle = MIN_DUTY_CYCLE + (20 * 90);
	  
	}
	tilt_duty_cycle = MIN_DUTY_CYCLE + (20 * tilt);
	pan_duty_cycle = MIN_DUTY_CYCLE + (20 * pan);
		
  assert(tilt_duty_cycle <= MAX_DUTY_CYCLE && tilt_duty_cycle >= MIN_DUTY_CYCLE);
  assert(pan_duty_cycle <= MAX_DUTY_CYCLE && pan_duty_cycle >= MIN_DUTY_CYCLE);
		
	PWMDTY45 = tilt_duty_cycle;
	PWMDTY67 = pan_duty_cycle;
	
}

// This function initialises the servos to begin in the central position
byte initialise_servos(void){

  // Configure Port P data direction register for output
	DDRP = 0xFF;
	
	// Set each concatenated period channel to the calculated period value
	PWMPER45 = PERIOD;
	PWMPER67 = PERIOD;

  // Set each duty cycle to be minimum plus 90 degrees so that servos initially start from central location
	PWMDTY45 = MIN_DUTY_CYCLE + (20 * 90);
	PWMDTY67 = MIN_DUTY_CYCLE + (20 * 90);
	
	return 1;
}

// This function configures PWM channel 5 and 7, the two input arguments are the prescaler
// values for clock A and B, function sets waves as polarity 1 and left aligned
byte initialise_PWM(int clock_A , int clock_B){
  
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
	
	return 1;
}

// This function casues a time delay of the inputted number of milliseconds
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