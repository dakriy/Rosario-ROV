#include <wiringPi.h>
#include "Network/Network.h"
#include "Core/GlobalContext.h"
#include <stdio.h>
#include "Utilities/InputParser.h"
#include <string>
#include <stdlib.h>

int main(int argc, char **argv) {
	// Main Thread
	wiringPiSetup();

	// Initialize elapsed time clock
	sf::Clock elapsedTime;
	GlobalContext::set_clock(&elapsedTime);

	Core::EventHandler<Core::Event, Core::Event::EventType::Count> coreEventHandler;
	GlobalContext::set_core_event_handler(&coreEventHandler);

	Core::Engine engine(&coreEventHandler);

	Network::Network network;
	GlobalContext::set_network(&network);
	bool done = false;

	// Uncomment these to add passive event driven functionality
	// Right now these things are not used until we get the tether working
//	engine.addExistence(std::make_unique<Camera::Camera>());
//	engine.addExistence(std::make_unique<Power::Power>());
//	engine.addExistence(std::make_unique<Camera::ServoHalf>());
//	engine.addExistence(std::make_unique<Camera::ServoTray>());
//	engine.addExistence(std::make_unique<External::Light>());

	const auto hook = coreEventHandler.add_event_callback([&](const Core::Event * p) -> bool {
		done = true;
		// coincidentally this needs to be run as root to work, otherwise the Pi stays on and the application exits
		system("/sbin/shutdown -P now");
		return false;
	}, Core::Event::EventType::Shutdown);

	// DRIVE THE ENGINE VROOOM VROOOM
	// Ok this used to be called Loop, but I have to name it drive now...
	// Renaming it drive 6/26/19
	while (!done) {
		engine.Drive();
	}

	coreEventHandler.unhook_event_callback_for_all_events(hook);

	return 0;
}