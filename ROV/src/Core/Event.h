#pragma once

#include "../Sensors/Sensor.h"

namespace Core {
	class Event
	{
	public:
		struct SensorsRequested
		{
			float frequency = 0.f;
			std::vector<sf::Uint8> sensors = {};
		};

		enum EventType
		{
			PingReceived,
			DataRequested,
			Disconnected,
			Shutdown,

			Count
		};

		EventType type = Count;

		union
		{
			SensorsRequested r;
		};

		explicit Event(EventType t) : type(t) {}
	};
}