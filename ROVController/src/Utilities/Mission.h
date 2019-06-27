#pragma once

#include <csv/writer.hpp>
#include "../Interfaces/IUpdateable.h"
#include "../Globals.h"
#include "../Core/Engine.h"
#include "../Core/Event.h"
#include <string>
#include <vector>

enum class SensorId {
	Temperature,
	Pressure,
	Lux,
	InternalTemperature,
	InternalPressure,
	InternalHumidity,
	Battery,
	Conductivity,
	Count
};

class Mission : public Interfaces::IUpdateable {
protected:
	std::vector<std::pair<Core::SensorInfo, bool>> sensorSelectMap;
	std::vector<CORE_EVENT_FUNC_INDEX> hooks;
	float selectedFreq = 1.f;
	float maxFreq = 1000.f;
	float minFreq = 1.f / 20.f; // One measurement every 20 seconds

	static const unsigned nameBuffSize = 64;
	char fileName[nameBuffSize] = "";


	bool localVideo = false, localData = true, remoteVideo = false, remoteData = false, inProgress = false;

	std::vector<std::string> recordedDataNames;
	std::vector<std::vector<float>> recordedData;
	std::vector<sf::Uint8> selectedSensorIdList;
	std::vector<std::pair<sf::Uint8, float>> lastVals;

	std::unique_ptr<csv::Writer> csv;

	sf::Time startTime;
public:
	bool showMissionWindow = false;

	void startMission();

	void stopMission();

	std::vector<std::vector<float>>& getData();
	std::vector<std::string>& getDataCols();

	float getLastValForSens(sf::Uint8 id);

	std::string getUnitsForSens(sf::Uint8 id);

	Mission();
	void update(const sf::Time&) override;
	void clearSensors();
	~Mission() override;
};

