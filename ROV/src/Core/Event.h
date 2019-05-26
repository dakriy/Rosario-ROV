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
			float theta = 0.f;
			float radius = 0.f;
		};

		struct LightChangeDetails {
			float percent = 0.f;
			bool on = false;
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
			LightChange,
			Shutdown,

			Count
		};

		EventType type = Count;


		CameraMovement c;
		SensorsRequested r;
		LightChangeDetails l;

		explicit Event(EventType t) : type(t) {}
	};
}