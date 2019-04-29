#include "Pressure.h"

const Sensor::SensorInfo &Sensor::Pressure::getSensorInfo() {
	return info;
}

bool Sensor::Pressure::setup() {
	return pressure.init();
}

void Sensor::Pressure::initiateConversion() {
	pressure.read();
}

float Sensor::Pressure::queryDevice() {
	return pressure.pressure();
}
