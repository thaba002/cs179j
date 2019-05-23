from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv4

#Sets up the camera to be read
camera = PiCamera()
camera.resolution(640,480) #setting the resolution to 640x480
camera.framerate = 30 #gonna need to play with this setting during testing
rawCap = PiRGBArray(camera, size = (640,480)) #creates the array

time.sleep(0,1)

for frame in camera.capture_continuous(rawCap, format="bgr", use_video_port = True):
    image = frame.array

    cv2.imshow("Frame", image) #shows the frame for testing
    key = cv2.waitKey(1) & 0xFF

    rawCap.truncate(0) #clears stream for next frame

    if (key == ord("q")): #exits the array
        break;
