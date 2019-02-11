from socket import *
import numpy as np
import netifaces as ni
from struct import *
import select
import subprocess
import cv2
import time
import math


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


ip = ni.ifaddresses('eth0')[ni.AF_INET][0]['broadcast']
me = ni.ifaddresses('eth0')[ni.AF_INET][0]['addr']

broadcastPort = 42070
connPort = 42069

done = False

connected = False
connAddr = ''

pingTime = 0
sentTime = 0

cap = 0
resolution = 0

frameRate = 0
secondsPerFrame = 0
noVid = False

video = False
moveUp = False
moveDown = False
pressure = False
temperature = False

frametime = 0

with socket(AF_INET, SOCK_DGRAM) as conn:
    conn.bind(('', connPort))
    conn.setblocking(0)
    while not done:
        with socket(AF_INET, SOCK_DGRAM) as cs:
            cs.setblocking(0)
            cs.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
            cs.setsockopt(SOL_SOCKET, SO_BROADCAST, 1)
            while not connected:

                if time.time() - sentTime > 5:
                    cs.sendto(('magic|' + gethostname() + '|end').encode(), (ip, broadcastPort))
                    sentTime = time.time()

                inputready,outputready,other = select.select([conn], [], [], 0)
                for s in inputready:
                    if s == conn:
                        while True:
                            try:
                                data, addr = conn.recvfrom(65535)
                                data = data.decode().strip()
                                if data == 'connectRequest':
                                    print('sending reply')
                                    conn.sendto('connect'.encode(), addr)
                                if data == 'connected':
                                    print('connected to ' + str(addr))
                                    connAddr = addr
                                    connected = True
                                    pingTime = time.time()
                            except BlockingIOError:
                                break
        while connected:
            inputready, outputready, other = select.select([conn], [], [], 0)
            for s in inputready:
                if s == conn:
                    while True:
                        try:
                            data, addr = conn.recvfrom(65535)
                            type = unpack('B', data)[0]
                            if type == 0:
                                print('received ping packet')
                                d = pack('B', type)
                                conn.sendto(d, connAddr)
                                pingTime = time.time()
                            elif type == 255:
                                print('shutting down')
                                subprocess.call(['shutdown', '-h', 'now'], shell=False)
                            elif type == 1:
                                print('staring video stream')

                                cap = cv2.VideoCapture(0)
                                resolution = set_res(cap, 1920, 1080)
                                print(resolution)

                                # frameRate = caluclate_framerate(cap)
                                frameRate = 16
                                secondsPerFrame = 1 / frameRate

                                noVid = False
                                if not cap.isOpened():
                                    noVid = True
                                    print("Unable to access camera")

                                video = True
                            elif type == 2:
                                cap.release()
                                print('stopping video stream')
                                video = False
                            elif type == 3:
                                print('Starting temperature stream')
                                temperature = True
                            elif type == 4:
                                print('Stopping temperature stream')
                                temperature = False
                            elif type == 5:
                                print('Starting pressure stream')
                                pressure = True
                            elif type == 6:
                                print('Stopping pressure stream')
                                pressure = False
                            elif type == 7:
                                print('start move up')
                                moveUp = True
                            elif type == 8:
                                print('stop move up')
                                moveUp = False
                            elif type == 9:
                                print('start move down')
                                moveDown = True
                            elif type == 10:
                                print('stop move down')
                                moveDown = False
                            else:
                                print('unknown packet type')
                        except BlockingIOError:
                            break

            if video and not noVid:
                if cap.isOpened():
                    ret, frame = cap.read()
                    if ret and time.time() - frametime >= secondsPerFrame:
                        frametime = time.time()
                        img = cv2.cvtColor(frame, cv2.COLOR_RGB2RGBA)

                        width = np.size(img, 0)
                        height = np.size(img, 1)
                        totalSize = 4 * width * height

                        num = int(math.ceil(totalSize / 65400))

                        bytesAPacket = 65400

                        data = img.tobytes()

                        bytesSent = 0

                        for _ in range(num):
                            if num - 1 == _:
                                # send the rest of the data on the last one
                                dat = data[_ * bytesAPacket:]
                            else:
                                dat = data[_ * bytesAPacket:_ * bytesAPacket + bytesAPacket]
                                # Format is type width height bytesSent, runningtotalbytessent, totalpackets, packet
                            d = pack('BHHHIBB', 11, width, height, len(dat), bytesSent, num, _ + 1) + dat
                            sent = False
                            while not sent:
                                try:
                                    conn.sendto(d, connAddr)
                                    bytesSent += len(dat)
                                    sent = True
                                except BlockingIOError:
                                    pass
                    else:
                        if not ret:
                            video = False
                            break
                else:
                    video = False
                pass
            if temperature:
                pass
            if pressure:
                pass
            if moveUp:
                pass
            if moveDown:
                pass
            if time.time() - pingTime > 15:
                connected = False
