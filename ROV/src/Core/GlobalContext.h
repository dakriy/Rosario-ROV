#pragma once

/**
 * @file GlobalContext.h
 * Class definition for the global context
 * It handles retrieving globally important
 * objects such as but not limited to the render 
 * window, the rendering engine, or the event handler.
 *
 * @author dakriy
 */

#include <SFML/Graphics/RenderWindow.hpp>
#include "Engine.h"
#include "../Network/Network.h"

class GlobalContext
{
private:
	// Engine instance
	static Core::Engine * engine_;

	// Global clock instance
	static sf::Clock * global_clock_;

	static Core::EventHandler<Core::Event, Core::Event::EventType::Count> * internal_event_handler_;

	// Network module
	static Network::Network * network_;

public:
	/**
	 * Returns the global clock if it exists
	 */
	static sf::Clock * get_clock();
	
	/**
	 * Returns the engine if it exists
	 */
	static Core::Engine * get_engine();

	static Core::EventHandler<Core::Event, Core::Event::EventType::Count> * get_core_event_handler();

	/**
	 * Returns the network module
	 */
	static Network::Network * get_network();


	/**
	 * Sets the global clock
	 * 
	 * @param ptr Clock pointer
	 */
	static void set_clock(sf::Clock * ptr);

	/**
	 * Sets the global engine
	 * 
	 * @param ptr Engine pointer
	 */
	static void set_engine(Core::Engine * ptr);

	static void set_core_event_handler(Core::EventHandler<Core::Event, Core::Event::EventType::Count> * ptr);

	/**
	 * Sets the global network module
	 * 
	 * @param ptr Network object pointer
	 */
	static void set_network(Network::Network * ptr);


	/**
	 * Clears the global clock
	 */
	static void clear_clock();

	/**
	 * clears the globl engine
	 */
	static void clear_engine();

	static void clear_core_event_handler();

	/**
	 * Clears the global network object
	 */
	static void clear_network();
};
