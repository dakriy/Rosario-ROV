#pragma once

#include <SFML/Network.hpp>
#include "../Core/Network.h"

namespace Factory {
	class PacketFactory {
	protected:
		static void add_type_to_packet(sf::Packet &p, Core::PacketTypes type);
	public:
		static sf::Packet create_shutdown_packet();
		static sf::Packet create_camera_move_packet(float amount);
		static sf::Packet create_video_stream_start_packet();
		static sf::Packet create_video_stream_stop_packet();
		static sf::Packet create_pressure_data_start_packet();
		static sf::Packet create_pressure_data_stop_packet();
		static sf::Packet create_temperature_data_start_packet();
		static sf::Packet create_temperature_data_stop_packet();
	};
}
