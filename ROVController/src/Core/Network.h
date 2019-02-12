#pragma once

#include <SFML/Network.hpp>
#include <vector>
#include <array>
#include <chrono>
#include "Engine.h"

namespace Core
{
	enum class PacketTypes
	{
		Ping,
		StartVideo,
		StopVideo,
		StartTemp,
		StopTemp,
		StartPressure,
		StopPressure,
		StartMoveUp,
		StopMoveUp,
		StartMoveDown,
		StopMoveDown,
		Video,
		Temperature,
		Pressure,
		Shutdown = 255,
		Count
	};

	class Network
	{
	protected:

		// Constants
		static const int broadcastBufferLen = 100;

		// max packet size in bytes
		static const int bufferLen = 4096;

		static const int pingWindow = 10;
		
		// Flags
		bool connected = false;

		bool connecting = false;

		bool search = false;

		
		
		// Sockets and buffers
		sf::UdpSocket broadcast;

		sf::TcpSocket connection;

		sf::IpAddress ROV;

		std::vector<std::pair<sf::IpAddress, std::string>> found_devices;

		std::array<unsigned char, 1024> tempBuff;

		std::array<char, broadcastBufferLen> broadcastBuffer;

		// Ping information
		unsigned int pingCounter = 0;

		sf::Clock pingClock;

		std::array<sf::Time, pingWindow> pingvals;


		size_t recvall(size_t len);

		bool continueRecv();

		void startPacket(PacketTypes);

		std::vector<sf::Uint8> buffer;
		
		size_t frameBuffSize = 1920 * 1080 * 4 + 100;

		PacketTypes currentIncomingType = PacketTypes::Count;
		Event * incoming = nullptr;
		size_t received = 0;
		size_t expectedSize = 1;

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

		void send_packet(PacketTypes t);

		void process_packet(PacketTypes t);

		void disconnect();

		float get_ping_time();

		void connect_to_host(sf::IpAddress);

		~Network();
	};
}
