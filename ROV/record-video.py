#!/usr/bin/env python3

import cv2
import time
import math

def caluclate_framerate(device):
    num_frames = 120

    # Start time
    start = time.time()
    # Grab a few frames
    for i in range(num_frames):
        ret, frame = device.read()
    # End time
    end = time.time()

    # Time elapsed
    seconds = end - start

    # Calculate frames per second
    fps = num_frames / seconds
    return fps


def set_res(dev, x, y):
    dev.set(cv2.CAP_PROP_FRAME_WIDTH, int(x))
    dev.set(cv2.CAP_PROP_FRAME_HEIGHT, int(y))
    return str(cap.get(cv2.CAP_PROP_FRAME_WIDTH)), str(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))

start = time.time()

cap = cv2.VideoCapture(0)
resolution = set_res(cap, 1920, 1080)

#framerate = math.floor(caluclate_framerate(cap))
framerate = 16
fourcc = "MJPG"
file = 'hi.avi'

if not cap.isOpened():
    print("Unable to access camera")
    exit(1)

write = cv2.VideoWriter_fourcc(*fourcc)
out = cv2.VideoWriter(file, write, framerate, (1920, 1080))

while cap.isOpened():
    ret, frame = cap.read()
    if ret:
        out.write(frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    else:
        break

cap.release()
out.release()
cv2.destroyAllWindows()
