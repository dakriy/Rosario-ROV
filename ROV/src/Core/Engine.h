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
	// There is a copy of this in the ROVController that needs to be kept up to date with this one
	enum class ROVState {
		Idle,
		Connected,
		ConnectedPaused,
		MissionConnected,
		MissionDisconnected,
		COUNT
	};

	class Engine
	{
	protected:
		// Event system variables
		std::queue<std::unique_ptr<Core::Event>> core_events_;

		EventHandler<Core::Event, Core::Event::EventType::Count>* cev_;

		std::mutex coreEventHandlerLock;


		// Event processor
		void Events();

		// Frame updater
		void Update();

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

		sf::Time startTime = sf::Time::Zero;

		// Set to one by default
		float sensorFrequency = 1;
		// Vector of indexes into the sensors vector of the wanted sensors...
		// "By default, use vector when you need a container" - Bjarne Stroustrup.
		std::vector<size_t> requestedSensors;
		bool readyForConversion = true;
		bool sensorsInitialized = false;

		std::vector<EVENT_FUNC_INDEX_CORE> hooks;

		std::string localFile = "";

		void setHooks();

		// If called when sensors are not ready, it will block until they sensors are ready
		// determined by sensorFrequency
		std::vector<std::pair<sf::Uint8, float>> querySensors(bool allSensors = false);
		void recordDataSet(const std::vector<std::pair<sf::Uint8, float>> & data);

		// Requires requestedSensors to be set first
		void initializeRecordFile(const std::string &fileName);

		// State keeping variables
		ROVState rovState = ROVState::Idle;
	public:
		/**
		 * Engine Constructor
		 *
		 * @param cev core event handler to process core events off of
		 * @param local Specifies whether to record local or not
		 * @param file the name of the file to record local, empty string if not
		 */
		explicit Engine(EventHandler<Core::Event, Core::Event::EventType::Count> * cev);

		/**
		 *
		 */
		void add_event(std::unique_ptr<Event> e);

		void Drive();

		void addExistence(std::unique_ptr<Existence> ex);

		void log(const std::string& message);

		/**
		 * Engine destructor.
		 */
		~Engine();
	};
}
