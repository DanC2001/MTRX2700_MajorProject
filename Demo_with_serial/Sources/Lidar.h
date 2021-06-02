#ifndef LIDAR_HEADER
#define LIDAR_HEADER

// This function initialises the Lidar by configuring it for manual triggering
byte initialise_lidar(void);

// Converts the number of clock cycles in a pulse to a time in milliseconds
void convertTimerToTime(int prescalar, unsigned timer, unsigned overflow, float* output);

// Uses 1ms to m ratio to convert time in milliseconds to distance in metres
void convertTimeToDist(float* Output);

// Function to trigger lidar and returns the range in metres to the nearest object
float getRange(void);

// Timer overflow interrupt service routine
__interrupt void TOV_ISR(void);


#endif