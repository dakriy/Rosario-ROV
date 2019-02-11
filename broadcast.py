from socket import *
import time
import netifaces as ni
from struct import *
import select

ip = ni.ifaddresses('eth0')[ni.AF_INET][0]['broadcast']
me = ni.ifaddresses('eth0')[ni.AF_INET][0]['addr']

broadcastPort = 42070
connPort = 42069

connected = False
connAddr = ''

pingTime = 0
sentTime = 0

with socket(AF_INET, SOCK_DGRAM) as conn:
    conn.bind(('', connPort))
    conn.setblocking(0)
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
                                conn.sendto(('connect').encode(), addr)
                            if data == 'connected':
                                print('connected to ' + str(addr))
                                connAddr = addr
                                connected = True
                                pingTime = time.time()
                        except BlockingIOError:
                            break
    while connected:
        if time.time() - pingTime > 15:
            connected = False
        inputready,outputready,other = select.select([conn], [], [], 0)
        for s in inputready:
            if s == conn:
                print('here')
                while True:
                    try:
                        data, addr = conn.recvfrom(65535)
                        type = unpack('B', data)[0]
                        if type == 0:
                            print('recieved ping packet')
                            d = pack('B', type)
                            conn.sendto(d, connAddr)
                            pingTime = time.time()
                            print('ping!')
                    except BlockingIOError:
                        break



