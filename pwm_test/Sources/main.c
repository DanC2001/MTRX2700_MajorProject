#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include "Servo.h"
#include "Lidar.h"
#include "Speaker.h"


void main(void) {

	
  	// Define tilt and pan scan bounds passed of design requirements, includes calibration adjustments
  	unsigned char min_tilt = 90;
  	unsigned char max_tilt = 110;
  	unsigned char min_pan = 50;
  	unsigned char max_pan = 170;
  	
  	// Define increment to increase pan and tilt by
  	unsigned char increment = 10;
  	unsigned char i,j;
  	unsigned char max_count_tilt = (max_tilt-min_tilt)/increment + 1;
  	unsigned char max_count_pan = (max_pan-min_pan)/increment + 1;
  	
  	// Define matrix of distances
  	float distance;
    float distance_matrix[3][13];
    
    // Define central column so can be determined if object is to left or right
    unsigned int central_column = (max_count_pan + 1)/2;
    
    // Keep track of distance to closest object and its position
    float min_distance = 3.0;
    int min_column = 1;
    
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
          
           
          
        }
        
        single_beep(min_distance);
        
      }
      /* Scan complete */
      
      // Determine the minimum, non zero, value of the distance matrix, and its column index
      min_distance = distance_matrix[1][1];
      min_column = 1;
      
      // Loop through each element
      for(i = 0; i < max_count_tilt; i++){
      
        for(j = 0; j < max_count_pan; j++){
          
          // Check if element is less than min
          if(distance_matrix[i][j] < min_distance){
            
            // Update the new minimum distance
            min_distance = distance_matrix[i][j];
            
            // Set the minimum column to correspond to location of min distance
            min_column = j;
          }
          
        }
      }
      
      // Display reccomended direction of travel to the user
      //LED_display(min_column, central_column);
      
      
      // Feed the dog after each scan to avoid timeout reset
      _FEED_COP();
      
  	}
    

  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
