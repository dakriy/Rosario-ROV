#include "PacketFactory.h"

void Factory::PacketFactory::add_type_to_packet(Core::PacketTypes type, std::unique_ptr<sf::Packet> &p) {
	*p << static_cast<sf::Uint8>(type);
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_shutdown_packet() {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(Core::PacketTypes::Shutdown, p);
	return p;
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_camera_move_packet(float amount) {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(Core::PacketTypes::Move, p);
	*p << amount;
	return p;
}

std::unique_ptr<sf::Packet>
Factory::PacketFactory::create_request_data_packet(float frequency, std::vector<Core::SensorInfo::Sensor> sensors) {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(Core::PacketTypes::MissionStart, p);
	*p << frequency;
	*p << sensors.size();
	for (const auto & sensor : sensors) {
		*p << static_cast<unsigned>(sensor);
	}
	return p;
}
