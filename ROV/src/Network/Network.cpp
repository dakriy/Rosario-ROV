#include "Network.h"
#include <chrono>

Network::Network * Network::Network::instance = nullptr;

Network::Network::Network() : messenger(&Network::watch, this) {
	if (instance != nullptr)
		throw "Only one instance should exist at a time.";
	instance = this;
	// Bind to the broadcast port
	listener.listen(connectionPort);
	broadcast.setBlocking(true);
	listener.setBlocking(true);
	connection.setBlocking(true);
	selector.add(broadcast);
	selector.add(listener);
}

Network::Network::~Network() {
	done = true;
	if (messenger.joinable()) messenger.join();
	connection.disconnect();
	broadcast.unbind();
	listener.close();
	selector.clear();
	instance = nullptr;
	while (!sendQueue.empty()) {
		delete sendQueue.front();
		sendQueue.pop();
	}
}

void Network::Network::watch() {
	sf::Clock lastBroadcastTime;

	while (!done)
	{
		// No connection yet, broadcast ourselves.
		if (!connection.getRemotePort()) {
			if (selector.wait(sf::milliseconds(broadcastInterval))) {
				// Check if it was listener that got new data
				if (selector.isReady(listener)) {
					// Accept new connection
					if (listener.accept(connection) == sf::Socket::Done) {
						// Watch the new socket
						selector.add(connection);
					}
				}
			}

			if (lastBroadcastTime.getElapsedTime() >= sf::milliseconds(broadcastInterval)) {

				// -1 as we do not want to include the trailing \n
				// Broadcast ourselves
				if (broadcast.send(magicBroadcastString, sizeof(magicBroadcastString) / sizeof(char) - 1, sf::IpAddress::Broadcast, broadcastPort) == sf::Socket::Status::Done) {
					// Restart broadcast clock
					lastBroadcastTime.restart();
				}
			}
		} else { // Otherwise we got a connection already open, we should constantly check it for stuff
			if(selector.wait(sf::milliseconds(packetWaitTimeout))) {
				consecutiveTimeouts = 0;
				// Somebody is trying to reconnect, did something happen to the previous connection?
				// Since this is used in applications where the network JUST consists of the ROv and computer
				// We will prioritize the new incoming connection.
				if (selector.isReady(listener)) {
					connection.disconnect();
					listener.accept(connection);
				}

				// New packet!
				if (selector.isReady(connection)) {
					sf::Packet p;
					if (connection.receive(p) == sf::Socket::Done) {
						handlePacket(p);
					}
				}
			} else { // TODO: Handle too many timeouts
				++consecutiveTimeouts;
			}


			///////////////////////////////////////////
			//////// SEND DATA          ///////////////
			///////////////////////////////////////////

			// Make sure we are still connected
			if (connection.getRemotePort()) {
				// Send things
				sendQueueGuard.lock();
				while (!sendQueue.empty()) {
					auto p = sendQueue.front();
					// When we aren't done leave everything in the queue to be sent later
					if (connection.send(*p) == sf::Socket::Done) {
						sendQueue.pop();
						delete p;
					} else {
						break;
					}
				}
				sendQueueGuard.unlock();
			}
		}
	}
}

void Network::Network::sendPacket(sf::Packet * packet) {
	sendQueueGuard.lock();
	sendQueue.push(packet);
	sendQueueGuard.unlock();
}

PACKET_EVENT_FUNC_INDEX Network::Network::hook(const PACKET_EVENT_FUNC_TYPE_NS &hook, PacketTypes type) {
	handlerGuard.lock();
	auto index = packetHandler.add_event_callback(hook, type);
	handlerGuard.unlock();
	return index;
}

PACKET_EVENT_FUNC_INDEX Network::Network::hookAll(const PACKET_EVENT_FUNC_TYPE_NS &hook) {
	handlerGuard.lock();
	auto index = packetHandler.add_event_callback_for_all_events(hook);
	handlerGuard.unlock();
	return index;
}

void Network::Network::unhookAll(PACKET_EVENT_FUNC_INDEX_NS index) {
	handlerGuard.lock();
	packetHandler.unhook_event_callback_for_all_events(index);
	handlerGuard.unlock();
}

void Network::Network::unhookOne(PACKET_EVENT_FUNC_INDEX_NS index, PacketTypes type) {
	handlerGuard.lock();
	packetHandler.unhook_event_callback(index, type);
	handlerGuard.unlock();
}

void Network::Network::handlePacket(sf::Packet &packet) {
	// TODO: Handle packets lol
}
