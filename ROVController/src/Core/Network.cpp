#include "Network.h"
#include "../Utilities.h"
#include "GlobalContext.h"
#include "Event.h"

Core::Network::Network()
{
	broadcast.setBlocking(false);
	connection.setBlocking(false);
	if (connection.bind(42069) == sf::Socket::Status::Error)
	{
		QuitWithError("Could not bind to port, check that all instances of the application are closed.", 1);
	}
	for (auto & p : pingvals)
	{
		p = sf::Time::Zero;
	}
}

bool Core::Network::isConnected() const
{
	return connected;
}

sf::IpAddress Core::Network::connectedHost() const
{
	return ROV;
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

	if (connecting)
	{
		size_t rcvd;
		sf::IpAddress rip;
		unsigned short rport;
		if (connection.receive(static_cast<void*>(recvBuffer.data()), sf::UdpSocket::MaxDatagramSize, rcvd, rip, rport) == sf::Socket::Done)
		{
			if (strcmp(recvBuffer.data(), "connect") == 0)
			{
				connected = true;
				connecting = false;
				connection.send("connected", 9, ROV, connectionPort);
				pingRecvClock.restart();
			}
			else
			{
				connecting = false;
				connected = false;
			}
		}
	}


	if (connected)
	{
		// Receiving
		sf::IpAddress rip;
		unsigned short rport;
		sf::Packet p;
		if (connection.receive(p, rip, rport) == sf::Socket::Done)
		{
			if (rip == ROV)
				process_packet(p);
		}

		// Sending

		// Send ping every 5 seconds
		if (pingClock.getElapsedTime().asSeconds() > 5)
		{
			send_packet(PacketTypes::Ping);
		}

		if (pingRecvClock.getElapsedTime().asSeconds() > 15)
		{
			connected = false;
		}
	}
}

void Core::Network::send_packet(PacketTypes t)
{
	sf::Packet p;
	p << static_cast<sf::Int8>(t);
	if (connected)
	{
		switch(t)
		{
		case PacketTypes::Ping:
			pingClock.restart();
		case PacketTypes::StartVideo:
		case PacketTypes::StopVideo:
		case PacketTypes::StartTemp:
		case PacketTypes::StopTemp:
		case PacketTypes::StartPressure:
		case PacketTypes::StopPressure:
		case PacketTypes::StartMoveUp:
		case PacketTypes::StopMoveUp:
		case PacketTypes::StartMoveDown:
		case PacketTypes::StopMoveDown:
		default:
			connection.send(p, ROV, connectionPort);
			break;
		}
	}
}

void Core::Network::process_packet(sf::Packet& p)
{
	sf::Int8 t;
	p >> t;
	auto type = static_cast<PacketTypes>(t);
	switch (type)
	{
	case PacketTypes::Ping:
	{
		if (pingCounter == pingWindow)
			pingCounter = 0;
		pingvals[pingCounter++] = pingClock.getElapsedTime();

		// We got a ping so reset the watchdog
		pingRecvClock.restart();
		Core::Event e(Core::Event::EventType::PingReceived);
		GlobalContext::get_engine()->add_event(e);
		break;
	}
	case PacketTypes::Video:
	{
		Core::Event e(Core::Event::EventType::VideoFrameReceived);

		e.f.data = nullptr;
		e.f.h = 0;
		e.f.w = 0;

		GlobalContext::get_engine()->add_event(e);
		break;
	}	
	case PacketTypes::Temperature:
	{
		float temp;
		p >> temp;
		Core::Event e(Core::Event::EventType::TemperatureReceived);
		e.t.temp = temp;

		GlobalContext::get_engine()->add_event(e);
		break;
	}
	case PacketTypes::Pressure:
	{
		float pressure;
		p >> pressure;
		Core::Event e(Core::Event::EventType::PressureReceived);
		e.p.pressure = pressure;

		GlobalContext::get_engine()->add_event(e);
		break;
	}
	default:
		break;
	}
}

void Core::Network::disconnect()
{
	connected = false;
	connecting = false;
}

float Core::Network::get_ping_time()
{
	auto total = sf::Time::Zero;
	unsigned n = 0;
	for (auto t : pingvals)
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

sf::Socket::Status Core::Network::connect_to_host(sf::IpAddress addr)
{
	connected = false;
	connecting = true;
	ROV = addr;
	return connection.send("connectRequest", 14, addr, connectionPort);
}

Core::Network::~Network()
{
	broadcast.unbind();
	connection.unbind();
}
