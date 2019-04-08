#include "Network.h"
#include "../Utilities/Utilities.h"
#include "GlobalContext.h"
#include "Event.h"

size_t Core::Network::recvall(size_t len)
{
	sf::Socket::Status status;
	size_t received = 0, total_received = 0;
	while (received < len)
	{
		status = connection.receive(static_cast<void*>(tempBuff.data() + total_received), len - total_received, received);
		total_received += received;
		if (status == sf::Socket::Done)
		{
			return received;
		}
		if (status == sf::Socket::Disconnected)
		{
			connected = false;
			return received;
		}
		if (status == sf::Socket::Error || status == sf::Socket::NotReady)
		{
			return received;
		}
	}
	return received;
}

bool Core::Network::continueRecv()
{
	if (expectedSize)
	{
		sf::Socket::Status status;
		size_t r;
		status = connection.receive(static_cast<void*>(&buffer[received]), expectedSize - received, r);
		received += r;
		if (status == sf::Socket::Disconnected)
		{
			connected = false;
		}
		if (received == expectedSize)
		{
			received = expectedSize = 0;
			return true;
		}
		return false;

	}
	return true;
}

void Core::Network::startPacket(PacketTypes t)
{
	currentIncomingType = t;
	received = 0;
	// Default to packet being handled and done
	expectedSize = 0;
	switch (t)
	{
	case PacketTypes::Ping:
	{
		incoming = new Core::Event(Event::PingReceived);
		break;
	}
	case PacketTypes::Video:
	{
		//sf::Uint16 w, h;
		sf::Uint32 len;

		recvall(4);

		//w = *reinterpret_cast<sf::Uint16*>(&tempBuff[0]);
		//h = *reinterpret_cast<sf::Uint16*>(&tempBuff[2]);
		len = *reinterpret_cast<sf::Uint32*>(&tempBuff[0]);

		incoming = new Core::Event(Event::VideoFrameReceived);
		incoming->f.len = len;
		expectedSize = len;
		break;
	}

	case PacketTypes::Temperature:
	{
		incoming = new Core::Event(Event::TemperatureReceived);
		expectedSize = 8;
		break;
	}
	case PacketTypes::Pressure:
	{
		incoming = new Core::Event(Event::PressureReceived);
		expectedSize = 8;
		break;
	}
	default:
		break;
	}
}

Core::Network::Network()
{
	broadcast.setBlocking(false);
	connection.setBlocking(false);

	for (auto & p : pingvals)
	{
		p = sf::Time::Zero;
	}
	buffer.resize(frameBuffSize);
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

	if (connected)
	{

		auto done = continueRecv();
		
		// If recieving has finished from last packet, there is new data availble, and it is the count meaning the packet identifier
		if (done && currentIncomingType == PacketTypes::Count)
		{
			// Identify the packet
			auto t = static_cast<PacketTypes>(buffer[0]);
			// Start the packet
			startPacket(t);
			done = false;
		}

		if (done)
		{
			// Done receiving packet, process it and setup for the next one...
			process_packet(currentIncomingType);
			received = 0;
			expectedSize = 1;
			currentIncomingType = PacketTypes::Count;
		}

		// Sending
		// Send ping every 5 seconds
		if (pingClock.getElapsedTime().asSeconds() > 5)
		{
			send_packet(PacketTypes::Ping);
		}
	}

	if (connecting)
	{
		auto status = connection.connect(ROV, connectionPort);
		if (status == sf::TcpSocket::Status::Error)
		{
			connecting = false;
			connected = false;
			// error
		}
		else
		{
			connecting = false;
			connected = true;
		}

	}
}

void Core::Network::send_packet(PacketTypes t, void * data, size_t size)
{
	if (connected)
	{
		switch(t)
		{
		case PacketTypes::Move:
		{
			sf::Socket::Status status;
			unsigned char buff[100];
			buff[0] = static_cast<unsigned char>(t);
			std::memcpy(&buff[1], data, size);
			do
			{
				status = connection.send(buff, size + 1);
			} while (status == sf::Socket::Status::Partial);
			if (status == sf::Socket::Disconnected)
			{
				connected = false;
			}
			break;
		}
		case PacketTypes::Ping:
			pingClock.restart();
			// Nothing special needed for nay of these packets currently except ping packet and move packet
		default:
		{
			sf::Socket::Status status;
			do
			{
				status = connection.send(&t, 1);
			} while (status == sf::Socket::Status::Partial);
			if (status == sf::Socket::Disconnected)
			{
				connected = false;
			}
			break;
		}
		}
	}
}

void Core::Network::process_packet(PacketTypes type)
{
	switch (type)
	{
	case PacketTypes::Ping:
	{
		if (pingCounter == pingWindow)
			pingCounter = 0;
		pingvals[pingCounter++] = pingClock.getElapsedTime();
		GlobalContext::get_engine()->add_event(incoming);
		break;
	}
	case PacketTypes::Video:
	{
		incoming->f.data = buffer.data();
		GlobalContext::get_engine()->add_event(incoming);
		break;
	}	
	case PacketTypes::Temperature:
	{
		double temp;
		temp = *reinterpret_cast<double *>(&buffer[0]);
		incoming->t.temp = temp;
		GlobalContext::get_engine()->add_event(incoming);
		break;
	}
	case PacketTypes::Pressure:
	{
		double pressure;
		pressure = *reinterpret_cast<double *>(&buffer[0]);
		incoming->p.pressure = pressure;
		GlobalContext::get_engine()->add_event(incoming);
		break;
	}
	default:
		break;
	}
	incoming = nullptr;
}

void Core::Network::disconnect()
{
	connection.disconnect();
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

void Core::Network::connect_to_host(sf::IpAddress addr)
{
	disconnect();
	connecting = true;
	ROV = addr;
}

Core::Network::~Network()
{
	broadcast.unbind();
	connection.disconnect();
}
