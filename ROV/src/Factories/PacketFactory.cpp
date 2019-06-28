#include "PacketFactory.h"
#include "../Core/GlobalContext.h"
#include "../Utilities/Utilities.h"

void Factory::PacketFactory::add_type_to_packet(std::unique_ptr<sf::Packet> &p, Network::PacketTypes type) {
	*p << static_cast<sf::Uint8>(type);
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_video_packet(const std::vector<uint8_t>& buffer) {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(p, Network::PacketTypes::Video);
	// Add width, height, and type
	*p << static_cast<sf::Uint32>(buffer.size());
	// Add pixel data
	p->append(buffer.data(), buffer.size());
	return p;
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_sensor_list_packet(const std::vector<Sensor::SensorInfo>& sensors) {
	std::unique_ptr<sf::Packet> p = std::make_unique<sf::Packet>();
	add_type_to_packet(p, Network::PacketTypes::Sensors);
	*p << static_cast<sf::Uint32>(sensors.size());
	// Build packet of functioning sensors.
	for (auto& info : sensors) {
		*p << info.id << info.maxFrequency << info.name << info.units;
	}
	return p;
}

std::unique_ptr<sf::Packet>
Factory::PacketFactory::create_data_packet(const std::vector<std::pair<sf::Uint8, float>> &data) {
	auto p = std::make_unique<sf::Packet>();
	add_type_to_packet(p, Network::PacketTypes::Data);
	*p << GlobalContext::get_clock()->getElapsedTime().asMicroseconds();
	*p << static_cast<sf::Uint32>(data.size());
	for (const auto & dat : data) {
		*p << std::get<sf::Uint8>(dat);
		*p << std::get<float>(dat);
	}
	return p;
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_log_packet(const std::string& message) {
	auto p = std::make_unique<sf::Packet>();
	add_type_to_packet(p, Network::PacketTypes::Message);
	*p << message;
	return p;
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_battery_percentage_packet(float percent) {
	auto p = std::make_unique<sf::Packet>();
	add_type_to_packet(p, Network::PacketTypes::BatteryPercent);
	*p << percent;
	return p;
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_rov_state_packet(unsigned currentState) {
	auto p = std::make_unique<sf::Packet>();
	add_type_to_packet(p, Network::PacketTypes::ROVState);
	*p << currentState;
	return p;
}

std::unique_ptr<sf::Packet>
Factory::PacketFactory::create_mission_data_file_list_packet(const std::vector<std::string> & files) {
	auto p = std::make_unique<sf::Packet>();
	add_type_to_packet(p, Network::PacketTypes::MissionFileList);
	auto fileNumber = static_cast<sf::Uint16>(files.size());
	*p << fileNumber;
	for (auto & file : files) {
		*p << file;
	}
	return p;
}

std::unique_ptr<sf::Packet> Factory::PacketFactory::create_mission_data_file_packet(std::ifstream & file) {
	auto p = std::make_unique<sf::Packet>();
	add_type_to_packet(p, Network::PacketTypes::MissionFile);
	auto data = yeetFileIntoByteVector(file);
	auto size = static_cast<sf::Uint32>(data.size());
	*p << size;
	p->append(data.data(), data.size());
	return p;
}
