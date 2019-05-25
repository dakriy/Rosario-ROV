#pragma once

#include <SFML/Network.hpp>

namespace Network {

	const unsigned short broadcastPort = 42070;
	const unsigned short connectionPort = 42069;
	const unsigned short broadcastInterval = 3000; // In milliseconds
	const unsigned short packetWaitTimeout = 1; // In milliseconds
	const char * const magicBroadcastString = "magic|Rosario ROV|end";

	enum class PacketTypes
	{
		Ping,
		MissionStart,
		MissionStop,
		RequestSensors,
		Sensors,
		Data,
		StartVideoStream,
		StopVideoStream,
		CameraMove,
		Video,
		TimeSync,
		Message,
		BatteryPercent,
		Shutdown = 254,
		Count
	};
}
