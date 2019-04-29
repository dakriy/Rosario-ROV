#pragma once

#include <SFML/Config.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <unordered_map>

namespace Core {
	struct SensorInfo {
		sf::Uint8 id = 255;
		float maxFrequency = 0.f;
		std::string name;
		std::string units;
		SensorInfo(
				sf::Uint8 id,
		float maxFrequency,
		std::string name,
		std::string units) : id(id), maxFrequency(maxFrequency), name(std::move(name)), units(std::move(units)) {}
		SensorInfo() = default;
    };

	class Event
	{
	public:
		enum EventType
		{
			PingReceived,
			SensorInfoReceived,
			DataReceived,
			Count
		};

		EventType type = Count;

		// Turn this into union when we get more things.
//		union {
//		};

        std::vector<SensorInfo> sInfo;
		explicit Event(EventType t) : type(t){}
	};

}
