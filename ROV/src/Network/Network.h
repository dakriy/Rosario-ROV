#pragma once
#include <SFML/Network.hpp>
#include "NetworkConstants.h"
#include <thread>
#include <functional>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <queue>
#include "../Core/EventHandler.h"
#include "../Core/Engine.h"
#include <atomic>
#include <memory>

namespace Network {
	class Network {
	protected:
		sf::SocketSelector selector;
		sf::UdpSocket broadcast;
		sf::TcpSocket connection;
		sf::TcpListener listener;

		std::atomic_bool done, closeConnection, connected;

		// Running threads
		std::thread messenger;

		// Synchronization locks
		std::mutex sendQueueGuard;

		// Incoming and outgoing queues
		std::queue<std::unique_ptr<sf::Packet>> sendQueue;

		static Network * instance;

		unsigned consecutiveTimeouts = 0;

		void watch();

		std::unique_ptr<Core::Event> decode(sf::Packet &p);
		void preProcess(std::unique_ptr<Core::Event> &ev);
	public:
		Network();

		void sendPacket(std::unique_ptr<sf::Packet> packet);

		bool isConnected() const;

		void disconnect();

		~Network();
	};
}
