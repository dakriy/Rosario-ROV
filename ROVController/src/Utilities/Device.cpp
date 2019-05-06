#include "Device.h"
#include "../Core/GlobalContext.h"
#include "../Factories/PacketFactory.h"


Device::Device() {
	setHooks();
	GlobalContext::set_device(this);
}

void Device::setHooks() {
	// New Connection hook
	hooks.push_back(GlobalContext::get_core_event_handler()->add_event_callback([this](const Core::Event *e)->bool {
		// Ask device for all the information

		name = std::get<std::string>(e->data);
		connected = true;

		auto network = GlobalContext::get_network();

		// Get Sensors
		network->send_packet(Factory::PacketFactory::create_sensor_request_packet());

		// Synchronize time


		return false;
	}, Core::Event::NewConnection));

	// Sensor Information Hook
	hooks.push_back(GlobalContext::get_core_event_handler()->add_event_callback([this](const Core::Event *e)->bool {
		// Copy over the sensor info
		// Yes we want a copy here
		sensors = std::get<std::vector<Core::SensorInfo>>(e->data);
		return false;
	}, Core::Event::SensorInfoReceived));

	hooks.push_back(GlobalContext::get_core_event_handler()->add_event_callback([this](const Core::Event *e)->bool {
		connected = false;
		return false;
	}, Core::Event::Disconnected));
}

Device::~Device() {
	for (auto & hook : hooks) {
		GlobalContext::get_core_event_handler()->unhook_event_callback_for_all_events(hook);
	}
	GlobalContext::clear_device();
}

const std::vector<Core::SensorInfo> &Device::getSensorList() {
	return sensors;
}

const std::string &Device::getName() {
	return name;
}

bool Device::isConnected() {
	return connected;
}
