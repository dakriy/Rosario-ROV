#!/usr/bin/env python3

import cv2
import time

def caluclate_framerate(device):
    num_frames = 60

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


cap = cv2.VideoCapture(0)
resolution = set_res(cap, 1920, 1080)

framerate = caluclate_framerate(cap)
fourcc = "MJPG"

if not cap.isOpened():
    print("Unable to access camera")
    exit(1)

write = cv2.VideoWriter_fourcc(*fourcc)
out = cv2.VideoWriter(file, write, framerate, (1920, 1080))

while cap.isOpened():
    ret, frame = cap.read()
    if ret:
        out.write(frame)
    else:
        break

cap.release()
out.release()
cv2.destroyAllWindows()
