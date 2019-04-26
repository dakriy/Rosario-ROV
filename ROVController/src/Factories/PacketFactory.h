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

		/**
		 * Heres how the packet layout is gonna look...
		 *
		 * Type 					| Core::PacketTypes
		 * frequency (Hz)			| float
		 * Number of Sensors		| size_t prolly
		 * list of sensors			| Core::SensorInfo::Sensor[]
		 *
		 * @param frequency
		 * @param sensors
		 * @return
		 */
		static std::unique_ptr<sf::Packet> create_request_data_packet(float frequency, std::vector<Core::SensorInfo::Sensor> sensors);
	};
}
