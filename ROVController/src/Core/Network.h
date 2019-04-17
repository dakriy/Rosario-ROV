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

namespace Core
{
	enum class PacketTypes
	{
		Ping = 0,
		StartVideo = 1,
		StopVideo = 2,
		StartTemp = 3,
		StopTemp = 4,
		StartPressure = 5,
		StopPressure = 6,
		Move = 7,
		Video = 11,
		Temperature = 12,
		Pressure = 13,
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
        static const int packetWaitTimeout = 100;

		// Global Instance
        static Network * instance;

		// Flags
		bool connected = false;
        bool search = false;
        std::atomic_bool done = false;
		
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

		// Threads and synchronization
		std::thread runner;

        void run();

        //void handlePacket(sf::Packet & p);

        //void preProcessPacket();

        std::mutex packetQueueLock;
        std::queue<sf::Packet *> packetQueue;

        Core::Event* decode(sf::Packet &p);
        void preProcess(Core::Event *);

	public:
		static const unsigned short connectionPort = 42069;

		static const unsigned short broadcastPort = 42070;

		Network();

		// const std::vector<std::pair<sf::IpAddress, std::string>>& get_devices() const;

		~Network();
	};
}
