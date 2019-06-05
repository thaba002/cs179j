import numpy as np
import time
import cv2
from picamera.array import PiRGBArray
from picamera import PiCamera
import RPi.GPIO as GPIO
from lanes import *

maxHeight = 480 #change these later
maxWidth = 640

def steerLanes(result):
	if (result == 0):
		GPIO.output(21, False) #value = 0000
		GPIO.output(22, False)
		GPIO.output(23, False)
		GPIO.output(24, False)
		print("nothing")
	elif (result > 0 and result < 10):
		GPIO.output(21, True)	#value = 0001 = 1
		GPIO.output(22, False)
		GPIO.output(23, False)
		GPIO.output(24, False)
		print("R1")
	elif (result > 10):
		GPIO.output(21, False)  #value = 0010 = 2
		GPIO.output(22, True)
		GPIO.output(23, False)
		GPIO.output(24, False)
		print("R2")
	elif (result < 0 and result > -10):
		GPIO.output(21, True) #value = 0011 = 3
		GPIO.output(22, True)
		GPIO.output(23, False)
		GPIO.output(24, False)
		print("L1")
	elif (result < -10):
		GPIO.output(21, False) #value = 0100 = 4
		GPIO.output(22, False)
		GPIO.output(23, True)
		GPIO.output(24, False)
		print("L2")
	elif (result < -20):
		#turn left hardest
		print("L3")


camera = PiCamera()
camera.resolution = (maxWidth,maxHeight)
camera.framerate = 32
rawCap = PiRGBArray(camera, size=(maxWidth, maxHeight))

time.sleep(0.1) #let it warm up

GPIO.setmode(GPIO.BOARD) #SETUP THE GPIO PINS
GPIO.setup(21,GPIO.OUT) 
GPIO.setup(22,GPIO.OUT)
GPIO.setup(23,GPIO.OUT)
GPIO.setup(24,GPIO.OUT)

for frame in camera.capture_continuous(rawCap, format="bgr", use_video_port=True):
	image = frame.array
	
	
	warped, lanes, results = LaneAssist(image) #calls the lanes.py file which runs our lane assist algorithm, results stores the difference from center
	cv2.imshow("Original", image)
	cv2.imshow("Perspective", warped)
	cv2.imshow("Lane Detect",lanes)
	steerLanes(results)
	
	
	
	
	
	key = cv2.waitKey(1) & 0xFF

	rawCap.truncate(0)
	
	if key == ord("q"):
		break

GPIO.cleanup() #reset the pins at the end of the program
