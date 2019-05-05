#include "Network.h"
#include "../Utilities/Utilities.h"
#include "GlobalContext.h"
#include "EventHandler.h"
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
                        if (event) {
							// Do any necessary pre-processing
							preProcess(event);
							// Pass it to the event handler
							GlobalContext::get_engine()->add_event(std::move(event));
                        }
                    }
                }
            }

            // Sending things
            while(!packetQueue.empty()) {
                packetQueueLock.lock();
                // Get packet pointer
                auto p = std::move(packetQueue.front());
                // Pop it off the top.
                packetQueue.pop();
                packetQueueLock.unlock();
                // Send the packet pointer
                auto status = connection.send(*p);
                if (status == sf::Socket::Status::Disconnected) {
                    closeConnection = true;
                }
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

std::unique_ptr<Core::Event> Core::Network::decode(sf::Packet &p) {
    auto t = static_cast<sf::Uint8>(PacketTypes::Count);
    if (!(p >> t)) {
        // Error?
        // No type sent? or something weird happened
        return nullptr;
    }
    auto type = static_cast<PacketTypes>(t);

    std::unique_ptr<Core::Event> event = nullptr;

    // Decode all of the packets here
    switch (type) {
        case PacketTypes::Ping:
            event = std::make_unique<Core::Event>(Core::Event::PingReceived);
            GlobalContext::get_engine()->log.AddLog(
            		"[%.1f] [%s] Ping packet received\n",
            		GlobalContext::get_clock()->getElapsedTime().asSeconds(), "log");
			break;
    	case PacketTypes::Sensors:
		{
			event = std::make_unique<Core::Event>(Core::Event::SensorInfoReceived);

			// Get number of sensors returned
			sf::Uint32 sensorsNumber = 0;
			if (!(p >> sensorsNumber)) {
				return nullptr;
			}

			event->sInfo.reserve(sensorsNumber);

			// Retrieve sensor information from each sensor.
			for (unsigned i = 0; i < sensorsNumber; ++i) {
				sf::Uint8 id;
				float maxFrequency;
				std::string name;
				std::string units;
				if (!(p >> id >> maxFrequency >> name >> units)) return nullptr;
				event->sInfo.emplace_back(id, maxFrequency, name, units);
				GlobalContext::get_engine()->log.AddLog(
						"[%.1f] [%s] New Sensor Info:\nId: %u \nFreq: %f\nName: %s\nUnits: %s\n",
						GlobalContext::get_clock()->getElapsedTime().asSeconds(), "log", id, maxFrequency, name.c_str(), units.c_str());
			}
			break;
		}
    	case PacketTypes::Data:
		{
			event = std::make_unique<Core::Event>(Core::Event::DataReceived);

			// Pull number of measurements from packet
			sf::Uint32 sensorsNumber = 0;
			if (!(p >> sensorsNumber)) {
				return nullptr;
			}

			event->data.reserve(sensorsNumber);

			// Get each piece of data.
			for (unsigned i = 0; i < sensorsNumber; ++i) {
				float val = 0.f;
				if (!(p >> val)) {
					return nullptr;
				}
				event->data.emplace_back(val);
				GlobalContext::get_engine()->log.AddLog(
						"[%.1f] [%s] New Sensor Data %u:\ndata: %f\n",
						GlobalContext::get_clock()->getElapsedTime().asSeconds(), "log", i, val);
			}
			break;
		}
		case PacketTypes::Video:
		{
			event = std::make_unique<Core::Event>(Core::Event::VideoFrameReceived);
			sf::Uint32 size;

			if (!(p >> size)) {
				return nullptr;
			}

//			GlobalContext::get_engine()->log.AddLog(
//					"[%.1f] [%s] New Image Frame\n",
//					GlobalContext::get_clock()->getElapsedTime().asSeconds(), "log");

			// 5 because 1 byte for type, and 4 bytes for the size byte
			if (p.getDataSize() == size + 5) {
				event->imgData = std::vector<uint8_t>(
						// Start at where the image starts
						static_cast<const uint8_t*>(p.getData()) + 5,
						// go to the end of the image
						static_cast<const uint8_t*>(p.getData()) + 5 + size
				);
			} else {
				// Invalid packet, ignore
				return nullptr;
			}
//			for (unsigned i = 0; i < size; ++i) {
//				if ()
//			}
//			for (unsigned i = 0; i < width*height*3; ++i) {
//				if (!(p >> imgData.data[i])) {
//					return nullptr;
//				}
//			}[]
//
//			if (!p.endOfPacket()) {
//				GlobalContext::get_engine()->log.AddLog(
//						"[%.1f] [%s] Extra information?\n",
//						GlobalContext::get_clock()->getElapsedTime().asSeconds(), "log");
//			}
			break;
		}
        default: //unknown packet type
        	event = nullptr;
        	break;
    }
    return event;
}

Core::Network::~Network()
{
    done = true;
    if (runner.joinable()) runner.join();
    connection.disconnect();
    broadcast.unbind();
    selector.clear();
    instance = nullptr;
}

void Core::Network::preProcess(std::unique_ptr<Event> &ev) {
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

void Core::Network::send_packet(std::unique_ptr<sf::Packet> p) {
	if (isConnected()) {
		// Place it on the packet queue
		packetQueueLock.lock();
		packetQueue.push(std::move(p));
		packetQueueLock.unlock();
	}
}

void Core::Network::disconnect() {
	closeConnection = true;
}
