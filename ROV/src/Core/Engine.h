#pragma once

#include "EventHandler.h"
#include <vector>
#include <queue>
#include <mutex>
#include <memory>
#include "Event.h"
#include "../Sensors/Pressure.h"

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
		const unsigned defaultTimeout = 250;


		std::array<std::unique_ptr<Sensor::Sensor>, 1> sensors = {
				std::make_unique<Sensor::Pressure>()
		};


		// Requested sensor vars
		sf::Clock dataTimer;
		float sensorFrequency;
		// Vector of indexes into the sensors vector of the wanted sensors...
		// "By default, use vector when you need a container" - Bjarne Stroustrup.
		std::vector<size_t> requestedSensors;
		bool missionInProgress = false;

		EVENT_FUNC_INDEX_CORE watchForRequest;
		EVENT_FUNC_INDEX_CORE sensorRequest;

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
