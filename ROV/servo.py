#!/usr/bin/env python3

import RPi.GPIO as GPIO
import time

servoPIN = 16


GPIO.setmode(GPIO.BOARD)
GPIO.setup(servoPIN, GPIO.OUT)
p = GPIO.PWM(servoPIN, 50)
# 3.4 left
# 10 right
p.start(3.4)
input("enter to stop")

p.stop()
GPIO.cleanup()


