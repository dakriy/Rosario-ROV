#include "Network.h"
#include "../Core/GlobalContext.h"
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
	listener.close();
	connection.disconnect();
	broadcast.unbind();
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
		//////////////////////////////////////////
		///////    NO ACTIVE CONNECTION //////////
		//////////////////////////////////////////
		if (!connected) {
			if (selector.wait(sf::milliseconds(packetWaitTimeout))) {
				// Check if it was listener that got new data
				if (selector.isReady(listener)) {
					// Accept new connection
					if (listener.accept(connection) == sf::Socket::Done) {
						// Watch the new socket
						connected = true;
						selector.add(connection);
					}
				}
			}

			// Time to rebroadcast?
			if (lastBroadcastTime.getElapsedTime() >= sf::milliseconds(broadcastInterval)) {
				// -1 as we do not want to include the trailing \n
				// Broadcast ourselves
				if (broadcast.send(magicBroadcastString, strlen(magicBroadcastString), sf::IpAddress::Broadcast, broadcastPort) == sf::Socket::Status::Done) {
					// Restart broadcast clock
					lastBroadcastTime.restart();
				}
			}

			if (closeConnection) {
				closeConnection = false;
			}
		} else { // Otherwise we got a connection already open, we should constantly check it for stuff
			////////////////////////////////////////
			/////////   ACTIVE CONNECTION   ////////
			////////////////////////////////////////

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
					auto status = connection.receive(p);
					if (status == sf::Socket::Done) {
						auto event = decode(p);
						preProcess(event);
						GlobalContext::get_engine()->add_event(event);
					} else if (status == sf::Socket::Disconnected) {
						closeConnection = true;
					}
				}
			} else { // TODO: Handle too many timeouts
				++consecutiveTimeouts;
			}


			///////////////////////////////////////////
			////////       SEND DATA    ///////////////
			///////////////////////////////////////////

			// Make sure we are still connected
			if (connected) {
				// Send things
				while (!sendQueue.empty()) {
					sendQueueGuard.lock();
					auto p = sendQueue.front();
					auto status = connection.send(*p);
					if (status == sf::Socket::Done) {
						sendQueue.pop();
						delete p;
					} else if (status == sf::Socket::Disconnected) {
						closeConnection = true;
					}
					sendQueueGuard.unlock();
				}
			}

			if (closeConnection) {
				closeConnection = false;
				connected = false;
				selector.remove(connection);
				connection.disconnect();
				GlobalContext::get_engine()->add_event(new Core::Event(Core::Event::Disconnected));
			}
		}
	}
}

void Network::Network::sendPacket(sf::Packet * packet) {
	if (isConnected()) {
		// Place it on the packet queue
		sendQueueGuard.lock();
		sendQueue.push(packet);
		sendQueueGuard.unlock();
	} else {
		// Get rid of it if we aren't connected.
		delete packet;
	}
}

bool Network::Network::isConnected() const {
	return connected;
}

Core::Event *Network::Network::decode(sf::Packet &p) {
	auto t = static_cast<sf::Uint8>(PacketTypes::Count);
	if (!(p >> t)) {
		// Error?
		// No type sent? or something weird happened
		return nullptr;
	}
	auto type = static_cast<PacketTypes>(t);

	auto pEvent = new Core::Event;

	// Decode all of the packets here
	switch (type) {
		case PacketTypes::Ping:
			pEvent->type = Core::Event::PingReceived;
			break;
		case PacketTypes::Shutdown:
			pEvent->type = Core::Event::Shutdown;
			break;
		case PacketTypes::Video:
		case PacketTypes::StartVideo:
		case PacketTypes::StopVideo:
		case PacketTypes::StartTemp:
		case PacketTypes::StopTemp:
		case PacketTypes::StartPressure:
		case PacketTypes::StopPressure:
		case PacketTypes::Move:
		case PacketTypes::Temperature:
		case PacketTypes::Pressure:
		default: //unknown packet type
			delete pEvent;
			return nullptr;
	}
	return pEvent;
}

void Network::Network::preProcess(Core::Event * ev) {
	if(!ev) return;
	switch (ev->type) {
		case Core::Event::PingReceived:
		{
			// Immediately send back a ping
			sf::Packet p;
			p << static_cast<sf::Uint8>(PacketTypes::Ping);
			if (connection.send(p) == sf::Socket::Status::Disconnected) {
				closeConnection = true;
			}
			break;
		}
		case Core::Event::Disconnected:
			disconnect();
			break;
		default: break;
	}
}

void Network::Network::disconnect() {
	closeConnection = true;
}
