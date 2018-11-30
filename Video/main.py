import cv2
import time

framerate = 25
fourcc = "DIVX"
file = "whee.avi"
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

print("Length should be:")
print (time.time() - start)

cap.release()
out.release()
cv2.destroyAllWindows()
