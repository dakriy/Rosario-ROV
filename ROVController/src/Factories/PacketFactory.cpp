#include "PacketFactory.h"

void Factory::PacketFactory::add_type_to_packet(Core::PacketTypes type, std::unique_ptr<sf::Packet> &p) {
	*p << static_cast<sf::Uint8>(type);
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_shutdown_packet() {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(Core::PacketTypes::Shutdown, p);
	return p;
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_camera_move_packet(float theta, float r) {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(Core::PacketTypes::CameraMove, p);
	*p << theta << r;
	return p;
}

std::unique_ptr<sf::Packet>
Factory::PacketFactory::create_start_mission_packet(float frequency, const std::vector<sf::Uint8> &sensors,
													const std::string &rovRecordFileName) {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(Core::PacketTypes::MissionStart, p);
	auto sensorNumber = static_cast<sf::Uint32>(sensors.size());
	*p << frequency << sensorNumber;
	for (auto sensor : sensors) {
		*p << sensor;
	}
	*p << rovRecordFileName;
	return p;
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_sensor_request_packet() {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(Core::PacketTypes::RequestSensors, p);
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

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_light_packet(bool state, float percent) {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(Core::PacketTypes::LightUpdate, p);
	*p << state << percent;
	return p;
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_video_record_packet(bool record) {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(Core::PacketTypes::VideoRecord, p);
	*p << record;
	return p;
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_set_incoming_data_packet(bool sendData) {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(Core::PacketTypes::PauseDataPacket, p);
	*p << sendData;
	return p;
}
