#include "Conductivity.h"

const Sensor::SensorInfo &Sensor::Conductivity::getSensorInfo() {
	return info;
}

bool Sensor::Conductivity::setup() {
	auto s = conductivity.init();
//	conductivity.singleCalibrate(53065.f);
	return s;
}

void Sensor::Conductivity::initiateConversion() {
	conductivity.activate();
}

float Sensor::Conductivity::queryDevice() {
	return conductivity.getTDM();
}
