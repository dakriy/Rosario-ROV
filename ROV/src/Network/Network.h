#pragma once
#ifndef ROV_NETWORK_H
#define ROV_NETWORK_H

#include <SFML/Network.hpp>
#include "NetworkConstants.h"
#include <thread>
#include <functional>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <queue>
#include "../Core/Event.h"
#include <atomic>

#define PACKET_EVENT_FUNC_TYPE EVENT_FUNC_TYPE(Network::PacketContainer)
#define PACKET_EVENT_FUNC_INDEX EVENT_FUNC_INDEX(Network::PacketContainer, static_cast<int>(Network::PacketTypes::Count))
#define PACKET_EVENT_FUNC_TYPE_NS EVENT_FUNC_TYPE(PacketContainer)
#define PACKET_EVENT_FUNC_INDEX_NS EVENT_FUNC_INDEX(PacketContainer, static_cast<int>(PacketTypes::Count))

namespace Network {
	class Network {
	protected:
		sf::SocketSelector selector;
		sf::UdpSocket broadcast;
		sf::TcpSocket connection;
		sf::TcpListener listener;

		std::atomic_bool done = false;
		bool connected = false;

		// Running threads
		std::thread messenger;

		// Synchronization locks
		std::mutex sendQueueGuard;
		std::mutex handlerGuard;

		// Incoming and outgoing queues
		std::queue<sf::Packet *> sendQueue;

		Core::EventHandler<PacketContainer, static_cast<int>(PacketTypes::Count)> packetHandler;

		static Network * instance;

		unsigned consecutiveTimeouts = 0;

		void watch();
		void handlePacket(sf::Packet &packet);

		PACKET_EVENT_FUNC_INDEX_NS pingHook;
	public:
		Network();

		void sendPacket(sf::Packet *packet);
		PACKET_EVENT_FUNC_INDEX_NS hook(const PACKET_EVENT_FUNC_TYPE_NS & hook, PacketTypes type);
		PACKET_EVENT_FUNC_INDEX_NS hookAll(const PACKET_EVENT_FUNC_TYPE_NS & hook);
		void unhookAll(PACKET_EVENT_FUNC_INDEX_NS index);
		void unhookOne(PACKET_EVENT_FUNC_INDEX_NS index, PacketTypes type);
		~Network();
	};
}

#endif //ROV_NETWORK_H
