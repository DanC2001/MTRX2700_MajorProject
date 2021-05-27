# MTRX2700 Major Project
Major Project for MTRX2700 at USYD in Semester 1 2021 

Presentation: 3rd June 2021

Andy Scott, 

      Primary responsibilities: LIDAR
      Other responsibilities: PWM, Timer
      
Daniel Cook, 490394414

      Primary responsibilities: Timer & interrupts
      Other responsibilities:
      
Isabella Shaw, 500554852

      Primary responsibilities: PWM & Servos, Presentation
      Other responsibilities: Documentation
      
# User instructions      
* set values
* ...

# Modules
## PWM & Servomotors
      High Level Code Information:
            * The PWM and servo module is located in pwm.c
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
## Timer
      High Level Code Information:
            * rbfkwejn
      Limitations
            * bslunsr
      Testing
            * chjrbcsne
            
## LIDAR
      High Level Code Information:
            * rbfkwejn
      Limitations
            * bslunsr
      Testing
            * chjrbcsne
            
## Speaker
      High Level Code Information:
            * rbfkwejn
      Limitations
            * bslunsr
      Testing
            * chjrbcsne
            
## LEDs
      High Level Code Information:
            * rbfkwejn
      Limitations
            * bslunsr
      Testing
            * chjrbcsne
            
