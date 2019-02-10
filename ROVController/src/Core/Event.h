#pragma once

/**
 * @file Event.h
 * Class definition Global Event Handler
 * It handles hooking and unhooking
 * event callbacks from the function chain
 *
 * @author dakriy
 */

#include <vector>
#include <SFML/Window/Event.hpp>
#include <functional>
#include <array>
#include "../Globals.h"
#include <iostream>

namespace Core
{
	/**
	 * Event hook struct
	 * Contains the attached events and
	 * the functor callback
	 */
	struct EventHook
	{
		std::array<bool, sf::Event::EventType::Count> event_type_enable_flag = { false };
		EVENT_FUNC_TYPE callback;
	};

	class EventHandler
	{
		// Event hook chain to be processed
		std::vector<EventHook*> event_hooks_;
	public:
		/**
		 * EventHandler Constructor
		 * 
		 * Sets the global context.
		 */
		EventHandler();

		/**
		 * Processes all callbacks attached to event e
		 * 
		 * @param e the event that happened
		 */
		void handle_event(sf::Event* e);

		/**
		 * Adds a callback to be fired on any event
		 * 
		 * @param callback Callback to be processed on an event
		 * @return Index to the hook used for unhooking
		 */
		EVENT_FUNC_INDEX add_event_callback_for_all_events(const EVENT_FUNC_TYPE& callback);

		/**
		 * Unhooks an index from all attached events
		 * 
		 * @param index The returned event function index from an add call
		 */
		void unhook_event_callback_for_all_events(EVENT_FUNC_INDEX index);

		/**
		 * Adds a callback to the event chain for a list of events.
		 * 
		 * @param callback the function callback
		 * @param types paramter packed event type list
		 */
		template<typename... EventTypes>
		EVENT_FUNC_INDEX add_event_callback(EVENT_FUNC_TYPE callback, EventTypes ... types);

		/**
		 * Unhooks a callback from the passed in events
		 * 
		 * @param index The index from the add call to remove from the passed in events
		 * @param types paramter packed event type list to unsubscribe from
		 */
		template<typename... EventTypes>
		void unhook_event_callback(EVENT_FUNC_INDEX index, EventTypes ... types);

		/**
		 * EventHandler Destructor
		 * 
		 * Removes the reference to the global context.
		 */
		~EventHandler();
	};


	template<typename... EventTypes>
	inline EVENT_FUNC_INDEX EventHandler::add_event_callback(EVENT_FUNC_TYPE callback, EventTypes ... eventtypes)
	{
		auto hook = new EventHook;
		hook->callback = callback;

		for (auto e : { eventtypes... })
		{
			if (e < hook->event_type_enable_flag.size())
				hook->event_type_enable_flag[e] = true;
		}

		// Add the hook onto the list to keep track of it for later
		this->event_hooks_.push_back(hook);

		return hook;
	}

	template <typename... EventTypes>
	inline void EventHandler::unhook_event_callback(EVENT_FUNC_INDEX index, EventTypes... eventtypes)
	{
		if (index == nullptr)
			return;

		// Find a non const pointer so we can do operations on it
		const auto it = std::find(this->event_hooks_.begin(), this->event_hooks_.end(), index);

		// If it was not found just return, somebody passed us one that we didn't make, or that was deleted.
		if (it == event_hooks_.end())
			return;

		// Turn off all event flags requested.
		for (auto e : { eventtypes... })
			if (e < (*it)->event_type_enable_flag.size())
				(*it)->event_type_enable_flag[e] = false;

		
		// Go through each table position to see if it's active.
		// If they are all inactive we can delete the EventHook.
		for (auto & flag : (*it)->event_type_enable_flag)
			if (flag)
				return;

		// If all flags are false, delete it.
		delete *it;
		*it = nullptr;
		this->event_hooks_.erase(it);
	}
}
