#include "PacketFactory.h"

void Factory::PacketFactory::add_type_to_packet(sf::Packet &p, Network::PacketTypes type) {
	p << static_cast<unsigned char>(type);
}

sf::Packet *Factory::PacketFactory::createVideoPacket() {
	auto p = new sf::Packet;
	add_type_to_packet(*p, Network::PacketTypes::StopTemp);
	return p;
}
