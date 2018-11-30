#!/usr/bin/env python3

import cv2
import time
import signal

class GracefulKiller:
    kill_now = False
    def __init(self):
        signal.signal(signal.SIGINT, self.exit_gracefully)
        signal.signal(signal.SIGTERM, self.exit_gracefully)
        signal.signal(signal.SIGKILL, self.exit_gracefully)

    def exit_gracefully(self, signum, frame):
        self.kill_now = True

start = time.time()
framerate = 25
fourcc = "DIVX"
cap = cv2.VideoCapture(0)
file = "/home/pi/Measurements/Video/zucc " + time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(start)) + ".avi"
resolution = (640, 480)

if cap.isOpened() == False:
    print("Unable to access camera")
    exit(1)

write = cv2.VideoWriter_fourcc(*fourcc)
out = cv2.VideoWriter(file, write, framerate, resolution)
killer = GracefulKiller()

while True:
    ret, frame = cap.read()
    if ret and not killer.kill_now:
        out.write(frame)
    else:
        break

cap.release()
out.release()
cv2.destroyAllWindows()
