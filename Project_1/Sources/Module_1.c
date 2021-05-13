#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
// Include required header files
// WARNING: Remember to alter required file depending if in simulation or hardware!

unsigned diff1, diff2, time1, time2;
volatile double timetotal;
unsigned overflow = 0;
int prescalar = 0x01;

void module1(void) {
   int operator = 3; // 0=+, 1=-, 2=*, 3=/, 4=sin(a), 5=cos(a), 6=sqrt(a) 
   double test  = 4.2;
   double test2 = 5.5;
   double result;
   	
   enableTimer(0x90, prescalar);	
   
   time1 = TCNT;
   result = test * test2;
   time2 = TCNT;
   
   
   BenchmarkTable(test, test2, prescalar);
   
   while(1){
   }
}
