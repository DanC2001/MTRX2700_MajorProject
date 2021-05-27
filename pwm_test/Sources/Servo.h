#ifndef SERVO_HEADER
#define SERVO_HEADER



// This function configures PWM channel 5 and 7, the two input arguments are the prescaler
// values for clock A and B, function sets waves as polarity 1 and left aligned
void initialise_PWM(int clock_A , int clock_B);

// This function initialises the servos to begin in the central position
void initialise_servos(void);

// This function takes in a tilt and pan angle and moves the servo to the desired position
// A one is returned on success and a 0 is returned on failure if angle is out of range
unsigned char move_servos(float tilt, float pan);

// This function casues a time delay of the inputted number of milliseconds
void ms_delay(unsigned int time_ms);



#endif

