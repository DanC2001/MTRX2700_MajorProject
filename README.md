# MTRX2700 Major Project
Major Project for MTRX2700 at USYD in Semester 1 2021 

Presentation: 3rd June 2021

Link to presentation slides for editing:
https://docs.google.com/presentation/d/1Vonpd3pOWvPvMa8eycqZICycjNNrLNRT4l6XoxO-a8A/edit?usp=sharing

Andy Scott, 500443844

      Primary responsibilities: LIDAR
      Other responsibilities: PWM, Timer
      
Daniel Cook, 490394414

      Primary responsibilities: Timer & interrupts
      Other responsibilities:
      
Isabella Shaw, 500554852

      Primary responsibilities: PWM & Servos, Presentation
      Other responsibilities: Documentation

# Project Summary
This project provides an assistive technology for people operating from a wheelchair. The project incorporates a panning lidar
system by using two perpendicular servos which scan the area in front of the wheelchair. A speaker and 7-seg are used as interfaces
to alert the user when there is an object close by and provide a reccomendation on the direction of travel. An optional serial
interface has also been included which prints a table of the lidar values to serial for testing means and verifying that the lidar
is working as required.

      
# User instructions      
* Set input values by changing lines xxx in main.c
* Run the program to ensure input values are within range for each module
* When stopping and restarting the program, press the reset button on the HCS12 board to ensure full functionality
* 

# Modules
## PWM & Servomotors
      High Level Code Information:
            * The functions for the PWM and servos module can be found in main.c and servo.c
            * This module takes a duty cycle and period input value and generates a square wave PWM signal based on these values
            * Alignment is set at left-aligned, but can be changed to centre- or right-alignment if necessary
            * The module will calculate the values needed to generate the correct signal, which will be used to move the servos
            * Channels 4 and 5 and channels 6 and 7 of the microcontroller are concatenated to control the tilt and pan servos, respectively
            * Minimum and maximum pan and tilt values are set to suit our intended function for the servo module, but can be adjusted for other uses
            * The servos take the PWM signal as an input in conjunction with the pan and tilt values to set the position of the servo
            * The PWM signal, pan and tilt values are then adjusted to cause the servos to continously move
      Limitations
            * Signal can only be generated within E-Clock limitations, as period and duty cycle are based on E-clock frequency (24MHz)
            * Full range of motion of the servos is limited to ...
            * ...
      Testing
            * Attach an oscilloscope to the relevant pins on the microcontroller to see the PWM signal and check it has the correct duty cycle and period
            * Set up an LED to flash with the PWM signal (LED is on when PWM is HIGH)
            * Run the program using the microcontroller and ensure the servos are moving as expected
                  * Disable either pan or tilt PWM channels to check that each one is working in isolation before combining them
            * Used assert functions to stop program if inputs were outside the range
            * Tested different inputted pan and tilt values to ensure that servos were never directed to move past their mechanical stops

## Timer
      High Level Code Information:
            * The functions for the timer module can be found in Lidar.c and main.c
            * This module utilises the timer channels on the HCS12 board to work in conjunction with the LIDAR sensor
            * The timer is used to calculate the time between rising and falling edges, signals generated by the LIDAR when an object is detected
            * This module takes the time data (initially measured in clock cycles) and converts it to a time measurement in milliseconds
            * The time value can then be outputted to other modules, converted to a distance value (where 1 microsecond = 1cm)
      Limitations
            * Limits the use of the timer for other functions in a project
      Testing
            * 
            
## LIDAR
      High Level Code Information:
            * The functions for the timer module can be found in Lidar.c and main.c
            * The LIDAR is a sensor that is used to detect obstacles in the surrounding environment
            * The sensor is mounted on the servos and scans the surrounds as the servo is being moved
            * The LIDAR module works in conjuction with the timer to collect and store data
            * This module calculates distance from time data from the timer and stores it in an array
            * The minimum value in the matrix (excluding 0s) is extracted and can be used as an output
      Limitations
            * The LIDAR hardware can give false or zero readings
      Testing
            * Started by taking single measurements to a large object and then measured physical difference to ensure they matched
            * Then tested it by taking measurements across a grid using servos and outputted the matrix to serial to ensure that results aligned with the                         environment being mapped

            
## Serial
      High Level Code Information:
            * The functions for the timer module can be found in serial.c and main.c
      Limitations
            * 
      Testing
            *            
            
## Speaker
      High Level Code Information:
            * The functions for the timer module can be found in speaker.c
            * Tis module takes distance information from the LIDAR module and plays a beep at a pitch that is inversely proportional to the distance value
                  * The closer the obstacle is, the higher the pitch
            * Each time a row in the distance matrix is filled, a beep sounds from the speaker at a pitch calculated from the last minimum value inputted to the                 module from the LIDAR module
            * When the matrix is full, the minimum value is sent to this module and a new pitch is calculated
      Limitations
            * Speaker cannot sound while the servos and LIDAR are scanning, only when they have finished one scan, so beeping cannot be continuous
      Testing
            * Tested that module still functioned appropriately for invalid inputs such as 0 or negative values
            * Tested that if the distance is above the upper bound then the speaker stops 
            * Moved an object from far to near and audibly ensured that the frequency of beeps increased

            
## LEDs
      High Level Code Information:
            * The functions for the timer module can be found in LED.c
            * This module takes the index within the distance matrix of the minimum distance value from the LIDAR as an input
            * The index is used to determine if the obstacle is on the left or the right of the sensor
            * The 4, 7-seg LEDs will display a message (LEFT or RIGT) telling the user where to turn to avoid the obstacle
            * If the object is directly in front of the person, LEFT will be displayed (in conjunction with Australian social norms)
                  * This can be changed for use in other countries to display RIGT by changing lines xxxx
            * After exiting the module, an L or R will remain displayed on the first LED until the next full scan is complete
      Limitations
            * Only 4 7-seg LEDs are available (hence having to display "rigt" instead of "right")
            * Only 1 LED can remain on while the servos and LIDAR continue to scan
      Testing
            * Basic functionality easily tested using visual appearance
            * Tested that the indicated direction was correct by moving an object horizontally across the field of view of the lidar while in operation and                       confirmed that the recommended direction changed
