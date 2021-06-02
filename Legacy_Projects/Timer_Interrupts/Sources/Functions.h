#ifndef FUNCTIONS_HEADER
#define FUNCTIONS_HEADER

void convertTimerToTime(int prescalar, unsigned timer, unsigned overflow, float* output);
void convertTimeToDist(float* Output);
float getRange(void);
__interrupt void TOV_ISR(void);


#endif