#pragma once

#include <SFML/Network.hpp>
#include <vector>
#include <array>
#include <chrono>

namespace Core
{
	enum class PacketTypes
	{
		Ping = 0,
		Video = 11,
		Temperature = 12,
		Pressure = 13,

		Count
	};
	class Network
	{
	protected:
		// Constants
		static const int broadcastBufferLen = 100;

		static const int pingWindow = 10;

		
		// Flags
		bool connected = false;

		bool connecting = false;

		bool search = false;

		
		
		// Sockets and buffers
		sf::UdpSocket broadcast;

		sf::UdpSocket connection;

		sf::IpAddress ROV;

		std::vector<std::pair<sf::IpAddress, std::string>> found_devices;

		std::array<char, broadcastBufferLen> broadcastBuffer;

		std::array<char, sf::UdpSocket::MaxDatagramSize> recvBuffer;


		// Ping information
		unsigned int pingCounter = 0;

		sf::Clock pingClock;
		sf::Clock pingRecvClock;

		std::array<sf::Time, pingWindow> pingvals;

	public:
		static const unsigned short connectionPort = 42069;

		static const unsigned short broadcastPort = 42070;

		Network();

		bool isConnected() const;

		sf::IpAddress connectedHost() const;

		const std::vector<std::pair<sf::IpAddress, std::string>>& get_devices() const;

		void search_for_devices();

		void stop_search_for_devices();

		void process_packets();

		void process_packet(sf::Packet&);

		void disconnect();

		float get_ping_time();

		sf::Socket::Status connect_to_host(sf::IpAddress);

		~Network();
	};
}
