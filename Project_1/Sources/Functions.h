#ifndef FUNCTIONS_HEADER
#define FUNCTIONS_HEADER

void enableTimer(int cr1, int cr2);
void TCNTOF(unsigned enable);
int convertTimerToTime(int prescalar, unsigned timer, float* output);
__interrupt void tovisr(void);
__interrupt void tieC1I(void);

#endif