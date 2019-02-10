#include "GlobalContext.h"

Core::Engine * GlobalContext::engine_ = nullptr;
sf::Clock * GlobalContext::global_clock_ = nullptr;
sf::RenderWindow * GlobalContext::window_ = nullptr;
Core::EventHandler * GlobalContext::event_handler_ = nullptr;
Core::Network * GlobalContext::network_ = nullptr;

sf::Clock * GlobalContext::get_clock()
{
	return global_clock_;
}

void GlobalContext::set_clock(sf::Clock*c)
{
	if (global_clock_ == nullptr && c != nullptr)
		global_clock_ = c;
}

sf::RenderWindow * GlobalContext::get_window()
{
	return window_;
}

void GlobalContext::set_window(sf::RenderWindow * w)
{
	if (window_ == nullptr && w != nullptr)
		window_ = w;
}

Core::EventHandler * GlobalContext::get_event_handler()
{
	return event_handler_;
}

Core::Network* GlobalContext::get_network()
{
	return network_;
}

void GlobalContext::set_network(Core::Network* ptr)
{
	if (network_ == nullptr && ptr != nullptr)
		network_ = ptr;
}

void GlobalContext::clear_network()
{
	network_ = nullptr;
}

void GlobalContext::set_event_handler(Core::EventHandler * e)
{
	if (event_handler_ == nullptr && e != nullptr)
		event_handler_ = e;
}

Core::Engine* GlobalContext::get_engine()
{
	return engine_;
}

void GlobalContext::set_engine(Core::Engine* e)
{
	if (e != nullptr && engine_ == nullptr)
		engine_ = e;
}

void GlobalContext::clear_window()
{
	window_ = nullptr;
}

void GlobalContext::clear_clock()
{
	global_clock_ = nullptr;
}

void GlobalContext::clear_event_handler()
{
	event_handler_ = nullptr;
}

void GlobalContext::clear_engine()
{
	engine_ = nullptr;
}
