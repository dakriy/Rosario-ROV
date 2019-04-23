#pragma once

#include <SFML/Network.hpp>

namespace Network {

	const unsigned short broadcastPort = 42070;
	const unsigned short connectionPort = 42069;
	const unsigned short broadcastInterval = 3000; // In milliseconds
	const unsigned short packetWaitTimeout = 100; // In milliseconds
	const char * const magicBroadcastString = "magic|Rosario ROV|end";

	enum class PacketTypes
	{
		Ping = 0,
		StartVideo = 1,
		StopVideo = 2,
		StartTemp = 3,
		StopTemp = 4,
		StartPressure = 5,
		StopPressure = 6,
		Move = 7,
		Video = 11,
		Temperature = 12,
		Pressure = 13,
		Shutdown = 254,
		Count
	};
}
