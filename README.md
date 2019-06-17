 # cs179j

s.py is the code to detect stop lights. Run it with the command python s.py

stoplight.py creates a stoplight that s.py can detect and ensure that it works.

The stop signs folder contains a folder full of stop signs and ML_test.py. This code does not work and was a failed attempt to get keras/ tensorflow wworking on my computer. 

The main.c file in the Atmega_subsystem folder is the main function running on the Atmega1284. It controls the motors through a motor driver, and also contains the collision avoidance feature.

adc.h is a header file for functions to use the adc on the Atmega.

timer.h is a header file to use a timer on the Atmega.

IR_demo.c is code to demonstrate the IR proximity sensor is working correctly.

lanes.py holds the functions that execute the lane detecion algorithm. This takes in an input of pixels, transforms the image, creates a histogram of likely lane lines, and outputs a result that is the difference the center is from the lane line center.

drive.py is the main program run on the raspberry pi. It takes in the camera module input and calls the lane detection functions along with the sign detection functions. This is also the user can edit the frames per second and resolution of the camera.
