#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// This function creates a time delay for a specified number of microseconds
void microsec_delay(unsigned int time_ms){

    // Define counter variables to count back from
    // Volatile helps ensure this is not optimised out by compiler
    volatile int i; 
    volatile int j;
    
    // Define length of delay required for 10 microseconds
    volatile int ms_delay = 400;
    
    // Waste time for number of inputted microseconds
    for(i = 0; i < time_ms ; i++) 
      for(j = 0 ; j <ms_delay ; j++);
}


// This function creates a beep on the speaker for the desired length
void single_beep(unsigned int beep_length){

    // Define array index
    unsigned int i = 0;
    
    // Define scale factor
    unsigned int scale_factor = 10;
    
    // Calculate how many loops is equal to the designated time
    unsigned int no_loops = beep_length*scale_factor;
    
    // Set pin 5 of port P to be output 
    DDRT |= PTT_PTT5_MASK;
    
    // Create square wave at designated frequency for length of no_loops
    for (i = 0; i < no_loops ; i++){
      
      // Set speaker port to low and pause
      PTT_PTT5 = 0;
      microsec_delay(10);
      
      // Set speaker port to high and pause
      PTT_PTT5 = 1;
      microsec_delay(10);
    } 
  
}


void main(void) {


  for(;;){
  
    volatile int button_state = PTH0;
    
    if(button_state == 1){
      
    
    single_beep(1);
    
    microsec_delay(10);
    
    }
  }

  
/*
  
    // Initialise variables
    unsigned int array_index = 0;
    unsigned int delay_index = 0;
    unsigned int no_loops;

    // Define the length of the string
    int length = strlen(input_string);
    
    // Set pin 5 of port P to be output 
    DDRT |= PTT_PTT5_MASK;
    
    // Loop through each frequency to be played 
     
    for (array_index = 0; array_index < length ; array_index++){
      
       // Calculate how many loops is equal to the designated time
       no_loops = note_duration/(2*period_array[array_index]);
          
       // Play each note for designated time
       for(delay_index = 0; delay_index <  no_loops; delay_index++){
        
        // Set speaker port to low and pause 
        PTT_PTT5 = 0;
        milsec_delay(period_array[array_index]);
        
        // Set speaker port to high and pause
        PTT_PTT5 = 1;    
        milsec_delay(period_array[array_index]);
       }
             
    }             

 
*/  


	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
