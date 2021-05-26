#ifndef FUNCTIONS_HEADER
#define FUNCTIONS_HEADER

void enableTimer(int cr1, int cr2);
void convertTimerToTime(int prescalar, unsigned timer, unsigned overflow, float* output);
void convertTimeToDist(float* Output);
void init_TC1(void);
int getRange(void);
void TCNTOF(unsigned enable);
__interrupt void TOV_ISR(void);
__interrupt void TC1_ISR(void);

#endif