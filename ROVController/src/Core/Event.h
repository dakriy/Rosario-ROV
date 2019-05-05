#pragma once

#include <SFML/Config.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <unordered_map>
#include <opencv2/opencv.hpp>

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
			VideoFrameReceived,
			Count
		};

		EventType type = Count;

		// Turn this into union when we get more things.
//		union {
//			std::vector<SensorInfo> sInfo;
//			std::vector<float> data;
//		};

		std::vector<SensorInfo> sInfo;
		std::vector<float> data;
		std::vector<uint8_t> imgData;
		explicit Event(EventType t) : type(t){}
	};

}
