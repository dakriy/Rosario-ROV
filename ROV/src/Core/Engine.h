#pragma once

#include "EventHandler.h"
#include <vector>
#include <queue>
#include <mutex>
#include <memory>
#include "../Sensors/Sensor.h"

namespace Core
{

	class Event
	{
	public:
		struct VideoFrame
		{
			unsigned len;
			const sf::Uint8 * data;
		};

		struct Temperature
		{
			double temp;
		};

		struct Pressure
		{
			double pressure;
		};

		struct SensorsRequested
		{
			float frequency;
			std::vector<Sensor::SensorInfo::Sensor> sensors;
		};

		enum EventType
		{
			PingReceived,
			DataRequested,
			Disconnected,
			Shutdown,

			Count
		};

		EventType type = Count;

		union
		{
			VideoFrame f;
			Temperature t;
			Pressure p;
			SensorsRequested r;
		};

		explicit Event(EventType t) : type(t) {}
		Event() = default;
	};

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
