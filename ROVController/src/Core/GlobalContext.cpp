#include "GlobalContext.h"

Core::Engine * GlobalContext::engine_ = nullptr;
sf::Clock * GlobalContext::global_clock_ = nullptr;
sf::RenderWindow * GlobalContext::window_ = nullptr;
Core::EventHandler<sf::Event, sf::Event::EventType::Count> * GlobalContext::event_handler_ = nullptr;
Core::EventHandler<Core::Event, Core::Event::EventType::Count> * GlobalContext::internal_event_handler_ = nullptr;
Core::Network * GlobalContext::network_ = nullptr;
Device * GlobalContext::device_ = nullptr;
AppLog * GlobalContext::log_ = nullptr;

sf::Clock * GlobalContext::get_clock()
{
	return global_clock_;
}

void GlobalContext::set_clock(sf::Clock*c)
{
	if (global_clock_ == nullptr && c)
		global_clock_ = c;
}

sf::RenderWindow * GlobalContext::get_window()
{
	return window_;
}

void GlobalContext::set_window(sf::RenderWindow * w)
{
	if (window_ == nullptr && w)
		window_ = w;
}

Core::EventHandler<sf::Event, sf::Event::EventType::Count> * GlobalContext::get_event_handler()
{
	return event_handler_;
}

Core::EventHandler<Core::Event, Core::Event::EventType::Count>* GlobalContext::get_core_event_handler()
{
	return internal_event_handler_;
}

Core::Network* GlobalContext::get_network()
{
	return network_;
}

void GlobalContext::set_network(Core::Network* ptr)
{
	if (network_ == nullptr && ptr)
		network_ = ptr;
}

void GlobalContext::clear_network()
{
	network_ = nullptr;
}

void GlobalContext::set_event_handler(Core::EventHandler<sf::Event, sf::Event::EventType::Count> * e)
{
	if (event_handler_ == nullptr && e)
		event_handler_ = e;
}

void GlobalContext::set_core_event_handler(Core::EventHandler<Core::Event, Core::Event::EventType::Count>* ptr)
{
	if (internal_event_handler_ == nullptr && ptr)
		internal_event_handler_ = ptr;
}

Core::Engine* GlobalContext::get_engine()
{
	return engine_;
}

void GlobalContext::set_engine(Core::Engine* e)
{
	if (e && engine_ == nullptr)
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

void GlobalContext::clear_core_event_handler()
{
	internal_event_handler_ = nullptr;
}

void GlobalContext::clear_engine()
{
	engine_ = nullptr;
}

Device *GlobalContext::get_device() {
	return device_;
}

void GlobalContext::set_device(Device *dev) {
	if (device_ == nullptr && dev)
		device_ = dev;
}

void GlobalContext::clear_device() {
	device_ = nullptr;
}

AppLog *GlobalContext::get_log() {
	return log_;
}

void GlobalContext::set_log(AppLog *log) {
	if (log_ == nullptr && log)
		log_ = log;
}

void GlobalContext::clear_log() {
	log_ = nullptr;
}
