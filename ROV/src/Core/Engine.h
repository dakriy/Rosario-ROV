#pragma once

#include "EventHandler.h"
#include <vector>
#include <queue>
#include <mutex>
#include <memory>
#include "Event.h"

namespace Core
{
	class Engine
	{
	protected:

		std::queue<std::unique_ptr<Core::Event>> core_events_;

		// Event processor
		void Events();

		// Frame updater
		void Update();

		EventHandler<Core::Event, Core::Event::EventType::Count>* cev_;

		std::mutex coreEventHandlerLock;

		// In milliseconds
		static const unsigned defaultTimeout = 250;


		std::array<Sensor::Sensor, 5> sensors = {

		};


		// Requested sensor vars
		sf::Clock dataTimer;
		float sensorFrequency;
		// Vector of indexes into the sensors vector of the wanted sensors...
		// "By default, use vector when you need a container" - Bjarne Stroustrup.
		std::vector<size_t> requestedSensors;
		bool dataRequested = false;

		EVENT_FUNC_INDEX_CORE watchForRequest;

	public:
		/**
		 * Engine Constructor
		 *
		 * @param cev core event handler to process core events off of
		 */
		explicit Engine(EventHandler<Core::Event, Core::Event::EventType::Count> * cev);

		/**
		 *
		 */
		void add_event(std::unique_ptr<Event> e);

		void Loop();

		/**
		 * Engine destructor.
		 */
		~Engine();
	};
}
