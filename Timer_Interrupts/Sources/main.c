#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include "Functions.h"
// Include required header files
// WARNING: Remember to alter required file depending if in simulation or hardware!

// Global Variables
unsigned overflow, edge1, diff, prescalar, TCR1Set, NewMeasure; 
unsigned long pulse_width;



void main(void) {
  volatile float distance = 0;
  
/*
  prescalar = 0x02;
  TCR1Set   = 0x80;
  edge1     = 0;
  
  enableTimer(TCR1Set, prescalar);
  
  TIOS     &=~TIOS_IOS1_MASK;     // Select input capture on PT1
  TCTL4     = 0x0C;               // Set up to capture both edges
  TFLG1    |= TFLG1_C1F_MASK;     // Clear the C1F flag
  TIE       = 0x02;               // Enable interupts for timer capture 1
*/ 

  
  // Initialise channel 1 for interrupts
  init_TC1();
  
  //TCNTOF(0x01);                   // Enables TCNT Overflow interrupt
  
  TSCR2 |= 0x80; // Enable TCNT overflow interrupt
  TFLG2  = 0x80; // clear TOF flag
           
  EnableInterrupts;
  
  
  /* put your own code here */

  for(;;) {
    if(NewMeasure == 1){
      convertTimerToTime(0x02, pulse_width, overflow, &distance);
      convertTimeToDist(&distance);
      NewMeasure = 0;
    } 
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */

}

