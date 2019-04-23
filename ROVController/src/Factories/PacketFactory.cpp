#include "PacketFactory.h"

void Factory::PacketFactory::add_type_to_packet(sf::Packet &p, Core::PacketTypes type) {
	p << static_cast<sf::Uint8>(type);
}

sf::Packet* Factory::PacketFactory::create_shutdown_packet() {
	auto p = new sf::Packet;
	add_type_to_packet(*p, Core::PacketTypes::Shutdown);
	return p;
}

sf::Packet* Factory::PacketFactory::create_camera_move_packet(float amount) {
	auto p = new sf::Packet;
	add_type_to_packet(*p, Core::PacketTypes::Move);
	*p << amount;
	return p;
}

sf::Packet* Factory::PacketFactory::create_video_stream_start_packet() {
	auto p = new sf::Packet;
	add_type_to_packet(*p, Core::PacketTypes::StartVideo);
	return p;
}

sf::Packet* Factory::PacketFactory::create_video_stream_stop_packet() {
	auto p = new sf::Packet;
	add_type_to_packet(*p, Core::PacketTypes::StopVideo);
	return p;
}

sf::Packet* Factory::PacketFactory::create_pressure_data_start_packet() {
	auto p = new sf::Packet;
	add_type_to_packet(*p, Core::PacketTypes::StartPressure);
	return p;
}

sf::Packet* Factory::PacketFactory::create_pressure_data_stop_packet() {
	auto p = new sf::Packet;
	add_type_to_packet(*p, Core::PacketTypes::StopPressure);
	return p;
}

sf::Packet* Factory::PacketFactory::create_temperature_data_start_packet() {
	auto p = new sf::Packet;
	add_type_to_packet(*p, Core::PacketTypes::StartTemp);
	return p;
}

sf::Packet* Factory::PacketFactory::create_temperature_data_stop_packet() {
	auto p = new sf::Packet;
	add_type_to_packet(*p, Core::PacketTypes::StopTemp);
	return p;
}
