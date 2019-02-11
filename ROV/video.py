#!/usr/bin/env python3

import cv2
import numpy as np
from struct import *
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


frametime = 0
cap = cv2.VideoCapture(0)
resolution = set_res(cap, 1920, 1080)

#frameRate = caluclate_framerate(cap)
frameRate = 16
secondsPerFrame = 1 / frameRate


if not cap.isOpened():
    print("Unable to access camera")
    exit()


while cap.isOpened():
    ret, frame = cap.read()
    if ret and time.time() - frametime >= secondsPerFrame:
        frametime = time.time()
        img = cv2.cvtColor(frame, cv2.COLOR_RGB2RGBA)
        d = pack('BHH', 11, np.size(img, 0), np.size(img, 1)) + img.tobytes()
        print(d)
        # conn.sendto(d, connAddr)
    else:
        if not ret:
            break

cap.release()
