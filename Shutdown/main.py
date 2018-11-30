#!/usr/bin/python3
import RPi.GPIO as GPIO
import subprocess
GPIO.setmode(GPIO.BCM)
GPIO.setup(4, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.wait_for_edge(4, GPIO.FALLING)

subprocess.call(['shutdown', '-h', 'now'], shell=False)
