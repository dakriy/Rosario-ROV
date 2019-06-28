#include "Device.h"
#include "../Core/GlobalContext.h"
#include "../Factories/PacketFactory.h"
#include "Utilities.h"
#include <fstream>


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
		network->send_packet(Factory::PacketFactory::create_mission_file_list_request_packet());

		return false;
	}, Core::Event::NewConnection));

	// Sensor Information Hook
	hooks.push_back(GlobalContext::get_core_event_handler()->add_event_callback([this](const Core::Event *e)->bool {
		// Copy over the sensor info
		// Yes we want a copy here
		// This is so that we can dynamically get sensor information later
		// instead of having to pull the last values off of the mission
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

	hooks.push_back(GlobalContext::get_core_event_handler()->add_event_callback([this](const Core::Event *e)->bool {
		missionFilesOnROVString = std::get<std::vector<std::string>>(e->data);
		missionFilesOnROV.clear();
		missionFilesOnROV.reserve(missionFilesOnROVString.size());
		for (auto & string : missionFilesOnROVString) {
			missionFilesOnROV.push_back(string.c_str());
		}
		return true;
	}, Core::Event::MissionFileListReceived));

	hooks.push_back(GlobalContext::get_core_event_handler()->add_event_callback([this](const Core::Event *e)->bool {
		auto & data = std::get<std::vector<uint8_t>>(e->data);
		std::ofstream outfile(missionFilesOnROVString[selectedFile], std::ios::out | std::ios::binary);
		// THIS MEMORY IS WHATEVER I SAY IT IS MUHAHAHAHA
		outfile.write(reinterpret_cast<const char *>(data.data()), data.size());
		finishedDownload = true;
		return true;
	}, Core::Event::MissionFileReceived));
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
		ImGui::Checkbox("Filesystem Management Window", &showFileSystemManagementWindow);
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


		ImGui::Begin("Live Sensor Data");
		ImGui::Checkbox("Get live data when no mission is in progress.", &updateWhenNoMission);
		ImGui::Text("Pressure: = %f (%s)", m.getLastValForSens(static_cast<sf::Uint8>(SensorId::Pressure)), m.getUnitsForSens(static_cast<sf::Uint8>(SensorId::Pressure)).c_str());
		ImGui::Text("Temperature: = %f (%s)", m.getLastValForSens(static_cast<sf::Uint8>(SensorId::Temperature)), m.getUnitsForSens(static_cast<sf::Uint8>(SensorId::Temperature)).c_str());
		ImGui::Text("Conductivity: = %f (%s)", m.getLastValForSens(static_cast<sf::Uint8>(SensorId::Conductivity)), m.getUnitsForSens(static_cast<sf::Uint8>(SensorId::Conductivity)).c_str());
		ImGui::End();

		if (updateWhenNoMission != updateWhenNoMissionLast) {
			network->send_packet(Factory::PacketFactory::create_set_incoming_data_packet(updateWhenNoMission));
			updateWhenNoMissionLast = updateWhenNoMission;
		}

		if (showFileSystemManagementWindowLast != showFileSystemManagementWindow && showFileSystemManagementWindow) {
			network->send_packet(Factory::PacketFactory::create_mission_file_list_request_packet());
			showFileSystemManagementWindowLast = showFileSystemManagementWindow;
		}

		if (showFileSystemManagementWindow) {
			ImGui::Begin("File System Manager");

			ImGui::Combo("Mission Files on ROV", &selectedFile, missionFilesOnROV.data(), missionFilesOnROV.size());
			if (ImGui::Button("Download") && !missionFilesOnROV.empty())
			{
				finishedDownload = false;
				network->send_packet(Factory::PacketFactory::create_mission_file_request_packet(missionFilesOnROVString[selectedFile]));
			}
			if (finishedDownload) {
				ImGui::Text("Finished download");
			}
			if (ImGui::Button("Delete ROV Mission Files"))
			{
				network->send_packet(Factory::PacketFactory::create_delete_mission_files_packet());
				missionFilesOnROVString.clear();
				missionFilesOnROV.clear();
			}

			ImGui::End();
		}
	}
}
