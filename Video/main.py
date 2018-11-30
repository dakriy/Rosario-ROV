#!/usr/bin/env python3

import cv2
import time

framerate = 25
fourcc = "DIVX"
file = "/home/pi/Measurements/Video/zucc.avi"
cap = cv2.VideoCapture(0)
resolution = (640, 480)

if cap.isOpened() == False:
    print("Unable to access camera")
    exit(1)

write = cv2.VideoWriter_fourcc(*fourcc)
out = cv2.VideoWriter(file, write, framerate, resolution)
start = time.time()

try:
    while True:
        ret, frame = cap.read()
        if ret:
            out.write(frame)
        else:
            break
except KeyboardInterrupt:
    pass

cap.release()
out.release()
cv2.destroyAllWindows()
