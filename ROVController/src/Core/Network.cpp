#include "Network.h"
#include "../Utilities/Utilities.h"
#include "GlobalContext.h"
#include "Event.h"
#include <cstring>

Core::Network::Network * Core::Network::Network::instance = nullptr;

Core::Network::Network() : runner(&Network::run, this)
{
    // TODO: Derive from std exception
    if (instance != nullptr)
        throw "Only one instance should exist at a time.";
    instance = this;

	broadcast.setBlocking(true);
	connection.setBlocking(true);

	for (auto & p : pingvals) p = sf::Time::Zero;

	broadcastBuffer.fill(0);

	selector.add(broadcast);
}

const std::vector<std::pair<sf::IpAddress, std::string>>& Core::Network::get_devices() const
{
	return found_devices;
}

void Core::Network::search_for_devices()
{
//	if (broadcast.bind(broadcastPort) == sf::Socket::Status::Error)
//	{
//		QuitWithError("Could not bind broadcast socket to port, check that all instances of the application are closed.", 1);
//	}
	search = true;
}

void Core::Network::stop_search_for_devices()
{
	broadcast.unbind();
	found_devices.clear();
	search = false;
}

void Core::Network::run()
{
    while (!done) {
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
                                    found_devices.emplace_back(std::make_pair(rip, broadcastBuffer.data() + pos[0]));
                                }
                            }
                        }
                    }
                }
            }
            // TODO: Remove things from the found devices if they've been there for too long
        }

        if (connected) {
            // Receive
            selector.wait(sf::milliseconds(packetWaitTimeout)) {
                if (selector.isReady(connection)) {

                }
            }
        }

        // Sending things
        packetQueueLock.lock();
        while(!packetQueue.empty()) {
            // Get packet pointer
            auto p = packetQueue.front();
            // Send the packet pointer
            auto status = connection.send(*p);
            if (status == sf::Socket::Status::Disconnected) {
                connected = false;
            }
            // TODO: Don't drop packet if error in socket maybe? At least look into it
            // Pop it off the top.
            packetQueue.pop();
            delete p;
        }
        packetQueueLock.unlock();

//        if (connected)
//        {
//
//            auto done = continueRecv();
//
//            // If receiving has finished from last packet, there is new data available, and it is the count meaning the packet identifier
//            if (done && currentIncomingType == PacketTypes::Count)
//            {
//                // Identify the packet
//                auto t = static_cast<PacketTypes>(buffer[0]);
//                // Start the packet
//                startPacket(t);
//                done = false;
//            }
//
//            if (done)
//            {
//                // Done receiving packet, process it and setup for the next one...
//                process_packet(currentIncomingType);
//                received = 0;
//                expectedSize = 1;
//                currentIncomingType = PacketTypes::Count;
//            }
//
//            // Sending
//            // Send ping every 5 seconds
//            if (pingClock.getElapsedTime().asSeconds() > 5)
//            {
//                send_ping_packet();
//            }
//        }
    }
}

Core::Event* Core::Network::decode(sf::Packet & p) {
    auto t = static_cast<sf::Uint8>(PacketTypes::Count);
    if (!(p >> t)) {
        // Error?
        return nullptr;
    }
    auto type = static_cast<PacketTypes>(t);
    switch (t) {
        case PacketTypes::Ping:

    }
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
