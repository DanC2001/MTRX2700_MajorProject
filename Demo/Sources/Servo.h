#ifndef SERVO_HEADER
#define SERVO_HEADER

// This header file defines all the functions relating to the use of the servos which
// are used to scan the surroundings
// The initialise PWM function and initialise servos function are called from main
// The remaining functions are called from the scan area function
// Last editied by Andy Scott 2/06/2021


// This function configures PWM channel 5 and 7, the two input arguments are the prescaler
// values for clock A and B, function sets waves as polarity 1 and left aligned
unsigned char initialise_PWM(int clock_A , int clock_B);

// This function initialises the servos to begin in the central position
unsigned char initialise_servos(void);

// This function takes in a tilt and pan angle and moves the servo to the desired position
void move_servos(float tilt, float pan);

// This function casues a time delay of the inputted number of milliseconds
void ms_delay(unsigned int time_ms);



#endif

