#include "PacketFactory.h"

void Factory::PacketFactory::add_type_to_packet(std::unique_ptr<sf::Packet> &p, Network::PacketTypes type) {
	*p << static_cast<sf::Uint8>(type);
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::createVideoPacket() {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(p, Network::PacketTypes::Count);
	return p;
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_sensor_list_packet(std::vector<Sensor::SensorInfo>& sensors) {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(p, Network::PacketTypes::Sensors);
	*p << static_cast<sf::Uint32>(sensors.size());
	// Build packet of functioning sensors.
	for (auto& info : sensors) {
		*p << info.id << info.maxFrequency << info.name << info.units;
	}
	return p;
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_data_packet(std::vector<float>& data) {
	auto p = std::make_unique<sf::Packet>();
	add_type_to_packet(p, Network::PacketTypes::Data);
	*p << static_cast<sf::Uint32>(data.size());
	for (auto & dat : data) {
		*p << dat;
	}
	return p;
}
