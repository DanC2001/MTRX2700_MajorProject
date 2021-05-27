#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include "Servo.h"
#include "Lidar.h"
#include "Speaker.h"


/** The minimuim duty cycle allowed by the servos */
//#define MIN_DUTY_CYCLE 2700
/** The maximium duty cycle allowed by the servos */
//#define MAX_DUTY_CYCLE 6300
/** The period of the PWM signal */
//#define PERIOD 60000

/*
// Basic function to return a distance
float scan_distanc(void){
  
  float distance = 1.5;
  return distance;
}

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

*/

void main(void) {



    
	
  	// Define tilt and pan scan bounds passed of design requirements, includes calibration adjustments
  	unsigned char min_tilt = 60;
  	unsigned char max_tilt = 100;
  	unsigned char min_pan = 50;
  	unsigned char max_pan = 170;
  	
  	// Define increment to increase pan and tilt by
  	unsigned char increment = 10;
  	unsigned char i,j;
  	unsigned char max_count_tilt = (max_tilt-min_tilt)/increment + 1;
  	unsigned char max_count_pan = (max_pan-min_pan)/increment + 1;
  	
  	// Define matrix of distances
  	float distance;
    float distance_matrix[5][13];  /* Set size equal to max count */
    
    // Configure Lidar
    DDRH_DDRH0 = 1;     // configure PH0 as output
    PTH_PTH0   = 1;     // Prime the Lidar
  	
  	
  	// Initialise PWM module with a prescaler of 2^3 for both clock A and clock B
  	initialise_PWM(3,3);
  	
  	// Initialise servos
  	initialise_servos();
  		
  	// Enable interrupts for lidar
  	EnableInterrupts;
	
  	// Scan indefinitely
  	while(1){ 	  
    
      // Loop through tilt angles
      for(i = 0; i < max_count_tilt; i++){
      
        // Loop through panning angles
        for(j = 0; j < max_count_pan; j++){
          
          // Move servo to calculated position
          move_servos(min_tilt +  i*increment, min_pan + j*increment);
          
          // Pause for half a second
          ms_delay(20);
                    
          // Take measurement
          distance = getRange();
          
          // Save measuremnt into corresponding location in the distance matrix
          distance_matrix[i][j] = distance;
          
          single_beep(1); 
          
        }
        
      }
      
      // Feed the dog after each scan to avoid timeout reset
      _FEED_COP();
      
  	}
    

  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
