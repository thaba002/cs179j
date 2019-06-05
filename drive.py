import numpy as np
import time
import cv2
from picamera.array import PiRGBArray
from picamera import PiCamera
from lanes import *

maxHeight = 480 #change these later
maxWidth = 640

def steerLanes(result):
	if (result == 0):
		#do nothing
		print("nothing")
	elif (result > 0 and result < 10):
		#turn right slightly
		print("R1")
	elif (result > 10 and result < 20):
		#turn right harder
		print("R2")
	elif (result > 20):
		#turn right hardest
		print("R3")
	elif (result < 0 and result > -10):
		#turn left slightly
		print("L1")
	elif (result < -10 and result > -20):
		#turn left harder
		print("L2")
	elif (result < -20):
		#turn left hardest
		print("L3")


camera = PiCamera()
camera.resolution = (maxWidth,maxHeight)
camera.framerate = 32
rawCap = PiRGBArray(camera, size=(maxWidth, maxHeight))

time.sleep(0.1) #let it warm up

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
