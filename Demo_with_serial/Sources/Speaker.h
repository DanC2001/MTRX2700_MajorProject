#ifndef SPEAKER_HEADER
#define SPEAKER_HEADER

// This header file defines the functions relating to the speaker module which is used to give
// audible feedback to the user
// The single beep function is called from the scan area function
// The microsec delay function is called by the single beep function to generate a periodic waveform
// Last editied by Andy Scott 2/06/2021

// This function creates a time delay of 10*input microseconds
void microsec_delay(unsigned int time_ms);

// This function creates a beep on the speaker at the inputted frequency
void single_beep(float beep_frequency);


#endif