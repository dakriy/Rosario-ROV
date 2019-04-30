#pragma once

#include <SFML/Network.hpp>
#include "../Network/Network.h"
#include <memory>

namespace Factory {
	class PacketFactory {
	protected:
		static void add_type_to_packet(std::unique_ptr<sf::Packet> &p, Network::PacketTypes type);
	public:
		static std::unique_ptr<sf::Packet> createVideoPacket();
		static std::unique_ptr<sf::Packet> create_sensor_list_packet(std::vector<Sensor::SensorInfo>& sensors);
		static std::unique_ptr<sf::Packet> create_data_packet(float data);
	};
}
