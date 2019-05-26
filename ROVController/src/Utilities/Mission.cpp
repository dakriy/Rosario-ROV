#include "Mission.h"
#include "../Core/GlobalContext.h"
#include "Utilities.h"
#include "../Factories/PacketFactory.h"


Mission::Mission() {
	GlobalContext::get_engine()->addUpdateableEntitiy(this);
	sensorHook = GlobalContext::get_core_event_handler()->add_event_callback([this](const Core::Event *e)->bool {
		for (auto &sensorInfo : std::get<std::vector<Core::SensorInfo>>(e->data))
			sensorSelect.emplace_back(std::make_pair(sensorInfo, false));
		return false;
	}, Core::Event::SensorInfoReceived);

	dataHook = GlobalContext::get_core_event_handler()->add_event_callback([&](const Core::Event *e)->bool {
		auto vals = std::get<std::pair<sf::Time, std::vector<float>>>(e->data);
		auto time = std::get<sf::Time>(vals);
		auto vec = std::get<std::vector<float>>(vals);

		std::vector<float> allVals;
		allVals.reserve(vec.size() + 1);
		allVals.emplace_back((time - startTime).asSeconds());
		allVals.insert(std::end(allVals), std::begin(vec), std::end(vec));

		lastVals.clear();
		lastVals.reserve(vec.size());
		for (auto [i, thing] : enumerate(vec)) {
			if (i < sens.size()) {
				lastVals.emplace_back(std::make_pair(sens[i], thing));
			}
		}

		if (localData && csv) {
			std::vector<std::string> floatVals;
			floatVals.reserve(allVals.size());
			for (auto val : allVals) {
				floatVals.emplace_back(std::to_string(val));
			}
			csv->write_row(floatVals);
		}

		recordedData.emplace_back(allVals.begin(), allVals.end());

		return false;
	}, Core::Event::DataReceived);
}

Mission::~Mission() {
	GlobalContext::get_engine()->removeUpdateableEntity(this);
	GlobalContext::get_core_event_handler()->unhook_event_callback_for_all_events(sensorHook);
	GlobalContext::get_core_event_handler()->unhook_event_callback_for_all_events(dataHook);
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
				for (auto & s : sensorSelect) {
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
				ImGui::Checkbox("Record Local Video", &localVideo);
				ImGui::Checkbox("Record Remote Data", &remoteData);
				ImGui::Checkbox("Record Remote Video", &remoteVideo);

				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::Separator();

		ImGui::InputText("Enter the file name for output csv", fileName, nameBuffSize);

		if (!inProgress) {
			if (ImGui::Button("Start Mission")) {
				startMission();
			}
		} else {
			if (ImGui::Button("Stop Mission")) {
				stopMission();
			}
		}

		ImGui::End();
	}
}

void Mission::clearSensors()
{
	sensorSelect.clear();
}

void Mission::startMission() {
	if (localData) {
		if (strcmp(fileName, "") == 0) {
			strcpy(fileName, "default.csv");
		}
		csv = std::make_unique<csv::Writer>(fileName);
	}

	if (localData) {
		csv->configure_dialect().column_names("Time (s)");
	}
	recordedDataNames.clear();
	recordedDataNames.emplace_back("Time (s)");

	for (auto & s : sensorSelect) {
		auto str = std::get<Core::SensorInfo>(s).name + ' ' + '(' + std::get<Core::SensorInfo>(s).units + ')';
		if (std::get<bool>(s)) {
			sens.emplace_back(std::get<Core::SensorInfo>(s).id);
			recordedDataNames.emplace_back(str);
			if (localData) {
				csv->configure_dialect().column_names(str);
			}
		}
	}

	if (localData) {
		csv->write_header();
	}

	if (!sens.empty()) {
		GlobalContext::get_network()->send_packet(Factory::PacketFactory::create_start_mission_packet(selectedFreq, sens));
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

	sens.clear();
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
	for (auto & s : sensorSelect) {
		if (std::get<Core::SensorInfo>(s).id == id) {
			return std::get<Core::SensorInfo>(s).units;
		}
	}
	return "";
}
