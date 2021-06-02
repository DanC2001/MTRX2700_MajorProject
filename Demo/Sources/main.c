#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include "Servo.h"
#include "Lidar.h"
#include "LED.h"
#include "Scan_area.h"


void main(void) {

    // Initialise status bytes
    unsigned char PWM_status,servo_status, lidar_status;
  	
  	// Initialise PWM module with a prescaler of 2^3 for both clock A and clock B
    PWM_status	= initialise_PWM(3,3);
  	
  	// Initialise servos
  	servo_status = initialise_servos();
  	
  	// Initialise Lidar
  	lidar_status = initialise_lidar();
  	
  	// Initialise the 7-seg LED's, set of 1's are displayed if no issues with initialisations
  	initialise_LED(PWM_status, servo_status, lidar_status);
  		
  	// Enable interrupts for lidar
  	EnableInterrupts;
  	
    // Continuosly scan the area looking for objects  
    scan_area();
      
    
  // Additional precaution to ensure program never leaves main
  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */

}