#pragma once

#include "Event.h"
#include <vector>
#include <queue>
#include <mutex>

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

		enum EventType
		{
			PingReceived,
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
		};

		explicit Event(EventType t) : type(t) {}
		Event() = default;
		~Event();
	};

	class Engine
	{
	protected:

		std::queue<Core::Event*> core_events_;

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
		void add_event(Core::Event *e);

		void Loop();

		/**
		 * Engine destructor.
		 */
		~Engine();
	};
}
