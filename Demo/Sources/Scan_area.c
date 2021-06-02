#include <hidef.h>      /* common defines and macros */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "derivative.h"      /* derivative-specific definitions */
#include "Servo.h"
#include "Lidar.h"
#include "Speaker.h"
#include "LED.h"
#include "Scan_area.h"

// Function to make table as a string
static char * matrix2D_to_string(const float *matrix, size_t rows, size_t cols){
  //~~~ Strings in the stack ~~~//
  const char * format         = "%.2f";
  const char * col_seperator  = " ";
  const char * row_seperator  = "\n";
  
  int width = 0;
  int *col_width = NULL;
  size_t r = 0, c = 0, size = 0;
  char *buffer = NULL, *p = NULL;
  char buf[256];
  
  assert(rows<=0 || cols<=0);
  
  // Calculates maximum width for each column
  col_width = (int *)calloc(cols, sizeof(*col_width));
  
  for(r = 0; r < rows; ++r){
    for(c = 0; c < cols; ++c){
      width = sprintf(buf, format, matrix[r * cols + c]);
      if(width > col_width[c]){
        col_width[c] = width;
      }
    }
  }
  
  // Calculate the total buffer size taking...
  // ... Each value into account
  for(c = 0; c < cols; ++c){
    size += col_width[c] * rows;
  }
  
  // ... Column seperators into account
  size += (cols - 1) * strlen(col_seperator);
  
  // ... Row seperators into account
  size += (rows - 1) * strlen(row_seperator);
  
  // ... Null terminator into account
  size++;
  
  // Make the string
  buffer = (char*)malloc(size);
  p = buffer;
  for(r = 0; r < rows; ++r){
    if(r){
      strcpy(p,row_seperator);
      p += strlen(row_seperator); 
    }
    for(c = 0; c < cols; ++c){
      if(c){
        strcpy(p,col_seperator);
        p += strlen(col_seperator); 
      }
      width = sprintf(p,format,matrix[r * cols +c]);
      p += width;
      if(width < col_width[c]){
        width = col_width[c] - width;
        memset(p,' ', width);
        p += width; 
      }
    }
  }
  *p = '\0';
  
  //cleanup
  free(col_width);
  
  return buffer;
}


// This function continuosly performs an entire scan of the area and outputs information to the user
void scan_area(void){ 
  	
  	// Define a string for table
	  char *s = NULL;
  	
  	byte SCI_port = 1;
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
          //distance = 0.12;
          
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
          // Readings less than 4cm are ignored as indicates lidar error
          if(distance_matrix[i][j] < min_distance && distance_matrix[i][j] > 0.04){
            
            // Update the new minimum distance
            min_distance = distance_matrix[i][j];
            
            // Set the minimum column to correspond to location of min distance
            min_column = j;
          }
          
        }
      }
      
      s = matrix2D_to_string((const float *)distance_matrix, ARRAY_SIZE(distance_matrix), ARRAY_SIZE(distance_matrix[0]));
      tx.length = strlen(s);
      assert(tx.length <=512);
      strcpy(tx.str, s);
      tx.cursor = 0;
      send_mesage(SCI_port);
      
      // Display reccomended direction of travel to the user
      LED_display(min_column, central_column);
      
      
      
      // Feed the dog to avoid timeout reset
      _FEED_COP();
      
	  }
}