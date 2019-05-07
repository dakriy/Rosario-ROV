#include "Device.h"
#include "../Core/GlobalContext.h"
#include "../Factories/PacketFactory.h"
#include "Utilities.h"


Device::Device() {
	setHooks();
	GlobalContext::set_device(this);
	GlobalContext::get_engine()->addUpdateableEntitiy(this);
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
		m.showMissionWindow = false;
		m.stopMission();
		sensors.clear();
		m.clearSensors();
		return false;
	}, Core::Event::Disconnected));
}

Device::~Device() {
	GlobalContext::get_engine()->removeUpdateableEntity(this);
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

Mission & Device::getMission() {
	return m;
}

void Device::update(const sf::Time &) {
	// Connected window
	auto network = GlobalContext::get_network();
	if (network->isConnected())
	{
		std::string title = "Connected to ";
		title += std::get<std::string>(network->getConnectedHost());
		ImGui::Begin(title.c_str());
		ImGui::Text("Round Trip Ping: = %f ms", static_cast<float>(network->get_ping_time().asMicroseconds()) / 1000.f);
		ImGui::Checkbox("Mission Window", &m.showMissionWindow);
		ImGui::SameLine(); HelpMarker("Toggles the mission window");

		if (ImGui::Button("Disconnect"))
		{
			network->disconnect();
		}
		if (ImGui::Button("Shutdown ROV"))
		{
			network->send_packet(Factory::PacketFactory::create_shutdown_packet());
		}
		ImGui::End();
	}
}
