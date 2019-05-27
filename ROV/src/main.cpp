#include <wiringPi.h>
#include "Network/Network.h"
#include "Core/GlobalContext.h"
#include <stdio.h>
#include "Utilities/InputParser.h"
#include <string>
#include <stdlib.h>

int main(int argc, char **argv) {
	/*
	 * Plan:
	 * Two threads
	 *
	 * Using TCP cause its easier to do with images being larger than a UDP packet
	 *
	 * Network Thread:
	 * A. Setup UDP socket
	 * B. Setup TCP socket
	 * C. Before Established TCP connection loop
	 * 	1. Broadcast on the UDP broadcast address
	 * 	2. Wait for TCP connection, timeout of 3 seconds
	 * 	  a. on timeout restart the loop
	 * 	  b. on connection
	 * 	    i. destroy UDP socket
	 * 	    ii. initialize TCP connection
	 * D. After we establish TCP connection loop
	 * 	1. Block waiting for packets (with timeout maybe 500 ms, 250?)
	 * 	2. process incoming packet (If there is one)
	 * 	3. if too many timeouts we got disconnected probably, handle accordingly
	 * 	4. Send any packets in the to be sent queue
	 * 	5. Restart loop
	 *
	 *
	 * Main Thread
	 * Control path will go
	 * A. Start network handler
	 * B. Block on network not connected
	 * C. Set default timeout 250ms?
	 * D. Loop until stop request is received:
	 * 	1. Foreach wanted measurement:
	 * 	 a. Take measurement
	 * 	 b. Construct packet
	 * 	 c. Slap packet on send queue
	 * 	2. Calculate time to wait until next measurement
	 * 	3. Sleep that amount of time
	 *
	 * E. On stop request go back to waiting for connection
	 */

	InputParser input(argc, argv);

	bool localRecord = input.cmdOptionExists("-l") || input.cmdOptionExists("--local");

	if (!input.cmdOptionExists("-f") && localRecord) {
		printf("Please specify an output file when wanting to do a local record");
		exit(0);
	}

	// Main Thread
	wiringPiSetup();

	// Initialize elapsed time clock
	sf::Clock elapsedTime;
	GlobalContext::set_clock(&elapsedTime);

	Core::EventHandler<Core::Event, Core::Event::EventType::Count> coreEventHandler;
	GlobalContext::set_core_event_handler(&coreEventHandler);

	const std::string &fileName = input.getCmdOption("-f");

	Core::Engine engine(&coreEventHandler, localRecord, fileName);

	Network::Network network;
	GlobalContext::set_network(&network);
	bool done = false;

//	engine.addExistence(std::make_unique<Camera::Camera>());
//	engine.addExistence(std::make_unique<Power::Power>());
//	engine.addExistence(std::make_unique<Camera::ServoHalf>());
//	engine.addExistence(std::make_unique<Camera::ServoTray>());
//	engine.addExistence(std::make_unique<External::Light>());

	const auto hook = coreEventHandler.add_event_callback([&](const Core::Event * p) -> bool {
		done = true;
		system("/sbin/shutdown -P now");
		return false;
	}, Core::Event::EventType::Shutdown);

	while (!done) {
		engine.Loop();
	}

	coreEventHandler.unhook_event_callback_for_all_events(hook);

	return 0;
}