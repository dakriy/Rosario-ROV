#pragma once

#include "../Sensors/Sensor.h"
#include <variant>

namespace Core {
	class Event
	{
	public:
		struct SensorsRequested
		{
			float frequency = 0.f;
			std::vector<sf::Uint8> sensors;
			std::string fileToRecord = "";
		};

		struct CameraMovement
		{
			float theta = 0.f;
			float radius = 0.f;
		};

		struct LightChangeDetails {
			float percent = 0.f;
			bool on = false;
		};

		enum EventType
		{
			NewConnection,			// No info
			PingReceived,			// No info
			MissionStart,			// Data in SensorsRequested
			MissionStop,			// No extra data
			SensorRequest,			// No extra data
			Disconnected,			// No extra data
			StartCamera,			// No extra data
			StopCamera,				// No extra data
			CameraMove,				// Data in CameraMovement
			LightChange,			// Data in LightChangeDetails
			VideoRecord,			// Data in bool
			Shutdown,				// No extra data
			TemperatureTaken,		// Data in float
			DataSendState,			// Data in bool, 0 means no send, 1 means send

			Count
		};

		EventType type = Count;

		std::variant<CameraMovement, SensorsRequested, LightChangeDetails, bool, float> data;

		explicit Event(EventType t) : type(t) {}
	};
}