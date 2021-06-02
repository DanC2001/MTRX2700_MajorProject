#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

void delay(void){
  
  int i;
  int j;
  
  for(i = 10000; i > 0; i--){
  
    for(j = 4000; j > 0; j--){
     
     }
  
  }
}
  

void main(void) {
  /* put your own code here */
  PTT_PTT1 = 0;
  
    // Configure PORTB as output
  DDRB = 0xFF;
  
  // Configure PORTP as output        
  DDRP = 0xFF;
  
  // Select first LED
  PTP = 62;
  
  
  // Set timer input capture on channel 1
  TIOS = TIOS | TIOS_IOS1_MASK;
   
  // Check if PT1 is high
  for(;;){
    
    if(PTT_PTT1 == 1){
    
    
      
      PORTB = 6;   // Displays 1
      
    } 
    else{

        PORTB = 63;    // Display 0
        delay();
      
    }
          _FEED_COP();

 
  }
  
  

	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
