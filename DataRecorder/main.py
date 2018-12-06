#!/usr/bin/env python3

import ms5837py3 as ms5837
import time


sensor = ms5837.MS5837_30BA() # Default I2C bus is 1 (Raspberry Pi 3)

# We must initialize the sensor before reading it
if not sensor.init():
        print("Sensor could not be initialized")
        exit(1)

sensor.setFluidDensity(ms5837.DENSITY_SALTWATER)
current = int(time.time())
f = open("/home/pi/Measurements/"+time.ctime()+".txt", 'w') 
# Print readings
while True:
        next = int(time.time())
        if next == current+10*60:         #use the decimal to indicate how many minutes you want
            f = open("/home/pi/Measurements/"+time.ctime()+".txt", 'w')
            print("pulse")
            current = next
        if sensor.read():
            f.write("%.1f,%.3f, %.2f, %.2f \n" % (                  # mbar, psi, C, F
            sensor.pressure(), # Default is mbar (no arguments)
            sensor.pressure(ms5837.UNITS_psi), # Request psi
            sensor.temperature(), # Default is degrees C (no arguments)
            sensor.temperature(ms5837.UNITS_Farenheit))) # Request Farenheit
        else:
            print("Sensor read failed!")
            f.close()
            exit(1)


