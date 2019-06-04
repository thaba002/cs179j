import numpy as np
import time
import cv2

maxHeight = 480 #change these later
maxWidth = 640

def Perspective(image, image_Points, des_Points):
	cv2.line(image, image_Points[0], image_Points[1], (255, 0, 0), thickness=2)
	cv2.line(image, image_Points[0], image_Points[2], (255, 0, 0), thickness=2)
	cv2.line(image, image_Points[2], image_Points[3], (255, 0, 0), thickness=2)
	cv2.line(image, image_Points[1], image_Points[3], (255, 0, 0), thickness=2)
	
	
	matrix = cv2.getPerspectiveTransform(np.float32(image_Points), np.float32(des_Points))
	warped = cv2.warpPerspective(image,matrix,(maxWidth, maxHeight))
	#this creates the birds eye view for centering the vehicle
	return warped
	
def Threshold(image):
	gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
	gray_threshold = cv2.inRange(gray, 2,28)
	canny = cv2.Canny(gray_threshold, 500, 700)
	final = cv2.add(canny, gray_threshold)
	final = cv2.cvtColor(final, cv2.COLOR_GRAY2RGB)
	return final

def Histogram(image):
	array = np.empty(maxWidth)
	ROILane = image[280:maxHeight, 0:maxWidth]
	#cv2.imshow("ROILANE", ROILane)    #use this to check ROI
	for i in range(maxWidth):
		array[i] = np.int32(np.sum(ROILane[:,i]))
	return array

def LaneFinder(array, image):
	#Here is where we scan both sides of the image(array) to find lane lines based on the histagram made earlier
	LeftPtr = 0
	RightPtr = 320
	for i in range(320):
		if (array[i] >= array[LeftPtr]): 
			LeftPtr = i
	
	j = 320;
	for j in range(640):
		if (array[j] >= array[RightPtr]): 
			RightPtr = j
	
	cv2.line(image, (LeftPtr, 0), (LeftPtr,480),(0,0,255), 3)
	cv2.line(image, (RightPtr, 0), (RightPtr, 480), (0,0,255), 3)
	cv2.imshow("", image)
	
Points = [(190, 160),(460, 160),(10, 360),(626, 360)] #points used for region of interest
Destination = [(120,0),(520,0),(120,480),(520,480)] #points used for top perspective

image = cv2.imread('image.jpg')
warped = Perspective(image, Points, Destination)
threshold = Threshold(warped)
array = Histogram(threshold)
LaneFinder(array, threshold)
#print(array)
#cv2.imshow("Box", array)
#cv2.imshow("Perspective", warped)
#cv2.imshow("Final", threshold)
cv2.waitKey(0)

