#include "Mission.h"
#include "../Core/GlobalContext.h"
#include "Utilities.h"
#include "../Factories/PacketFactory.h"


Mission::Mission() {
	GlobalContext::get_engine()->addUpdateableEntitiy(this);

	hooks.push_back(GlobalContext::get_core_event_handler()->add_event_callback([this](const Core::Event *e)->bool {
		for (auto &sensorInfo : std::get<std::vector<Core::SensorInfo>>(e->data))
			sensorSelectMap.emplace_back(std::make_pair(sensorInfo, false));
		return false;
	}, Core::Event::SensorInfoReceived));

	hooks.push_back(GlobalContext::get_core_event_handler()->add_event_callback([&](const Core::Event *e)->bool {
		// I apologize for this madness....
		auto vals = std::get<std::pair<sf::Time, std::vector<std::pair<sf::Uint8, float>>>>(e->data);
		auto time = std::get<sf::Time>(vals);
		auto vec = std::get<std::vector<std::pair<sf::Uint8, float>>>(vals);

		std::vector<float> allVals;
		// size + 1 to account for the time column
		allVals.reserve(vec.size() + 1);
		allVals.emplace_back((time - startTime).asSeconds());
		for (const auto & set : vec) {
			allVals.emplace_back(std::get<float>(set));
		}

		// Doesn't necessarily clear currently allocated elements
		lastVals.clear();
		lastVals.reserve(vec.size());
		lastVals.insert(std::end(lastVals), std::begin(vec), std::end(vec));

		if (localData && csv) {
			std::vector<std::string> floatVals;
			floatVals.reserve(allVals.size());
			for (auto val : allVals) {
				floatVals.emplace_back(std::to_string(val));
			}
			csv->write_row(floatVals);
		}

		if (inProgress) {
			recordedData.emplace_back(allVals.begin(), allVals.end());
		}

		return false;
	}, Core::Event::DataReceived));

	hooks.push_back(GlobalContext::get_core_event_handler()->add_event_callback([&](const Core::Event *e)->bool {
		switch (std::get<Core::ROVState>(e->data)) {
			case Core::ROVState::Idle:
			case Core::ROVState::Connected:
				inProgress = false;
				break;
			case Core::ROVState::MissionConnected:
			case Core::ROVState::MissionDisconnected:
				inProgress = true;
				break;
			default:
				break;
		}
		return false;
	}, Core::Event::ROVStateUpdate));
}

Mission::~Mission() {
	GlobalContext::get_engine()->removeUpdateableEntity(this);
	for (auto hookIndex : hooks) {
		GlobalContext::get_core_event_handler()->unhook_event_callback_for_all_events(hookIndex);
	}
}

void Mission::update(const sf::Time &) {
	if (showMissionWindow) {
		ImGui::Begin("Mission", &showMissionWindow);

		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("Mission Settings", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Sensor Select"))
			{
				ImGui::Text("Select the sensors you want to record");
				maxFreq = 1000.f;
				for (auto & s : sensorSelectMap) {
					auto on = &std::get<bool>(s);
					auto sensor = std::get<Core::SensorInfo>(s);
					if (*on && sensor.maxFrequency < maxFreq)
						maxFreq = sensor.maxFrequency;
					ImGui::Checkbox(sensor.name.c_str(), on);
				}
				if (selectedFreq > maxFreq) {
					selectedFreq = maxFreq;
				}
				ImGui::SliderScalar("Record frequency",   ImGuiDataType_Float,  &selectedFreq, &minFreq, &maxFreq);
				ImGui::SameLine();
				HelpMarker("Drag or control click to manual enter value. It gets clamped to your slowest selected sensor.");

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Record Settings"))
			{
				ImGui::Text("Select the wanted record settings");

				ImGui::Checkbox("Record Local Data", &localData);
				ImGui::Checkbox("Record Local Video (Not yet Implemented)", &localVideo);
				ImGui::Checkbox("Record Remote Data", &remoteData);
				ImGui::Checkbox("Record Remote Video (Not yet Implemented)", &remoteVideo);

				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::Separator();
		ImGui::Text("Enter the file name for output csv.\n"
						 "If remote data recording is selected, the same filename will be used on the ROV");
		ImGui::InputText("File Name",
				   fileName, nameBuffSize);

		if (!inProgress) {
			if (ImGui::Button("START")) {
				startMission();
			}
		} else {
			if (ImGui::Button("STOP")) {
				stopMission();
			}
		}

		ImGui::End();
	}
}

void Mission::clearSensors()
{
	sensorSelectMap.clear();
}

void Mission::startMission() {
	if (localData || remoteData) {
		if (strcmp(fileName, "") == 0) {
			strcpy(fileName, "default.csv");
		}
	}

	recordedDataNames.clear();
	recordedDataNames.emplace_back("Time (s)");

	if (localData) {
		csv = std::make_unique<csv::Writer>(fileName);
		csv->configure_dialect().column_names("Time (s)");
	}

	for (auto & s : sensorSelectMap) {
		auto str = std::get<Core::SensorInfo>(s).name + ' ' + '(' + std::get<Core::SensorInfo>(s).units + ')';
		if (std::get<bool>(s)) {
			selectedSensorIdList.emplace_back(std::get<Core::SensorInfo>(s).id);
			recordedDataNames.emplace_back(str);
			if (localData) {
				csv->configure_dialect().column_names(str);
			}
		}
	}

	if (localData) {
		csv->write_header();
	}

	if (!selectedSensorIdList.empty()) {
		if (remoteData) {
			GlobalContext::get_network()->send_packet(Factory::PacketFactory::create_start_mission_packet(selectedFreq, selectedSensorIdList, fileName));
		} else {
			GlobalContext::get_network()->send_packet(Factory::PacketFactory::create_start_mission_packet(selectedFreq, selectedSensorIdList));
		}
	}

	startTime = GlobalContext::get_clock()->getElapsedTime();
	inProgress = true;
}

void Mission::stopMission() {
	if (inProgress)
		GlobalContext::get_network()->send_packet(Factory::PacketFactory::create_stop_mission_packet());

	if (csv) {
		csv->close();
		csv.reset();
	}

	selectedSensorIdList.clear();
	lastVals.clear();

	inProgress = false;
}

std::vector<std::vector<float>> & Mission::getData()
{
	return recordedData;
}

std::vector<std::string> &Mission::getDataCols() {
	return recordedDataNames;
}

float Mission::getLastValForSens(sf::Uint8 id) {
	for (auto & pair : lastVals) {
		if (std::get<sf::Uint8>(pair) == id) {
			return std::get<float>(pair);
		}
	}
	return 0.f;
}

std::string Mission::getUnitsForSens(sf::Uint8 id) {
	for (auto & s : sensorSelectMap) {
		if (std::get<Core::SensorInfo>(s).id == id) {
			return std::get<Core::SensorInfo>(s).units;
		}
	}
	return "";
}
