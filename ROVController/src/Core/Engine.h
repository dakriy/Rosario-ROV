#pragma once

/**
 * @file Engine.h
 * Class definition for the render engine
 * It handles frame actions and rendering
 *
 * @author dakriy
 */

#include "../Frames/IFrame.h"
#include "Event.h"
#include <vector>
#include <list>

class DebugTerminal;

namespace Core
{

	class Event
	{
	public:
		struct VideoFrame
		{
			char * data;
			size_t length;
		};

		struct Temperature
		{
			float temp;
		};

		struct Pressure
		{
			float pressure;
		};

		enum EventType
		{
			VideoFrameReceived,
			TemperatureReceived,
			PressureReceived,

			Count
		};

		EventType type = Count;

		union
		{
			VideoFrame f;
			Temperature t;
			Pressure p;
		};

		Event(EventType t) : type(t){}
		Event() = default;
	};

	/**
	 * List of actions we can do to a frame
	 * Must end with FrameActionCount
	 */
	enum FrameAction
	{
		PopFrame,
		PushFrame,
		ReplaceTopFrame,
		FrameActionCount,
	};

	/**
	 * Short for Frame Action.
	 * It holds the action and the frame getting the action.
	 */
	typedef struct FAction
	{
		FAction() = default;
		FAction(FrameAction a, Frames::IFrame* f) : action(a), frame(f){}
		FrameAction action = static_cast<FrameAction>(0);
		Frames::IFrame* frame = nullptr;
	} FAction;

	class Engine
	{
	protected:
		// Frames currently on the stack
		std::vector<Frames::IFrame *> frame_stack_;

		std::list<Core::Event> core_events_;

		// Event processor
		void Events();

		// Frame updater
		void Update();

		// Frame renderer
		void Render();

		/**
		 * Process a frame action
		 * 
		 * @param f_action the frame action to do
		 */
		void ProcessFrameAction(FAction& f_action);

		// The global render window
		sf::RenderWindow * window_;

		// Rate clock to get the current dt from last frame render off of
		sf::Clock rate_clock_;

		// Global clock to keep track of time passing when updating frames
		sf::Clock * global_clock_;

		// Frame push queue
		std::vector<Frames::IFrame *> push_frame_; 

		// Frame action queue
		std::vector<FAction> frame_action_list_;

		EventHandler<sf::Event, sf::Event::EventType::Count>* ev_;
		EventHandler<Core::Event, Core::Event::EventType::Count>* cev_;

	public:
		/**
		 * Engine Constructor
		 * 
		 * @param w window to render onto
		 * @param ev event hander to process events off of
		 * @param cev core event handler to process core events off of
		 * @param glbClk global clock to keep time off of
		 */
		Engine(sf::RenderWindow * w, EventHandler<sf::Event, sf::Event::EventType::Count> * ev, EventHandler<Core::Event, Core::Event::EventType::Count> * cev, sf::Clock * glbClk);

		/**
		 *
		 */
		void add_event(Core::Event e);

		/**
		 * Main loop. Order is:
		 * 
		 * 1. Process events
		 * 2. Update frame
		 * 3. Process any frame swapping
		 * 4. Update again if needed after any frame swapping
		 * 5. Draw/Render the screen
		 */
		void Loop();

		/**
		 * Take an action on a frame.
		 * This will take ownership of the frame and handle deletion if a delete was requested
		 * An update can finish after calling this even if a delete was requested it doesn't
		 * take effect right away. The frame action will happen after the update finishes,
		 * so the current update can finish out and do whatever it needs to do.
		 * 
		 * @param action The action to take
		 * @param frame The frame to act upon
		 */
		void frame_action(FrameAction action, Frames::IFrame* frame = nullptr);

		/**
		 * Engine destructor.
		 */
		~Engine();
	};
}
