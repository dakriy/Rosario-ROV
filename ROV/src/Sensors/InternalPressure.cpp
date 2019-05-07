#include "InternalPressure.h"
#include "lib/bme280.h"

const Sensor::SensorInfo &Sensor::InternalPressure::getSensorInfo() {
	return info;
}

bool Sensor::InternalPressure::setup() {
	return bme280::initialize();
}

void Sensor::InternalPressure::initiateConversion() {
	bme280::getRawData();
}

float Sensor::InternalPressure::queryDevice() {
	// Convert to mbar
	return bme280::compensatePressure() / 100.f;
}
