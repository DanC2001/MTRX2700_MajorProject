#include <hidef.h>      /* common defines and macros */
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "derivative.h"      /* derivative-specific definitions */

extern unsigned overflow, edge1, diff, prescalar, TCR1Set; 
extern unsigned long pulse_width;

void enableTimer(int cr1, int cr2){
   TSCR1 = cr1;  // enable timer, and fast clear
   TSCR2 = cr2;  // Prescaler, no timer overflow
   TFLG2 = 0x80;  // Clear the TOF flag       
} 


void TCNTOF(unsigned enable){
   if(enable == 0x01){ 
      TSCR2 |= 0x80; // Enable TCNT overflow interrupt
      return;
   }else if(enable == 0x00){
      TSCR2 &=~0x80; // Disable TCNT overflow interrupt
   }
}

int convertTimerToTime(int prescalar, unsigned timer, float* output){
   
   long Eclock = 24000000;    // 24MHz
   volatile double perStep;
   volatile double time;
   
   // gets time value for each clock tick
   switch(prescalar){         // what is the prescaler?
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
   
   time = timer * (1/perStep);  // Seconds
   time = time*1000;            // Miliseconds  (ms)
   time = time*1000;            // Microseconds (µs)
   
   *output = time;
   return 1;               
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED /* Interrupt section for this module. Placement will be in NON_BANKED area. */
__interrupt void tovisr(void) {  
  TFLG2 = 0x80;
  overflow++;       
}

__interrupt void tieC1I(void) {  
  TFLG2 = 0x80;
  TCNTOF(0x01);
  if(edge1 == 0){  
    edge1 = TC1;
    overflow = 0;
    return;
  }else{
    diff = TC1 - edge1;
    TCNTOF(0x00);
    if(TC1 < edge1){
      overflow -= 1;
    }
    pulse_width = (long)overflow * 65536u + (long)diff;
    edge1 = 0;
  }
  
}

