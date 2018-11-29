import ms5837py3 as ms5837
import tsys01
import time

sensor = ms5837.MS5837_30BA() # Default I2C bus is 1 (Raspberry Pi 3)

# We must initialize the sensor before reading it
if not sensor.init():
        print("Sensor could not be initialized")
        exit(1)

sensor.setFluidDensity(ms5837.DENSITY_SALTWATER)

# Print readings
while True:
        if sensor.read():
                print("P: %.1f mbar  %.3f psi\tT: %.2f C  %.2f F" % (
                    sensor.pressure(), # Default is mbar (no arguments)
                sensor.pressure(ms5837.UNITS_psi), # Request psi
                sensor.temperature(), # Default is degrees C (no arguments)
                sensor.temperature(ms5837.UNITS_Farenheit))) # Request Farenheit
        else:
                print("Sensor read failed!")
                exit(1)
