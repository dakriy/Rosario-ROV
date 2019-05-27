#include "Conductivity.h"
#include "../Core/GlobalContext.h"

Sensor::Conductivity::Conductivity() {
	tempTaken = GlobalContext::get_core_event_handler()->add_event_callback([&](const Core::Event * e) {
		float temp = std::get<float>(e->data);
		conductivity.setTemperature(temp);
		return true;
	}, Core::Event::TemperatureTaken);
}

const Sensor::SensorInfo &Sensor::Conductivity::getSensorInfo() {
	return info;
}

bool Sensor::Conductivity::setup() {
	return conductivity.init();
}

void Sensor::Conductivity::initiateConversion() {
	conductivity.activate();
}

float Sensor::Conductivity::queryDevice() {
	return conductivity.getPSS();
}

Sensor::Conductivity::~Conductivity() {
	GlobalContext::get_core_event_handler()->unhook_event_callback_for_all_events(tempTaken);
}
