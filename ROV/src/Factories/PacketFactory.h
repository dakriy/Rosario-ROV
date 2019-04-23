#pragma once

#include <SFML/Network.hpp>
#include "../Network/Network.h"

namespace Factory {
	class PacketFactory {
	protected:
		static void add_type_to_packet(sf::Packet &p, Network::PacketTypes type);
	public:
		static sf::Packet* createVideoPacket();
	};
}
