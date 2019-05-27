#pragma once

#include "EventHandler.h"
#include <vector>
#include <queue>
#include <mutex>
#include <memory>
#include "Event.h"
#include "../Sensors/Pressure.h"
#include "../Sensors/Lux.h"
#include "../Sensors/Temperature.h"
#include "../Sensors/Battery.h"
#include "../Utilities/Existence.h"
#include "../Sensors/Conductivity.h"
#include <csv/writer.hpp>


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
		const unsigned defaultTimeout = 50;

		std::array<std::unique_ptr<Sensor::Sensor>, 4> sensors = {
				std::make_unique<Sensor::Temperature>(),
				std::make_unique<Sensor::Lux>(),
				std::make_unique<Sensor::Pressure>(),
				std::make_unique<Sensor::Conductivity>(),
//				std::make_unique<Sensor::InternalTemperature>(),
//				std::make_unique<Sensor::InternalPressure>(),
//				std::make_unique<Sensor::InternalHumidity>()
		};

		std::vector<std::unique_ptr<Existence>> existences;

		// Requested sensor vars
		sf::Clock dataTimer;
		float sensorFrequency;
		// Vector of indexes into the sensors vector of the wanted sensors...
		// "By default, use vector when you need a container" - Bjarne Stroustrup.
		std::vector<size_t> requestedSensors;
		bool missionInProgress = false;
		bool readyForConversion = true;

		EVENT_FUNC_INDEX_CORE watchForRequest = nullptr;
		EVENT_FUNC_INDEX_CORE watchForRequestStop = nullptr;
		EVENT_FUNC_INDEX_CORE sensorRequest = nullptr;
		EVENT_FUNC_INDEX_CORE connectHook = nullptr;

		bool localInstance = false;

		const std::string& localFile;

	public:
		/**
		 * Engine Constructor
		 *
		 * @param cev core event handler to process core events off of
		 * @param local Specifies whether to record local or not
		 * @param file the name of the file to record local, empty string if not
		 */
		explicit Engine(EventHandler<Core::Event, Core::Event::EventType::Count> * cev, bool local, const std::string& file);

		/**
		 *
		 */
		void add_event(std::unique_ptr<Event> e);

		void Loop();

		void addExistence(std::unique_ptr<Existence> ex);

		void log(const std::string& message);

		/**
		 * Engine destructor.
		 */
		~Engine();
	};
}
