#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include "Frames/MainMenuFrame.h"
#include "Factories/TextFactory.h"
#include "Utilities/Utilities.h"
#include "Core/GlobalContext.h"
#include "Utilities/Device.h"

bool LoadResources()
{
	// Load fonts
	if (!Factory::TextFactory::load_assets()) {
		return false;
	}
	if (!Frames::MainMenuFrame::background_tex.loadFromFile("Resources/background.png")) {
		return false;
	}
	return true;
}

int main(int argc, char* argv[])
{
	// SEED
	srand(GetSeed("A seed, I don't really care what goes here atm... it gets hashed anyway"));

	sf::Clock elapsedTime;
	GlobalContext::set_clock(&elapsedTime);

	auto vidmode = sf::VideoMode::getFullscreenModes()[0];
//	auto vidmode = sf::VideoMode::getDesktopMode();
//	auto vidstyle = sf::Style::Default;
	auto vidstyle = sf::Style::Fullscreen;

	if (!vidmode.isValid())
	{
		vidmode = sf::VideoMode::getDesktopMode();
		vidstyle = sf::Style::Default;
		if (!vidmode.isValid())
		{
			vidmode = sf::VideoMode(100, 100);
			if (!vidmode.isValid())
				QuitWithError("Could not find a compatible Video Mode", EXIT_FAILURE);
		}
	}

	if (!LoadResources())
	{
		QuitWithError("Required resources failed to load", EXIT_FAILURE);
	}

	sf::ContextSettings settings;

	settings.antialiasingLevel = 8;
	

	// Must be kept alive in the root scope for everything else to be able to access it.
	Core::EventHandler<sf::Event, sf::Event::EventType::Count> eventHandler;
	Core::EventHandler<Core::Event, Core::Event::EventType::Count> coreEventHandler;
	GlobalContext::set_event_handler(&eventHandler);
	GlobalContext::set_core_event_handler(&coreEventHandler);

	// Start up main rendering window

	sf::RenderWindow Window(vidmode, APP_NAME, vidstyle, settings);

	ImGui::SFML::Init(Window);

	GlobalContext::set_window(&Window);

	Core::Engine engine(&Window, &eventHandler, &coreEventHandler, &elapsedTime);

	AppLog log;

	const auto ev1handle = eventHandler.add_event_callback([&](const sf::Event* e) -> bool {
		Window.close();
		return true;
	}, sf::Event::Closed);

	Core::Network net;

	GlobalContext::set_network(&net);

	// This just needs to exist, also it needs the network so it gets setup after network.
	Device d;

	while (Window.isOpen())
	{
		engine.Loop();
	}

	eventHandler.unhook_event_callback(ev1handle, sf::Event::Closed);

	ImGui::SFML::Shutdown();
	return 0;
}
