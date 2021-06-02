#ifndef SCAN_AREA_HEADER
#define SCAN_AREA_HEADER

// This header file defines the scan area function which is used to continously scan the environemtn using
// the servo motors, and take measurements using the lidar
// This header file also defines the range and resolution that the servos move at
// This function is called directly from main
// Last editied by Andy Scott 2/06/2021


// Define tilt and pan scan bounds based on design requirements, includes calibration adjustments
// Setting values outside the range of 0-180 will cause the program to stop as these vlaues may exceed servo
// mechanical bounds and cause damage
#define MIN_TILT 90
#define MAX_TILT 110
#define MIN_PAN 20
#define MAX_PAN 160

// This sets the number of degrees between servo positions, a smaller increment will result in higher resolution
// Smaller increment also causes a longer scan time
#define INCREMENT 10

// This function performs an entire scan of the area
void scan_area(void);


#endif