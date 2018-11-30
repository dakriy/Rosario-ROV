#!/usr/bin/python3
import RPi.GPIO as GPIO
from time import sleep

red_state = False
green_state = False

def toggle_red():
    if red_state:
        red_off()
    else:
        red_on()


def toggle_green():
    if green_state:
        green_off()
    else:
        green_on()


def red_off():
    global red_state
    with open('/sys/class/leds/led1/brightness', 'w') as f:
        f.write('0')
        red_state = False


def red_on():
    global red_state
    with open('/sys/class/leds/led1/brightness', 'w') as f:
        f.write('1')
        red_state = True


def green_off():
    global green_state
    with open('/sys/class/leds/led0/brightness', 'w') as f:
        f.write('0')
        green_state = False


def green_on():
    global green_state
    with open('/sys/class/leds/led0/brightness', 'w') as f:
        f.write('1')
        green_state = True


# starting out, off for 3 seconds
red_off()
green_off()
sleep(3)

GPIO.setmode(GPIO.BCM)
GPIO.setup(4, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.wait_for_edge(4, GPIO.FALLING)
print("You did it")
toggle_red()

