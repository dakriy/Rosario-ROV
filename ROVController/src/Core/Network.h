#pragma once

#include <SFML/Network.hpp>
#include <vector>
#include <array>

namespace Core
{
	class Network
	{
	protected:

		static const int broadcastBufferLen = 100;

		bool connected = false;

		bool search = false;

		sf::UdpSocket broadcast;

		sf::UdpSocket connection;

		sf::IpAddress ROV;

		std::vector<std::pair<sf::IpAddress, std::string>> found_devices;

		std::array<char, broadcastBufferLen> broadcastBuffer;

		std::array<char, sf::UdpSocket::MaxDatagramSize> recvBuffer;

	public:
		static const unsigned short connectionPort = 42069;

		static const unsigned short broadcastPort = 42070;

		Network();

		const std::vector<std::pair<sf::IpAddress, std::string>>& get_devices() const;

		void search_for_devices();

		void stop_search_for_devices();

		void process_packets();

		~Network();		
	};
}
