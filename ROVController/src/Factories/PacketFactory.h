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
	};
}
