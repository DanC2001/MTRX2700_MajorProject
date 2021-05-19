#ifndef FUNCTIONS_HEADER
#define FUNCTIONS_HEADER

//void enableTimer(int cr1, int cr2);
//int convertTimerToTime(int prescalar, unsigned timer, float* output);
void init_TC1(void);
void TCNTOF(unsigned enable);
__interrupt void TOV_ISR(void);
__interrupt void TC1_ISR(void);

#endif