#include "Battery.h"

float Sensor::Battery::queryDevice() {
	return ADC.getConversion();
}

bool Sensor::Battery::setup() {
	return ADC.init();
}

void Sensor::Battery::initiateConversion() {
	// Do nothing as a conversion gets initiated as soon as you get a value from it
}

const Sensor::SensorInfo &Sensor::Battery::getSensorInfo() {
	return info;
}
