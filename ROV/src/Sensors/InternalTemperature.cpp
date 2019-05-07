#include "InternalTemperature.h"
#include "lib/bme280.h"
#include <wiringPiI2C.h>

const Sensor::SensorInfo &Sensor::InternalTemperature::getSensorInfo() {
	return info;
}

bool Sensor::InternalTemperature::setup() {
	return bme280::initialize();
}

void Sensor::InternalTemperature::initiateConversion() {
	bme280::getRawData();
}

float Sensor::InternalTemperature::queryDevice() {
	return bme280::compensateTemperature();
}
