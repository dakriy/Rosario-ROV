#pragma once

#include "../Sensors/Sensor.h"

namespace Core {
	class Event
	{
	public:
		struct SensorsRequested
		{
			float frequency = 0.f;
			std::vector<sf::Uint8> sensors;
		};

		struct CameraMovement
		{
			float s1 = 0.f;
			float s2 = 0.f;
		};

		enum EventType
		{
			PingReceived,
			MissionStart,
			MissionStop,
			SensorRequest,
			Disconnected,
			StartCamera,
			StopCamera,
			CameraMove,
			Shutdown,

			Count
		};

		EventType type = Count;


		CameraMovement c;
		SensorsRequested r;

		explicit Event(EventType t) : type(t) {}
	};
}