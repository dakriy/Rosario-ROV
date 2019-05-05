#include "Network.h"
#include "../Core/GlobalContext.h"
#include "../Sensors/Sensor.h"
#include <chrono>
#include <cstring>

Network::Network * Network::Network::instance = nullptr;

Network::Network::Network() : messenger(&Network::watch, this), done(false), closeConnection(false), connected(false) {
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
						if (event) {
							preProcess(event);
							GlobalContext::get_engine()->add_event(std::move(event));
						}
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
					auto p = std::move(sendQueue.front());
					sendQueue.pop();
					sendQueueGuard.unlock();
					auto status = connection.send(*p);
					if (status == sf::Socket::Disconnected) {
						closeConnection = true;
					}
				}
			}

			if (closeConnection) {
				closeConnection = false;
				connected = false;
				selector.remove(connection);
				connection.disconnect();
				GlobalContext::get_engine()->add_event(std::make_unique<Core::Event>(Core::Event::Disconnected));
			}
		}
	}
}

void Network::Network::sendPacket(std::unique_ptr<sf::Packet> packet) {
	if (connected) {
		// Place it on the packet queue
		sendQueueGuard.lock();
		sendQueue.push(std::move(packet));
		sendQueueGuard.unlock();
	}
}

bool Network::Network::isConnected() {
	return connected;
}

std::unique_ptr<Core::Event> Network::Network::decode(sf::Packet &p) {
	auto t = static_cast<sf::Uint8>(PacketTypes::Count);
	if (!(p >> t)) {
		// Error?
		// No type sent? or something weird happened
		return nullptr;
	}
	auto type = static_cast<PacketTypes>(t);

	std::unique_ptr<Core::Event> pEvent = nullptr;

	// Decode all of the packets here
	switch (type) {
		case PacketTypes::Ping:
			pEvent = std::make_unique<Core::Event>(Core::Event::PingReceived);
			break;
		case PacketTypes::Shutdown:
			pEvent = std::make_unique<Core::Event>(Core::Event::Shutdown);
			break;
		case PacketTypes::MissionStart:
		{
			pEvent = std::make_unique<Core::Event>(Core::Event::MissionStart);

			float frequency = 0.f;
			size_t sensorNum = 0;

			if (!(p >> frequency >> sensorNum)) {
				return nullptr;
			}

			pEvent->r.frequency = frequency;
			pEvent->r.sensors.resize(sensorNum);

			for (auto i = 0; i < sensorNum; ++i) {
				auto sensorType = static_cast<sf::Uint8>(Sensor::SensorId::Count);
				if (!(p >> sensorType)) {
					return nullptr;
				}
				pEvent->r.sensors.push_back(sensorType);
			}
			break;
		}
		case PacketTypes::MissionStop:
			pEvent = std::make_unique<Core::Event>(Core::Event::MissionStop);
			break;
		case PacketTypes::RequestSensors:
			pEvent = std::make_unique<Core::Event>(Core::Event::SensorRequest);
			break;
		case PacketTypes::StartVideoStream:
			pEvent = std::make_unique<Core::Event>(Core::Event::StartCamera);
			break;
		case PacketTypes::StopVideoStream:
			pEvent = std::make_unique<Core::Event>(Core::Event::StopCamera);
			break;
		default: //unknown packet type
			pEvent.reset();
			break;
	}
	return pEvent;
}

void Network::Network::preProcess(std::unique_ptr<Core::Event> &ev) {
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
