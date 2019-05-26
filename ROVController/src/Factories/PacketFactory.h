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
		static std::unique_ptr<sf::Packet> create_camera_move_packet(float theta, float r);

		/**
		 * Heres how the packet layout is gonna look...
		 *
		 * Type 					| Core::PacketTypes
		 * frequency (Hz)			| float
		 * Number of Sensors		| size_t prolly
		 * list of sensors			| Core::SensorInfo::SensorID[]
		 *
		 * @param frequency
		 * @param sensors
		 * @return
		 */
		static std::unique_ptr<sf::Packet> create_start_mission_packet(float frequency,const std::vector<sf::Uint8> &sensors);
		static std::unique_ptr<sf::Packet> create_sensor_request_packet();
		static std::unique_ptr<sf::Packet> create_stop_mission_packet();
		static std::unique_ptr<sf::Packet> create_start_video_stream_packet();
		static std::unique_ptr<sf::Packet> create_stop_video_stream_packet();
		static std::unique_ptr<sf::Packet> create_light_packet(bool state, float percent);
	};
}
