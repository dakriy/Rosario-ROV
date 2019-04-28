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
		Ping,
		MissionStart,
		MissionStop,
		Data,
		Move,
		Shutdown = 254,
		Count
	};
}
