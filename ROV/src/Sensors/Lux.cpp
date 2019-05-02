#include "Lux.h"

const Sensor::SensorInfo &Sensor::Lux::getSensorInfo() {
	return info;
}

bool Sensor::Lux::setup() {
	lightSensor.begin();
}

void Sensor::Lux::initiateConversion() {
	lightSensor.startConversion();
}

float Sensor::Lux::queryDevice() {
	return lightSensor.getLight();
}
