#include "InternalHumidity.h"
#include "lib/bme280.h"

const Sensor::SensorInfo &Sensor::InternalHumidity::getSensorInfo() {
	return info;
}

bool Sensor::InternalHumidity::setup() {
	return bme280::initialize();
}

void Sensor::InternalHumidity::initiateConversion() {
	bme280::getRawData();
}

float Sensor::InternalHumidity::queryDevice() {
	return bme280::compensateHumidity();
}
