#pragma once

#include <SFML/Config.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <variant>
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

	enum class ROVState {
		Idle,
		Connected,
		ConnectedPaused,
		MissionConnected,
		MissionDisconnected,
		COUNT
	};

	class Event
	{
	public:
		enum EventType
		{
			NewConnection, 				// data::std::string, name of device
			PingReceived, 				// No info
			SensorInfoReceived,			// data::std::vector<SensorInfo>
			DataReceived,				// data::std::pair<sf::Time, std::vector<std::pair<sf::Uint8, float>>, contains all measurements
			VideoFrameReceived,			// data::std::vector<uint8_t>, contains jpeg data
			Disconnected,				// No info
			NewMessage,					// data::std::string, the message
			BatteryUpdate,				// data::float, the battery percentage
			ROVStateUpdate,				// data::ROVState, the updated rov state
			MissionFileListReceived,	// data::std::vector<std::string>
			MissionFileReceived,		// data::std::vector<uint8_t> contains the bytes of the file
			Count						// don't use, placeholder for things, also keep last.
		};

		EventType type = Count;

		std::variant<
		        std::vector<SensorInfo>,
		        std::pair<sf::Time, std::vector<std::pair<sf::Uint8, float>>>,
		        std::vector<uint8_t>,
		        std::string,
		        ROVState,
				std::vector<std::string>,
		        float> data;
		explicit Event(EventType t) : type(t){}
	};

}
