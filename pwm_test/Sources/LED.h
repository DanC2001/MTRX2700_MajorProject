#ifndef LED_HEADER
#define LED_HEADER

// This funciton initialises the LED's and also displays a horizontal line
// to ensure it is functioning as it should
void initialise_LED(void);

// This function displays either a left facing or right facing arrow to the 7-segs
// Decision is based of where the min distance is relative to central point
void LED_display(unsigned int min_column, unsigned int central_column);

// This function creates a time delay of the inputted number of milliseconds
void millisecond_delay(unsigned int time_ms);


#endif