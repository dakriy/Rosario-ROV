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

std::vector<std::pair<std::string, sf::IpAddress>> Core::Network::get_devices()
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

    	///////////////////////////////////////////////////////////
    	/////////////// PROCESS SEARCHING /////////////////////////
		///////////////////////////////////////////////////////////
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
                                	bool found = false;
                                	for (auto & d : found_devices) {
                                		if (std::get<sf::IpAddress>(d) == rip) {
                                			found = true;
                                			break;
                                		}
                                	}
                                	if (!found)
                                    	found_devices.emplace_back(std::make_pair(broadcastBuffer.data() + pos[0], rip));
                                    devicesLock.unlock();
                                }
                            }
                        }
                    }
                }
            }
            // TODO: Remove things from the found devices if they've been there for too long
        }

		///////////////////////////////////////////////////////////
		/////////////// PROCESS CONNECTIONS ///////////////////////
		///////////////////////////////////////////////////////////

        if (closeConnection) {
        	syncState = 0;
        	pingTime = .5f;
        	closeConnection = false;
        	connected = false;
        	selector.remove(connection);
        	connection.disconnect();
			GlobalContext::get_engine()->add_event(std::make_unique<Core::Event>(Core::Event::Disconnected));
        }

        if (startConnect) {
        	startConnect = false;
        	if (connection.connect(std::get<sf::IpAddress>(ROV), connectionPort) == sf::TcpSocket::Status::Error) {
        		// Error
				connected = false;
        	} else {
				connected = true;
				selector.add(connection);
				auto e = std::make_unique<Core::Event>(Core::Event::NewConnection);
				e->data = std::get<std::string>(ROV);
				GlobalContext::get_engine()->add_event(std::move(e));
        	}
		}

        if (connected) {
        	// 3 pings is probably good for doing time syncing
        	if (!timesync && syncState == 3) {
				sf::Packet p;
				p << static_cast<sf::Uint8>(PacketTypes::TimeSync);
				syncStart = GlobalContext::get_clock()->getElapsedTime();
				if (connection.send(p) == sf::Socket::Status::Disconnected) {
					closeConnection = true;
				}
				timesync = true;
        	}

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
            // If syncing time don't bother sending, just go back to receiving.
            while(!packetQueue.empty() && !timesync) {
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

            // Send ping packet every so often
            if (pingClock.getElapsedTime().asSeconds() > pingTime) {
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
    	case PacketTypes::TimeSync:
		{
			sf::Uint64 us;
			if (!(p >> us)) {
				// setup to try again cause bad packet
				syncState = 2;
			} else {
				++syncState;
				timeOffset = syncStart - sf::microseconds(us) - get_ping_time();
				pingTime = 5.f;
				GlobalContext::get_log()->AddLog(
						"[%.1f] [%s] Time synced, dt: %f seconds\n",
						GlobalContext::get_clock()->getElapsedTime().asSeconds(), "log", timeOffset.asSeconds());
			}
			timesync = false;
			break;
		}
        case PacketTypes::Ping:
            event = std::make_unique<Core::Event>(Core::Event::PingReceived);
//            GlobalContext::get_engine()->log.AddLog(
//            		"[%.1f] [%s] Ping packet received\n",
//            		GlobalContext::get_clock()->getElapsedTime().asSeconds(), "log");
            ++syncState;
			break;
    	case PacketTypes::Sensors:
		{
			// Get number of sensors returned
			sf::Uint32 sensorsNumber = 0;
			if (!(p >> sensorsNumber)) {
				return nullptr;
			}
			auto sInfo =  std::vector<SensorInfo>();

			sInfo.reserve(sensorsNumber);

			// Retrieve sensor information from each sensor.
			for (unsigned i = 0; i < sensorsNumber; ++i) {
				sf::Uint8 id;
				float maxFrequency;
				std::string name;
				std::string units;
				if (!(p >> id >> maxFrequency >> name >> units)) return nullptr;
				sInfo.emplace_back(id, maxFrequency, name, units);
			}
			event = std::make_unique<Core::Event>(Core::Event::SensorInfoReceived);
			event->data = std::move(sInfo);
			break;
		}
    	case PacketTypes::Data:
		{
			// Pull number of measurements from packet
			sf::Uint32 sensorsNumber = 0;
			sf::Int64 us = 0;
			if (!(p >> us >> sensorsNumber)) {
				return nullptr;
			}


			auto data = std::vector<std::pair<sf::Uint8, float>>();

			data.reserve(sensorsNumber);

			// Get each piece of data.
			for (unsigned i = 0; i < sensorsNumber; ++i) {
				sf::Uint8 sensorId = 0;
				float val = 0.f;
				if (!(p >> sensorId >> val)) {
					return nullptr;
				}
				data.emplace_back(std::make_pair(sensorId, val));
			}
			event = std::make_unique<Core::Event>(Core::Event::DataReceived);
			event->data = std::make_pair(convertRemoteTime(sf::microseconds(us)), std::move(data));
			break;
		}
		case PacketTypes::Video:
		{
			sf::Uint32 size;

			if (!(p >> size)) {
				return nullptr;
			}

			// 5 because 1 byte for type, and 4 bytes for the size byte
			if (p.getDataSize() == size + 5) {
				event = std::make_unique<Core::Event>(Core::Event::VideoFrameReceived);
				event->data = std::vector<uint8_t>(
						// Start at where the image starts
						static_cast<const uint8_t*>(p.getData()) + 5,
						// go to the end of the image
						static_cast<const uint8_t*>(p.getData()) + 5 + size
				);
			} else {
				// Invalid format, drop it
				return nullptr;
			}
			break;
		}
    	case PacketTypes::Message:
		{
			std::string message;
			if (!(p >> message)) {
				return nullptr;
			}
			event = std::make_unique<Core::Event>(Core::Event::NewMessage);
			event->data = message;
			break;
		}
    	case PacketTypes::BatteryPercent:
		{
			float percent;
			if (!(p >> percent)) {
				return nullptr;
			}
			event = std::make_unique<Core::Event>(Core::Event::BatteryUpdate);
			event->data = percent;
			break;
		}
    	case PacketTypes::ROVState:
		{
			unsigned state;
			if (!(p >> state)) {
				return nullptr;
			}

			// Invalid state
			if (state >= static_cast<int>(Core::ROVState::COUNT)) {
				return nullptr;
			}

			event = std::make_unique<Core::Event>(Core::Event::ROVStateUpdate);
			event->data = static_cast<Core::ROVState>(state);
			break;
		}
    	case PacketTypes::MissionFileList:
		{
			sf::Uint16 numberOfFiles;
			if (!(p >> numberOfFiles)) {
				return nullptr;
			}

			std::vector<std::string> fileList;
			fileList.reserve(numberOfFiles);

			for (unsigned i = 0; i < numberOfFiles; ++i) {
				std::string fileName;
				if (!(p >> fileName)) {
					return nullptr;
				}
				fileList.emplace_back(std::move(fileName));
			}
			event = std::make_unique<Core::Event>(Core::Event::MissionFileListReceived);
			event->data = std::move(fileList);
			break;
		}
    	case PacketTypes::MissionFile:
		{
			sf::Uint32 byteCountInMissionFile;
			if (!(p >> byteCountInMissionFile)) {
				return nullptr;
			}

			// Total size is the file data + size of file data size number + size of the packet type data type
			if (p.getDataSize() == byteCountInMissionFile + sizeof(byteCountInMissionFile) + 1) {
				std::vector<uint8_t> fileData;
				fileData.reserve(byteCountInMissionFile);
				event = std::make_unique<Core::Event>(Core::Event::MissionFileReceived);
				event->data = std::vector<uint8_t>(
						// Start at where the file starts
						static_cast<const uint8_t*>(p.getData()) + sizeof(byteCountInMissionFile) + 1,
						// go to the end of the file data
						static_cast<const uint8_t*>(p.getData()) +
						sizeof(byteCountInMissionFile) +
						1 +
						byteCountInMissionFile
				);
			} else {
				// Invalid format, drop it
				return nullptr;
			}
			break;
		}
        default: //unknown packet type
        	event.reset();
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

sf::Time Core::Network::get_ping_time() const {
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
		return sf::milliseconds(999999999);
	}
	return total / static_cast<float>(n);
}


bool Core::Network::isConnected() const {
	return connected;
}

void Core::Network::connect_to_host(std::string name, sf::IpAddress addr)
{
	closeConnection = true;
	ROV = std::make_pair(name, addr);
	startConnect = true;
}

std::pair<std::string, sf::IpAddress> Core::Network::getConnectedHost() const {
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

sf::Time Core::Network::convertRemoteTime(sf::Time t) {
	if (isConnected())
		return t + timeOffset - get_ping_time() / 2.f;
	return t;
}
