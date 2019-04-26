#include "PacketFactory.h"

void Factory::PacketFactory::add_type_to_packet(std::unique_ptr<sf::Packet> &p, Network::PacketTypes type) {
	*p << static_cast<unsigned char>(type);
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::createVideoPacket() {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(p, Network::PacketTypes::Count);
	return p;
}
