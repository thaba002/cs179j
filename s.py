import cv2
import numpy as np
from scipy.stats import itemfreq

#codse based on code found here https://onedrive.live.com/?authkey=%21AA_r_7gtr5cqWFE&cid=2AAE05B692B45603&id=2AAE05B692B45603%21251895&parId=2AAE05B692B45603%21139667&o=OneUp

def get_dominant_color(img, n_colors):
    pixels = np.float32(img).reshape((-1, 3))
    criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 200, .1)
    flags = cv2.KMEANS_RANDOM_CENTERS
    flags, labels, centroids = cv2.kmeans(
        pixels, n_colors, None, criteria, 10, flags)
    palette = np.uint8(centroids)
    return palette[np.argmax(itemfreq(labels)[:, -1])]




cameraCapture = cv2.VideoCapture(0) 
cv2.namedWindow('camera')

success, frame = cameraCapture.read()

while success:
    cv2.waitKey(1)
    success, frame = cameraCapture.read()

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    img = cv2.medianBlur(gray, 37)
    circles = cv2.HoughCircles(img, cv2.HOUGH_GRADIENT,
                              1, 50, param1=120, param2=40)
    
    if not circles is None:
        circles = np.uint16(np.around(circles))
        max_r, max_i = 0, 0
        for i in range(len(circles[:, :, 2][0])):
            if circles[:, :, 2][0][i] > 50 and circles[:, :, 2][0][i] > max_r:
                max_i = i
                max_r = circles[:, :, 2][0][i]
        x, y, r = circles[:, :, :][0][max_i]
        if y > r and x > r:
            square = frame[y-r:y+r, x-r:x+r]

            dominant_color = get_dominant_color(square, 2)
            print(dominant_color)
            if dominant_color[2] > 200 and dominant_color[1] < 100 and dominant_color[0] < 100:
                print("STOP")
            elif dominant_color[1] > 150 and dominant_color[2] < 150 and dominant_color[0] < 150:
                print("Go")
            elif dominant_color[0] < 150 and dominant_color[1] > 150 and dominant_color[2] > 150:
                print("Slow")
            elif dominant_color[0] < 100 and dominant_color[1]  < 100 and dominant_color[2] < 100:
                print("SPEED_1")
            else:
                print("N/A")

        for i in circles[0, :]:
            cv2.circle(frame, (i[0], i[1]), i[2], (0, 255, 0), 2)
            cv2.circle(frame, (i[0], i[1]), 2, (0, 0, 255), 3)
    cv2.imshow('camera', frame)


cv2.destroyAllWindows()
cameraCapture.release()