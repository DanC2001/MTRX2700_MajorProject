#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include "Functions.h"
// Include required header files
// WARNING: Remember to alter required file depending if in simulation or hardware!

// Global Variables
unsigned overflow, edge1, diff, prescalar, TCR1Set; 
unsigned long pulse_width;



void main(void) {

  prescalar = 0x02;
  TCR1Set   = 0x90;
  edge1     = 0;
  
  enableTimer(TCR1Set, prescalar);
  
  TIOS     &=~TIOS_IOS1_MASK;     // Select input capture on PT1
  TCTL4     = 0x0C;               // Set up to capture both edges
  TFLG1    |= TFLG1_C1F_MASK;     // Clear the C1F flag
  TIE       = 0x02;               // Enable interupts for timer capture 1
          
  EnableInterrupts;
  
  
  /* put your own code here */
  


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */

}

