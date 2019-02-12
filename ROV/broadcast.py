from socket import *
import numpy as np
import netifaces as ni
from struct import *
import select
import subprocess
import cv2
import time
import ms5837py3 as ms5837
import RPi.GPIO as GPIO
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


def recvall(sock, n):
    data = b''
    while len(data) < n:
        packet = sock.recv(n - len(data))
        if not packet:
            return None
        data += packet
    return data


ip = ni.ifaddresses('eth0')[ni.AF_INET][0]['broadcast']
me = ni.ifaddresses('eth0')[ni.AF_INET][0]['addr']

broadcastPort = 42070
connPort = 42069

servoPIN = 16
GPIO.setmode(GPIO.BOARD)
GPIO.setup(servoPIN, GPIO.OUT)
p = GPIO.PWM(servoPIN, 50)
p.start(10 - 3.4 / 2 + 3.4)
done = False

connected = False
connAddr = ''
client = ''

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


sensor = ms5837.MS5837_30BA()

if not sensor.init():
    print("could not initialize sensor")

try:
    with socket(AF_INET, SOCK_STREAM) as conn:
        conn.bind(('', connPort))
        conn.setblocking(0)
        conn.listen(2)
        conn.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
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
                            print('connected to client')
                            client_socket, address = conn.accept()
                            connAddr = address
                            client = client_socket
                            connected = True

            while connected:
                inputready, outputready, other = select.select([client], [], [], 0)
                for s in inputready:
                    if s == client:
                        data = client.recv(1)
                        if data:
                            type = unpack('B', data)[0]
                            print(type)
                            if type == 0:
                                print('received ping packet')
                                d = pack('B', type)
                                client.send(d)
                                pingTime = time.time()
                            elif type == 254:
                                connected = False
                                print('255')
                                client.close()
                                print('shutting down')
                                subprocess.call(['shutdown', '-h', 'now'], shell=False)
                            elif type == 1:
                                print('staring video stream')

                                cap = cv2.VideoCapture(0)
                                resolution = set_res(cap, 1920, 1080)

                                frameRate = caluclate_framerate(cap)
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
                                raw = recvall(client, 4)
                                num = unpack('<f', raw)[0]
                                p.ChangeDutyCycle(num)
                            else:
                                print('unknown packet type')
                        else:
                            print('here')
                            client.close()
                            print('client disconnected')
                            connected = False

                if video and not noVid:
                    if cap.isOpened():
                        if time.time() - frametime >= secondsPerFrame:
                            ret, frame = cap.read()
                            if ret:
                                frametime = time.time()
                                result, frame = cv2.imencode('.jpg', frame, [int(cv2.IMWRITE_JPEG_QUALITY), 90])
                                data = frame.tobytes('C')
                                d = pack('<BI', 11, len(data)) + data
                                client.send(d)
                            else:
                                print('vid stopping')
                                video = False
                                break
                    else:
                        print('vid stopped because cap isnt open')
                        video = False
                if temperature:
                    if sensor.read():
                        val = sensor.temperature(ms5837.UNITS_Farenheit)
                        d = pack('<Bd', 12, val)
                        client.send(d)
                if pressure:
                    if sensor.read():
                        val = sensor.pressure(ms5837.UNITS_mbar)
                        d = pack('<Bd', 13, val)
                        client.send(d)
except (KeyboardInterrupt, OSError):
    p.stop()
    GPIO.cleanup()
