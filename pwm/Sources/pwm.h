#ifndef PWM_HEADER
#define PWM_HEADER

#include <hidef.h> 
#include <stdlib.h>
#include "derivative.h"

#define PERIOD 24 // change period value to change frequency
#define LOW 0
#define HIGH 1
#define disable() __asm(sei)
#define enable() __asm(cli)


extern char pwm_state; 	// PWM low or high
extern int high_time; // how long to stay high

__interrupt void TOC2_ISR(void);

void pwm(float, float);

#endif