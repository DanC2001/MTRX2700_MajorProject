
#include <hidef.h>      /* common defines and macros */
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "derivative.h"      /* derivative-specific definitions */

// Variables local to this file to keep track of rising and falling edge and time the distance between them
unsigned overflow, edge1, edge2, diff; 
unsigned long pulse_width;

// This function initialises the Lidar by configuring it for manual triggering
byte initialise_lidar(void){

      // Configure Lidar
    DDRH_DDRH0 = 1;     // configure PH0 as output
    PTH_PTH0   = 1;     // prime the Lidar by setting signal to start high
    return 1;
}

// Converts the number of clock cycles in a pulse to a time in milliseconds
void convertTimerToTime(int prescalar, unsigned timer, unsigned overflow, float* output){
   
   long Eclock = 24000000;    // 24MHz
   volatile double perStep;
   volatile double time;
   
   // gets time value for each clock tick
   switch(prescalar){        
    case 0x00:  //000
      perStep = Eclock;
      break;
    
    case 0x01:  //001
      perStep = Eclock/2;
      break;
      
    case 0x02:  //010
      perStep = Eclock/4;
      break;
   
    case 0x03:  //011
      perStep = Eclock/8;
      break;
    
    case 0x04:  //100
      perStep = Eclock/16;
      break;
   
    case 0x05:  //101
      perStep = Eclock/32;
      break;
      
    case 0x06:  //110
      perStep = Eclock/64;
      break;
      
    case 0x07:  //111
      perStep = Eclock/128;
      break;
      
   }
   
   time = (timer + (overflow * 65536u)) * (1/perStep);  // Seconds
   time = time*1000;            // Miliseconds  (ms)
   // time = time*1000;            // Microseconds (µs)
   
   *output = time;         
}

// Uses 1ms to m ratio to convert time in milliseconds to distance in metres
void convertTimeToDist(float* Output){
  // Takes in a pointer to a number representing a period in ms and converts it to a distance
  volatile float Calibrate = 1; // What is the distance in meters per ms of period.
  
  *Output = *Output * Calibrate;
    
}

 // Function to trigger lidar and returns the range in metres to the nearest object
 float getRange(void){

    float distance = 0.0;
    
    // Set overflow to zero
    overflow = 0;
    // Set up timer to capture rising edge
    TSCR1 = 0x80;       // Enable timer counter, no enable fast flag clear
    TSCR2 = 0x00;       // Disable overflow interrupt, prescalar stays at 0
                        // Prescalar has to remain 0 due to its use in interrupts
                  
    TIOS &= ~0x02;      // Switch IOS1 to input-capture and leave other timers alone
    TCTL4 = 0x04;       // Capture on RISING edge only of PT1
    TFLG1 = 0x02;       // Write a 1 to the interrupt flag register for PT1, clearing it
    
    PTH_PTH0   = 0;     // Trigger the Lidar
    
    // Wait for rising edge 
    
    while(!(TFLG1 & 0x02)); // Wait until the first rising edge triggers TFLG1 for PT1
    TFLG2 = 0x80;       // Clear the timer overflow flag
    TSCR2|= 0x80;       // Turn on the Timer overflow interrupt bit        
    edge1 = TC1;        // Save the time the rising edge was recorded, and clear the C1F flag from TFLG1
    TFLG1 = 0x02;       // Write a 1 to the interrupt flag register for PT1, clearing it
    TCTL4 = 0x08;       // Capture on FALLING edge only of PT1
    
    // Time until falling edge
    while(!(TFLG1 & 0x02)); // Wait until the falling edge triggers the TFLG1 flag
    edge2 = TC1;
    TFLG1 = 0x02;       // Write a 1 to the interrupt flag register for PT1, clearing it
    diff = edge2 - edge1; // How many clock cycles pulse was high disregarding overflow
    if(edge2 < edge1){    // If the time that the edge fell was recorded 'before' it rose, then:
      overflow -= 1;    // Subtract 1 from overflow
    }
    
    pulse_width = (long)overflow * 65536u + (long)diff;
    
    convertTimerToTime(0x00, pulse_width, overflow, &distance);
    convertTimeToDist(&distance);
    
    PTH_PTH0   = 1;     // Untrigger the Lidar
    
    return distance;
 }

// Timer overflow interrupt service routine
#pragma CODE_SEG __NEAR_SEG NON_BANKED /* Interrupt section for this module. Placement will be in NON_BANKED area. */
__interrupt void TOV_ISR(void) { 
  //volatile int misc;
  TFLG2_TOF = 1;
  overflow++;       
}



