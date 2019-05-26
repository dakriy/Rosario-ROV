#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/Config.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <unordered_map>

namespace Sensor {

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

	struct SensorInfo {
		sf::Uint8 id;
		float maxFrequency;
		std::string name;
		std::string units;
	};

	class Sensor {
	public:
		virtual bool setup() = 0;
		virtual const SensorInfo& getSensorInfo() = 0;
		virtual void initiateConversion() = 0;
		virtual float queryDevice() = 0;
	};
}