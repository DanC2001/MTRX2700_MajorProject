
#include <hidef.h>      /* common defines and macros */
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "derivative.h"      /* derivative-specific definitions */

unsigned overflow, edge1, edge2, diff; 
unsigned long pulse_width;

void init_TC1(void){
 
  TSCR1 = 0x80;         // Activates timer, fast flag clear NOT set
  TSCR2 = 0x02;         // Sets prescaler to 4
  TIOS = 0;             // Set timer channels to be input capture
  //TCTL1 = 0x40;         // Equivalent for TC1 maybe TCTL2 = 0x4 ?  , 40 is from example with TC7
  TIE = 0x2;            // Enable channel 1 for interrupts
  TCTL4 = 0xC;          // Trigger on rising edge and falling edge
  TFLG1 = TFLG1_C1F_MASK;  // Clear the main timer interrupt flag 
  TFLG2 = 0x80;         // Clear the TOF flag 
   
}

void TCNTOF(unsigned enable){
   if(enable == 0x01){ 
      TSCR2 |= 0x80; // Enable TCNT overflow interrupt
      return;
   }else if(enable == 0x00){
      TSCR2 &=~0x80; // Disable TCNT overflow interrupt
   }
}
  


void enableTimer(int cr1, int cr2){
   TSCR1 = cr1;  // enable timer, and fast clear
   TSCR2 = cr2;  // Prescaler, no timer overflow
   TFLG2 = 0x80;  // Clear the TOF flag       
} 

void convertTimerToTime(int prescalar, unsigned timer, unsigned overflow, float* output){
   
   long Eclock = 24000000;    // 24MHz
   volatile double perStep;
   volatile double time;
   
   // gets time value for each clock tick
   switch(prescalar){        // what is the prescaler?
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

void convertTimeToDist(float* Output){
  // Takes in a pointer to a number representing a period in ms and converts it to a distance
  volatile float Calibrate = 1; // What is the distance in meters per ms of period.
  
  *Output = *Output * Calibrate;
    
}


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


#pragma CODE_SEG __NEAR_SEG NON_BANKED /* Interrupt section for this module. Placement will be in NON_BANKED area. */
__interrupt void TOV_ISR(void) { 
  //volatile int misc;
  TFLG2_TOF = 1;
  overflow++;       
}

//#pragma CODE_SEG __NEAR_SEG NON_BANKED /* Interrupt section for this module. Placement will be in NON_BANKED area. */
/*__interrupt void TC1_ISR(void) {  
  
  // At the start of this code, enable interrupts for timer overflow?
  
  // Make LED turn on or something so know that this has been executed?
  TFLG2_TOF = 1;
  //TCNTOF(0x01);         // Turn on TCNT Overflow
  TSCR2 |= 0x80; // Enable TCNT overflow interrupt
  if(edge1 == 0){       // If it is Rising edge (Hasn't measured pulse yet)
    edge1 = TC1;          // Save TCNT to edge 1
    overflow = 0;         // Set overflow counter to 0
    return;               // return and wait for falling edge
  }
  else{                 // If not rising must be falling
    diff = TC1 - edge1;   // Gets the difference between TCNT of falling and TCNT of rising
    //TCNTOF(0x00);         // Disables the TCNT overflow
    //TSCR2 &=~0x80; // Disable TCNT overflow interrupt
   /* if(TC1 < edge1){      // If it is smaller than edge 1
      overflow -= 1;      // Remove an overflow count
    } *//*
    pulse_width = (long)overflow * 65536u + (long)diff;
    NewMeasure = 1;
    edge1 = 0;
  }     
  
  // At copmletion of ISR, disable interrupts for timer overflow
  
} */


