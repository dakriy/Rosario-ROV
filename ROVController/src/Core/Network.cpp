#include "Network.h"
#include "../Utilities/Utilities.h"
#include "GlobalContext.h"
#include "Event.h"
#include <cstring>

Core::Network * Core::Network::instance = nullptr;

Core::Network::Network() : runner(&Network::run, this)
{
    // TODO: Derive from std exception
    if (instance != nullptr)
        throw "Only one instance should exist at a time.";
    instance = this;

	broadcast.setBlocking(true);
	connection.setBlocking(true);

	for (auto & p : pingVals) p = sf::Time::Zero;

	broadcastBuffer.fill(0);
}

std::vector<std::pair<sf::IpAddress, std::string>> Core::Network::get_devices()
{
	devicesLock.lock();
	auto s = found_devices;
	devicesLock.unlock();
	return s;
}

void Core::Network::search_for_devices()
{
	startSearch = true;
}

void Core::Network::stop_search_for_devices()
{
	stopSearch = true;
}

void Core::Network::run()
{
    while (!done) {
    	if (!search && !connected) {
    		std::this_thread::sleep_for(std::chrono::milliseconds(30));
    	}

		if (stopSearch) {
			search = false;
			stopSearch = false;
			selector.remove(broadcast);
			broadcast.unbind();
			found_devices.clear();
		}

		if (startSearch) {
			search = true;
			startSearch = false;
			if (broadcast.bind(broadcastPort) == sf::Socket::Status::Error) {
				QuitWithError("Could not bind broadcast socket to port, check that all instances of the application are closed.", 1);
			}
			selector.add(broadcast);
		}

        if (search) {
            size_t rcvd;
            sf::IpAddress rip;
            unsigned short rport;
            auto g = false;
            if(selector.wait(sf::milliseconds(packetWaitTimeout))) {
                if (selector.isReady(broadcast)) {
                    if (broadcast.receive(static_cast<void*>(broadcastBuffer.data()), broadcastBufferLen, rcvd, rip, rport) == sf::Socket::Done)
                    {
                        // Check to see if device already exists...
                        for (auto & dev : found_devices)
                        {
                            if (dev.first == rip)
                            {
                                g = true;
                            }
                        }

                        if (rcvd < broadcastBufferLen - 1 && rcvd > 9 && !g)
                        {
                            std::array<size_t, 2> pos = {0, 0};
                            broadcastBuffer[rcvd] = '\0';
                            size_t pipeCount = 0;
                            for (size_t i = 0; i < rcvd; ++i)
                            {
                                if (broadcastBuffer[i] == '|')
                                {
                                    if (pipeCount < 2)
                                    {
                                        pos[pipeCount] = i + 1;
                                        broadcastBuffer[i] = '\0';
                                        ++pipeCount;
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                            }

                            if (pipeCount == 2 && pos[1] + 3 == rcvd)
                            {
                                if (strcmp(broadcastBuffer.data(), "magic") == 0 && strcmp(broadcastBuffer.data() + pos[1], "end") == 0)
                                {
                                	devicesLock.lock();
                                    found_devices.emplace_back(std::make_pair(rip, broadcastBuffer.data() + pos[0]));
                                    devicesLock.unlock();
                                }
                            }
                        }
                    }
                }
            }
            // TODO: Remove things from the found devices if they've been there for too long
        }

        if (closeConnection) {
        	closeConnection = false;
        	connected = false;
        	selector.remove(connection);
        	connection.disconnect();
        }

        if (startConnect) {
        	startConnect = false;
        	if (connection.connect(ROV, connectionPort) == sf::TcpSocket::Status::Error) {
        		// Error
				connected = false;
        	} else {
				connected = true;
				selector.add(connection);
        	}
		}

        if (connected) {
            // Receive
            if (selector.wait(sf::milliseconds(packetWaitTimeout))) {
                if (selector.isReady(connection)) {
                    sf::Packet p;
                    if (connection.receive(p) == sf::Socket::Status::Disconnected) {
                        closeConnection = true;
                    } else {
                    	// Decode the packet
                        auto event = decode(p);
                        // Do any necessary pre-processing
                        preProcess(event);
                        // Pass it to the event handler
                        GlobalContext::get_engine()->add_event(event);
                    }
                }
            }

            // Sending things
            while(!packetQueue.empty()) {
                packetQueueLock.lock();
                // Get packet pointer
                auto p = packetQueue.front();
                // Pop it off the top.
                packetQueue.pop();
                packetQueueLock.unlock();
                // Send the packet pointer
                auto status = connection.send(*p);
                if (status == sf::Socket::Status::Disconnected) {
                    closeConnection = true;
                }
                // Prevent memory leaks
                delete p;
                // TODO: Don't drop packet if error in socket maybe? At least look into it
            }

            // Send ping packet every 5 seconds
            if (pingClock.getElapsedTime().asSeconds() > 5) {
            	sf::Packet p;
				p << static_cast<sf::Uint8>(PacketTypes::Ping);
				pingClock.restart();
				if (connection.send(p) == sf::Socket::Status::Disconnected) {
					closeConnection = true;
				}
            }
        }
    }
}

Core::Event* Core::Network::decode(sf::Packet & p) {
    auto t = static_cast<sf::Uint8>(PacketTypes::Count);
    if (!(p >> t)) {
        // Error?
        // No type sent? or something weird happened
        return nullptr;
    }
    auto type = static_cast<PacketTypes>(t);

    auto packet = new Core::Event;

    // Decode all of the packets here
    switch (type) {
        case PacketTypes::Ping:
        	packet->type = Core::Event::PingReceived;
			break;
		case PacketTypes::Video:
		{
			packet->type = Core::Event::VideoFrameReceived;
			sf::Uint32 size;

			if (!(p >> size)) {
				delete packet;
				return nullptr;
			}

			packet->f.len = size;

			// We'll let the event handler clean up the memory for us
			auto pixels = new sf::Uint8[size];

			bool read = true;

			// Fill out the pixel array assuming we just send straight pixels
			for (unsigned i = 0; i < size && read; ++i) {
				read = (p >> pixels[i]);
			}

			// Something went wrong during read. Abort.
			if (!read) {
				delete [] pixels;
				delete packet;
				return nullptr;
			}

			packet->f.data = pixels;
			break;
		}
		case PacketTypes::StartVideo:
		case PacketTypes::StopVideo:
		case PacketTypes::StartTemp:
		case PacketTypes::StopTemp:
		case PacketTypes::StartPressure:
		case PacketTypes::StopPressure:
		case PacketTypes::Move:
        case PacketTypes::Temperature:
        case PacketTypes::Pressure:
        case PacketTypes::Shutdown:
        default: //unknown packet type
        	delete packet;
            return nullptr;
    }
    return packet;
}

Core::Network::~Network()
{
    done = true;
    if (runner.joinable()) runner.join();
    connection.disconnect();
    broadcast.unbind();
    selector.clear();
    while(!packetQueue.empty()) {
    	delete packetQueue.front();
    	packetQueue.pop();
    }
    instance = nullptr;
}

void Core::Network::preProcess(Core::Event * ev) {
	if(!ev) return;
	switch (ev->type) {
		case Core::Event::PingReceived:
			if (pingCounter == pingWindow) pingCounter = 0;
			pingVals[pingCounter++] = pingClock.getElapsedTime();
			break;
		default:
			break;
	}
}

float Core::Network::get_ping_time() const
{
	auto total = sf::Time::Zero;
	unsigned n = 0;
	for (auto t : pingVals)
	{
		if (t != sf::Time::Zero)
		{
			++n;
			total += t;
		}
	}
	if (n == 0)
	{
		return 9999999999999.f;
	}
	return static_cast<float>(total.asMilliseconds()) / static_cast<float>(n);
}

bool Core::Network::isConnected() const {
	return connected;
}

void Core::Network::connect_to_host(sf::IpAddress addr)
{
	closeConnection = true;
	ROV = addr;
	startConnect = true;
}

sf::IpAddress Core::Network::getConnectedHost() const {
	return ROV;
}

void Core::Network::send_packet(sf::Packet *p) {
	if (isConnected()) {
		// Place it on the packet queue
		packetQueueLock.lock();
		packetQueue.push(p);
		packetQueueLock.unlock();
	} else {
		// Get rid of it if we aren't connected.
		delete p;
	}

}

void Core::Network::disconnect() {
	closeConnection = true;
}
