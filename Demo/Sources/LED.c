#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

// This function casues a time delay of the inputted number of milliseconds
void ms_delay(unsigned int time_ms);

// This funciton initialises the LED's and displays the status of each module
// 1 is displayed if module has initialised successful, otherwise a zero is displayed
void initialise_LED(unsigned char PWM_status, unsigned char servo_status, unsigned char lidar_status){

  // Define counter variable
  unsigned int i;
  
  // Define number of loops i.e. time to display start up for
  unsigned int no_loops = 50;

  // Set port B as output
  DDRB = 0xFF;
  
  // Set port P as output
  DDRP = 0xFF;
  
  // Loop through number of loops
  for(i = 0;i < no_loops;i++){
  
    // Check if pwm has initialised
    if(PWM_status){
    
      // If successful then display one
      PORTB = 0x06;
    } 
    else{
      
        // If not successful then display a 0
        PORTB = 0x3F;
    }
    
    // Select first LED and cause slight delay so number appears
    PTP = 0x0E;       
    ms_delay(1);          

    // Check if servo has initialised successfully
    if(servo_status){
      
      // If successful display one
      PORTB = 0x06;
    } 
    else{
      
      // If not successful display a 0
      PORTB = 0x3F;
      
    }

    // Select second LED and cause slight delay so number appears
    PTP = 0x0D;
    ms_delay(1);
    
    // Check if lidar has initialised correctly
    if(lidar_status){
      
      // If successful display a one
      PORTB = 0x06; 
    }
     else{
      
         // If not successful display a 0
         PORTB = 0x3F;
    }
    
    // Select third LED and cause slight delay so number appears
    PTP = 0x0B;
    ms_delay(1);
    
    // Display a 1 to the fourth LED, indicating that all LED's are functional
    PORTB = 0x06; 
    PTP = 0x07; 
    ms_delay(1);
  }
  
  // Reset LED's to display nothing
  PORTB = 0x00;
  
}

// This function displays either left or right to the 7-segs
// Decision is based of where the min distance is relative to central point
void LED_display(unsigned int min_column, unsigned int central_column){

  // Define counter variable
  unsigned int i;
  
  // Define number of loops i.e. time to display start up for
  unsigned int no_loops = 50;

  // Check if min column is greater than or equal to central column
  // If object in middle then left reccomendation has preference as left is social norm in AUS
  if(min_column >= central_column){
    
      // Display left across the LED's
      for(i = 0;i < no_loops;i++){
      
        // Display L to the first LED
        PORTB = 0b00111000;  
        PTP = 0x0E;       
        ms_delay(1);          

        // Display E to the second LED
        PORTB = 0b01111001;
        PTP = 0x0D;
        ms_delay(1);
        
        // Display an F to the third LED LED
        PORTB = 0b01110001;
        PTP = 0x0B;
        ms_delay(1);
        
        // Display t to the fourth LED
        PORTB = 0b01110000; 
        PTP = 0x07; 
        ms_delay(1);
      }
      
      // Onced finished, display an L to the leftmost LED to be left there during scan
      PTP = 0x0E;
      PORTB =  0b00111000;
    
  } 
  // Otherwise direct the user to avoid object by going to the right
  else{
    
      // Display right across the LED's
      for(i = 0;i < no_loops;i++){
      
        // Display r to the first LED
        PORTB = 0b01010000;  
        PTP = 0x0E;       
        ms_delay(1);          

        // Display I to the second LED
        PORTB = 0b00110000;
        PTP = 0x0D;
        ms_delay(1);
        
        // Display an g to the third LED
        PORTB = 0b01101111;
        PTP = 0x0B;
        ms_delay(1);
        
        // Display t to the fourth LED
        PORTB = 0b01110000; 
        PTP = 0x07; 
        ms_delay(1);
      }
      
      // Onced finished, display an r to the rightmost LED to be left there during scan
      PTP = 0x07;
      PORTB =  0b01010000;
  
  }
  
}
