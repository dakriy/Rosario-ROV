#!/usr/bin/env python3

import cv2
from struct import *
import numpy as np
import scipy.misc as smp
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
    if ret:
        # img = cv2.cvtColor(frame, cv2.COLOR_RGB2RGBA)
        # cv2.imshow('dst_rt', img)
        # cv2.waitKey(0)
        # a = smp.toimage(img)  # Create a PIL image
        # a.show()  # View in default viewer
        #d = pack('BHH', 11, np.size(img, 0), np.size(img, 1)) + img.tobytes()
        #print(d)
        # conn.sendto(d, connAddr)

        result, frame = cv2.imencode('.jpg', frame, [int(cv2.IMWRITE_JPEG_QUALITY), 90])

        print(len(frame.tobytes('C')))
        #print(frame.tobytes('C'))

        cap.release()
    else:
        if not ret:
            break

cap.release()
