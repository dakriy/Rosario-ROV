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

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_video_stream_start_packet() {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(Core::PacketTypes::StartVideo, p);
	return p;
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_video_stream_stop_packet() {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(Core::PacketTypes::StopVideo, p);
	return p;
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_pressure_data_start_packet() {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(Core::PacketTypes::StartPressure, p);
	return p;
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_pressure_data_stop_packet() {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(Core::PacketTypes::StopPressure, p);
	return p;
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_temperature_data_start_packet() {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(Core::PacketTypes::StartTemp, p);
	return p;
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_temperature_data_stop_packet() {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(Core::PacketTypes::StopTemp, p);
	return p;
}
