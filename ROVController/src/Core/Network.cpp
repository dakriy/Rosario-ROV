#include "Network.h"
#include "../Utilities.h"

Core::Network::Network()
{
	broadcast.setBlocking(false);
	connection.setBlocking(false);
	if (connection.bind(42069) == sf::Socket::Status::Error)
	{
		QuitWithError("Could not bind to port, check that all instances of the application are closed.", 1);
	}
}

const std::vector<std::pair<sf::IpAddress, std::string>>& Core::Network::get_devices() const
{
	return found_devices;
}

void Core::Network::search_for_devices()
{
	if (broadcast.bind(broadcastPort) == sf::Socket::Status::Error)
	{
		QuitWithError("Could not bind broadcast socket to port, check that all instances of the application are closed.", 1);
	}
	search = true;
}

void Core::Network::stop_search_for_devices()
{
	broadcast.unbind();
	found_devices.clear();
	search = false;
}

void Core::Network::process_packets()
{
	if (search)
	{
		size_t rcvd;
		sf::IpAddress rip;
		unsigned short rport;
		auto g = false;
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
				std::array<size_t, 2> pos;
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

Core::Network::~Network()
{
	broadcast.unbind();
	connection.unbind();
}
