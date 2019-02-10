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
#include "Event.h"
#include "Engine.h"
#include "Network.h"

class GlobalContext
{
private:
	// Engine instance
	static Core::Engine * engine_;

	// Global clock instance
	static sf::Clock * global_clock_;

	// Global render window instance
	static sf::RenderWindow * window_;

	// Global event handler instance
	static Core::EventHandler * event_handler_;

	// Networkmodule
	static Core::Network * network_;

public:
	/**
	 * Returns the global clock if it exists
	 */
	static sf::Clock * get_clock();
	
	/**
	 * Returns the engine if it exists
	 */
	static Core::Engine * get_engine();

	/**
	 * Returns the global render window
	 */
	static sf::RenderWindow * get_window();

	/**
	 * Returns the global event handler
	 */
	static Core::EventHandler * get_event_handler();

	/**
	 * Returns the network module
	 */
	static Core::Network * get_network();


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

	/**
	 * Sets the global render window
	 * 
	 * @param ptr Window pointer
	 */
	static void set_window(sf::RenderWindow * ptr);

	/**
	 * Sets the global event handler
	 * 
	 * @param ptr Event hander pointer
	 */
	static void set_event_handler(Core::EventHandler * ptr);

	/**
	 * Sets the global network module
	 * 
	 * @param ptr Network object pointer
	 */
	static void set_network(Core::Network * ptr);


	/**
	 * Clears the global clock
	 */
	static void clear_clock();

	/**
	 * clears the global window
	 */
	static void clear_window();

	/**
	 * clears the globl engine
	 */
	static void clear_engine();

	/**
	 * Clears the global event handler
	 */
	static void clear_event_handler();

	/**
	 * Clears the global network object
	 */
	static void clear_network();
};
