#pragma once

#include <vector>
#include "../Globals.h"
#include "../Core/Engine.h"

class Device {
protected:
	std::vector<CORE_EVENT_FUNC_INDEX> hooks;

	std::vector<Core::SensorInfo> sensors;

	bool connected;

	std::string name;

	void setHooks();
public:
	Device();
	const std::vector<Core::SensorInfo>& getSensorList();
	bool isConnected();
	const std::string& getName();
	~Device();
};
