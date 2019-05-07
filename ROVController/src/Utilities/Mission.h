#pragma once

#include <csv/writer.hpp>
#include "../Interfaces/IUpdateable.h"
#include "../Globals.h"
#include "../Core/Engine.h"
#include "../Core/Event.h"

class Mission : public Interfaces::IUpdateable {
protected:
	std::vector<std::pair<Core::SensorInfo, bool>> sensorSelect;
	CORE_EVENT_FUNC_INDEX sensorHook;
	CORE_EVENT_FUNC_INDEX dataHook;
	float selectedFreq = 1.f;
	float maxFreq = 1000.f;
	float minFreq = 1.f / 20.f; // One measurement every 20 seconds

	static const unsigned nameBuffSize = 64;
	char fileName[nameBuffSize] = "";


	bool localVideo = false, localData = true, remoteVideo = false, remoteData = false, inProgress = false;

	std::vector<std::vector<float>> recordedData;

	std::unique_ptr<csv::Writer> csv;

	sf::Time startTime;
public:
	bool showMissionWindow = false;

	void startMission();

	void stopMission();

	Mission();
	void update(const sf::Time&) override;
	~Mission() override;
};

