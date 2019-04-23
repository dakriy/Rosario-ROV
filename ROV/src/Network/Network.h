#pragma once
#include <SFML/Network.hpp>
#include "NetworkConstants.h"
#include <thread>
#include <functional>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <queue>
#include "../Core/Event.h"
#include "../Core/Engine.h"
#include <atomic>

namespace Network {
	class Network {
	protected:
		sf::SocketSelector selector;
		sf::UdpSocket broadcast;
		sf::TcpSocket connection;
		sf::TcpListener listener;

		std::atomic_bool done = false, closeConnection = false;
		bool connected = false;


		// Running threads
		std::thread messenger;

		// Synchronization locks
		std::mutex sendQueueGuard;

		// Incoming and outgoing queues
		std::queue<sf::Packet *> sendQueue;

		static Network * instance;

		unsigned consecutiveTimeouts = 0;

		void watch();

		Core::Event* decode(sf::Packet &p);
		void preProcess(Core::Event *);
	public:
		Network();

		void sendPacket(sf::Packet *packet);

		bool isConnected() const;

		void disconnect();

		~Network();
	};
}
