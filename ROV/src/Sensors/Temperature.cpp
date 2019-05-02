#include "Temperature.h"

const Sensor::SensorInfo &Sensor::Temperature::getSensorInfo() {
	return info;
}

bool Sensor::Temperature::setup() {
	return temp.init();
}

void Sensor::Temperature::initiateConversion() {
	return temp.read();
}

float Sensor::Temperature::queryDevice() {
	return temp.temperature();
}
