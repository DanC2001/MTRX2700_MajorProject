#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include "Servo.h"
#include "Lidar.h"
#include "Speaker.h"
#include "LED.h"
#include "Scan_area.h"

// This function continuosly performs an entire scan of the area and outputs information to the user
void scan_area(void){ 
  	
  	// Define counter variables to discretise area into a grid
  	unsigned char i,j;
  	unsigned char max_count_tilt = (MAX_TILT-MIN_TILT)/INCREMENT + 1;
    unsigned char max_count_pan = (MAX_PAN-MIN_PAN)/INCREMENT + 1;
	
  	// Define matrix of distances
  	float distance;
    float distance_matrix[3][15];
    
    // Define central column so can be determined if object is to left or right
    unsigned char central_column = (max_count_pan + 1)/2;
    
    // Keep track of distance to closest object and its position
    float min_distance = 3.0;    /* Currently set within bounds so beeps on first scan */
    unsigned char min_column = 1; 	
	  
	  // Scan indefinitely
	  while(1){
	    
  
      // Loop through tilt angles
      for(i = 0; i < max_count_tilt; i++){
      
        // Loop through panning angles
        for(j = 0; j < max_count_pan; j++){
          
          // Move servo to calculated position
          move_servos(MIN_TILT +  i*INCREMENT, MIN_PAN + j*INCREMENT);
          
          // Pause for slight delay to account for the time taken for servos to physically move
          ms_delay(10);          
                    
          // Take measurement
          distance = getRange();
          
          // Save measuremnt into corresponding location in the distance matrix
          distance_matrix[i][j] = distance;
          
          // Check if a closer object has been detected
          if(distance < min_distance){
            
            // Update min distance if closer object is detected
            min_distance = distance;
          }
        }
        
        // Create a beep on the speaker with frequency proporitonal to closest object
        single_beep(min_distance);
        
      }
      /* Scan complete */
      
      // Determine the minimum, non zero, value of the distance matrix, and its column index
      min_distance = distance_matrix[1][1];
      min_column = 1;
      
      // Loop through each element
      for(i = 0; i < max_count_tilt; i++){
      
        for(j = 0; j < max_count_pan; j++){
          
          // Check if element is less than min but not zero
          // Zero readings are ignored as indicates lidar error
          if(distance_matrix[i][j] < min_distance && distance_matrix[i][j] > 0){
            
            // Update the new minimum distance
            min_distance = distance_matrix[i][j];
            
            // Set the minimum column to correspond to location of min distance
            min_column = j;
          }
          
        }
      }
      
      // Display reccomended direction of travel to the user
      LED_display(min_column, central_column);
      
      // Feed the dog to avoid timeout reset
      _FEED_COP();
      
	  }
}