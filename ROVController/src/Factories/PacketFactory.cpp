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
	add_type_to_packet(Core::PacketTypes::CameraMove, p);
	*p << amount;
	return p;
}

std::unique_ptr<sf::Packet>
Factory::PacketFactory::create_request_data_packet(float frequency, std::vector<sf::Uint8> sensors) {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(Core::PacketTypes::MissionStart, p);
	*p << frequency;
	*p << sensors.size();
	for (auto sensor : sensors) {
		*p << sensor;
	}
	return p;
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_sensor_request_packet() {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(Core::PacketTypes::RequestSensors, p);
	return p;
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_start_mission_packet() {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(Core::PacketTypes::MissionStart, p);
	size_t num = 1;
	sf::Uint8 sensor = 0;
	float frequency = 1.f;
	*p << frequency << num << sensor;
	return p;
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_stop_mission_packet() {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(Core::PacketTypes::MissionStop, p);
	return p;
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_start_video_stream_packet() {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(Core::PacketTypes::StartVideoStream, p);
	return p;
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_stop_video_stream_packet() {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(Core::PacketTypes::StopVideoStream, p);
	return p;
}
