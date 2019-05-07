#pragma once

#include <vector>
#include "../Globals.h"
#include "../Core/Engine.h"
#include "Mission.h"

class Device : public Interfaces::IUpdateable {
protected:
	std::vector<CORE_EVENT_FUNC_INDEX> hooks;

	std::vector<Core::SensorInfo> sensors;

	bool connected;

	std::string name;

	void setHooks();

	Mission m;
public:
	Device();
	const std::vector<Core::SensorInfo>& getSensorList();
	Mission & getMission();
	bool isConnected();
	const std::string& getName();
	void update(const sf::Time&) override;
	~Device() override;
};
