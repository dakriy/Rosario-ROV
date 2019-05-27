#include "Temperature.h"
#include "../Core/GlobalContext.h"

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
	float t = temp.temperature();
	auto ev = std::make_unique<Core::Event>(Core::Event::TemperatureTaken);
	ev->data = t;
	GlobalContext::get_engine()->add_event(std::move(ev));
	return t;
}
