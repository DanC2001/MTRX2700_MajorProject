#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include <assert.h> /* Assert header for error catching */

// Defines the minimum distance in centimetres, anything below this will create same pitch noise
#define MIN_DISTANCE 5

// Defines the maximum distance that the speaker will make noise for
#define MAX_DISTANCE 300


// This function creates a time delay of 10*input microseconds
void microsec_delay(unsigned int time_ms){
    
    // Define counter variables to count back from
    // Volatile helps ensure this is not optimised out by compiler
    volatile int i; 
    volatile int j;
    
    // Define length of delay required for 10 microseconds
    volatile int ms_delay = 400;
    
    // Check that input is valid
    assert(time_ms >= 0);
    
    // Waste time for number of inputted microseconds
    for(i = 0; i < time_ms ; i++) 
      for(j = 0 ; j <ms_delay ; j++);
}


// This function creates a beep on the speaker at a frequency inversely proportional to min distance
void single_beep(float min_distance){
    
    // Define array index
    unsigned int i = 0;
    
    // Define period to be used
    unsigned int period;
    
    // Define conversion scale factor for distance to period
    unsigned int scale_factor = 5;
    
    // Number of loops required for beep of desired length
    unsigned int no_loops = 4;
    
    // Convert inputted distance from metres to centimetres
    float distance_cm = min_distance*100;
    
    // Round the distance to the nearest centimetre
    int distance = (int)distance_cm;
    
    // Check that input is valid
    assert(min_distance >= 0);
    
    // Check if distance is greater than max
    if(distance > MAX_DISTANCE){
      
      // Set the period value to be zero so beep turns off
      period = 0;
      
    }
    // Check if distance is below min threshold
    else if(distance < MIN_DISTANCE){
      
      // Set the period to the minimum possible value so at highest frequency
      period = 1;
    }
    // Otherwise calculate the intermediate period value
    else{
      
      period = distance/scale_factor;
    }
    
    
    // Set pin 5 of port P to be output 
    DDRT |= PTT_PTT5_MASK;
    
    // Create square wave at designated frequency for length of no_loops
    for (i = 0; i < no_loops ; i++){
      
      // Set speaker port to low and pause
      PTT_PTT5 = 0;
      microsec_delay(period);
      
      // Set speaker port to high and pause
      PTT_PTT5 = 1;
      microsec_delay(period);
    } 
  
}