#pragma once

#include <SFML/Network.hpp>
#include <vector>
#include <array>
#include <chrono>
#include "Engine.h"
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <memory>

namespace Core
{
	enum class PacketTypes
	{
		Ping,
		MissionStart,
		MissionStop,
		RequestSensors,
		Sensors,
		Data,
		Move,
		Shutdown = 254,
		Count
	};

	class Network
	{
	protected:
		// Constants
		static const int broadcastBufferLen = 100;
		static const int pingWindow = 5;
		// In milliseconds
        static const int packetWaitTimeout = 10;

		// Global Instance
        static Network * instance;

		// Flags
		bool connected = false;
		bool search = false;
        std::atomic_bool done = false;

		std::atomic_bool stopSearch = false, startSearch = false, startConnect = false, closeConnection = false;
		
		// Sockets
		sf::UdpSocket broadcast;
		sf::TcpSocket connection;
		sf::SocketSelector selector;

		// Buffers and caches
		sf::IpAddress ROV;
		std::vector<std::pair<sf::IpAddress, std::string>> found_devices;
		std::array<char, broadcastBufferLen> broadcastBuffer;

		// Ping information
		sf::Clock pingClock;
		std::array<sf::Time, pingWindow> pingVals;
		int pingCounter = 0;

		// Threads and synchronization
		std::thread runner;

        void run();

        std::mutex packetQueueLock;
        std::queue<std::unique_ptr<sf::Packet>> packetQueue;

        std::unique_ptr<Event> decode(sf::Packet &p);
        void preProcess(std::unique_ptr<Event> &ev);

        std::mutex devicesLock;

	public:
		static const unsigned short connectionPort = 42069;

		static const unsigned short broadcastPort = 42070;

		Network();

		std::vector<std::pair<sf::IpAddress, std::string>> get_devices();

		void search_for_devices();

		void stop_search_for_devices();

		float get_ping_time() const;

		bool isConnected() const;

		sf::IpAddress getConnectedHost() const;

		void connect_to_host(sf::IpAddress addr);

		void send_packet(std::unique_ptr<sf::Packet> p);

		void disconnect();

		~Network();
	};
}
