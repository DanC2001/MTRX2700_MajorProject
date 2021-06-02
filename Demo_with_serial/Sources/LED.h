#ifndef LED_HEADER
#define LED_HEADER

// This header file contains the functions relating to the LED module which is used to display information
// to the user afte the scanning process is complete
// The initialise LED function is called from main
// The LED display function is called from the scan area function
// Last editied by Andy Scott 2/06/2021

// This funciton initialises the LED's and displays the status of each module
// 1 is displayed if module has initialised successful, otherwise a zero is displayed
void initialise_LED(unsigned char PWM_status, unsigned char servo_status, unsigned char lidar_status);

// This function displays either a left facing or right facing arrow to the 7-segs
// Decision is based of where the min distance is relative to central point
void LED_display(unsigned char min_column, unsigned char central_column);

// This function creates a time delay of the inputted number of milliseconds
void millisecond_delay(unsigned int time_ms);


#endif