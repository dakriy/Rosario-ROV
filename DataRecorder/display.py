#!/usr/bin/env python3

import ms5837py3 as ms5837
import time
import os

sensor = ms5837.MS5837_30BA() # Default I2C bus is 1 (Raspberry Pi 3)

f = open("/home/pi/Measurements/"+time.ctime()+".txt", 'w') 
# We must initialize the sensor before reading it
if not sensor.init():
        f.write("Sensor could not be initialized")
        exit(1)

sensor.setFluidDensity(ms5837.DENSITY_SALTWATER)
current = int(time.time())
# Print readings
while True:
    if sensor.read():
        print("%.1f,%.3f, %.2f, %.2f \n" % (                  # mbar, psi, C, F
        sensor.depth(), # Default is mbar (no arguments)
        sensor.pressure(ms5837.UNITS_psi), # Request psi
        sensor.temperature(), # Default is degrees C (no arguments)
        sensor.temperature(ms5837.UNITS_Farenheit))) # Request Farenheit
    else:
        exit(1)
    time.sleep(5)


