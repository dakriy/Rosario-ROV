#pragma once

#include <SFML/Network.hpp>
#include "../Network/Network.h"
#include <memory>
#include <opencv2/opencv.hpp>
#include <string>
#include <fstream>

namespace Factory {
	class PacketFactory {
	protected:
		static void add_type_to_packet(std::unique_ptr<sf::Packet> &p, Network::PacketTypes type);
	public:
		static std::unique_ptr<sf::Packet> create_video_packet(const std::vector<uint8_t>& buffer);
		static std::unique_ptr<sf::Packet> create_sensor_list_packet(const std::vector<Sensor::SensorInfo>& sensors);
		static std::unique_ptr<sf::Packet> create_data_packet(const std::vector<std::pair<sf::Uint8, float>>& data);
		static std::unique_ptr<sf::Packet> create_log_packet(const std::string& message);
		static std::unique_ptr<sf::Packet> create_battery_percentage_packet(float percent);
		static std::unique_ptr<sf::Packet> create_rov_state_packet(unsigned currentState);
		static std::unique_ptr<sf::Packet> create_mission_data_file_list_packet(const std::vector<std::string>& files);
		static std::unique_ptr<sf::Packet> create_mission_data_file_packet(std::ifstream& file);
	};
}
