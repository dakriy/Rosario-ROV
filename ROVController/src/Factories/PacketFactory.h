#pragma once

#include <SFML/Network.hpp>
#include "../Core/Network.h"
#include <memory>

namespace Factory {
	class PacketFactory {
	protected:
		static void add_type_to_packet(Core::PacketTypes type, std::unique_ptr<sf::Packet> &p);
	public:
		static std::unique_ptr<sf::Packet> create_shutdown_packet();
		static std::unique_ptr<sf::Packet> create_camera_move_packet(float amount);
		static std::unique_ptr<sf::Packet> create_video_stream_start_packet();
		static std::unique_ptr<sf::Packet> create_video_stream_stop_packet();
		static std::unique_ptr<sf::Packet> create_pressure_data_start_packet();
		static std::unique_ptr<sf::Packet> create_pressure_data_stop_packet();
		static std::unique_ptr<sf::Packet> create_temperature_data_start_packet();
		static std::unique_ptr<sf::Packet> create_temperature_data_stop_packet();
	};
}
