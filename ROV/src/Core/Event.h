#pragma once
#ifndef ROV_EVENT_H
#define ROV_EVENT_H

/**
 * @file Event.h
 * Class definition Global Event Handler
 * It handles hooking and unhooking
 * event callbacks from the function chain
 *
 * @author dakriy
 */

#include <vector>
#include <functional>
#include <array>
#include <iostream>
#include "../Network/NetworkConstants.h"

#define EVENT_FUNC_TYPE(T) std::function<bool(const T*)>
#define EVENT_FUNC_INDEX(T, S) const Core::EventHook<T, S>*

namespace Core
{
	/**
	 * Event hook struct
	 * Contains the attached events and
	 * the functor callback
	 */
	template<class T, int EventSize>
	struct EventHook
	{
		std::array<bool, EventSize> event_type_enable_flag = { false };
		EVENT_FUNC_TYPE(T) callback;
	};

	template<typename T, int EventSize>
	class EventHandler
	{
		// Event hook chain to be processed
		std::vector<EventHook<T, EventSize>*> event_hooks_;
	public:
		/**
		 * EventHandler Constructor
		 *
		 * Sets the global context.
		 */
		EventHandler() = default;

		/**
		 * Processes all callbacks attached to event e
		 *
		 * @param e the event that happened
		 */
		void handle_event(const T* e);

		/**
		 * Adds a callback to be fired on any event
		 *
		 * @param callback Callback to be processed on an event
		 * @return Index to the hook used for unhooking
		 */
		EVENT_FUNC_INDEX(T, EventSize) add_event_callback_for_all_events(const EVENT_FUNC_TYPE(T)& callback);

		/**
		 * Unhooks an index from all attached events
		 *
		 * @param index The returned event function index from an add call
		 */
		void unhook_event_callback_for_all_events(EVENT_FUNC_INDEX(T, EventSize) index);

		/**
		 * Adds a callback to the event chain for a list of events.
		 *
		 * @param callback the function callback
		 * @param types paramter packed event type list
		 */
		template<typename... EventTypes>
		EVENT_FUNC_INDEX(T, EventSize) add_event_callback(EVENT_FUNC_TYPE(T) callback, EventTypes ... types);

		/**
		 * Unhooks a callback from the passed in events
		 *
		 * @param index The index from the add call to remove from the passed in events
		 * @param types paramter packed event type list to unsubscribe from
		 */
		template<typename... EventTypes>
		void unhook_event_callback(EVENT_FUNC_INDEX(T, EventSize) index, EventTypes ... types);

		/**
		 * EventHandler Destructor
		 *
		 * Removes the reference to the global context.
		 */
		~EventHandler() = default;
	};

	template <typename T, int EventSize>
	void EventHandler<T, EventSize>::handle_event(const T* e)
	{
		// Process the regular application event chain
		for (auto hook : event_hooks_)
			if (hook && hook->event_type_enable_flag[static_cast<int>(e->type)])
				if (hook->callback(e))
					return;
	}

	template <typename T, int EventSize>
	EVENT_FUNC_INDEX(T, EventSize) EventHandler<T, EventSize>::add_event_callback_for_all_events(const EVENT_FUNC_TYPE(T)& callback)
	{
		// We take care of deletion and control everything about the pointer
		// So not going to worry about memory leaks.
		auto hook = new EventHook<T, EventSize>;

		// Set everything onto the hook.
		hook->callback = callback;
		for (auto & flag : hook->event_type_enable_flag)
			flag = true;

		this->event_hooks_.push_back(hook);

		return hook;
	}

	template <typename T, int EventSize>
	void EventHandler<T, EventSize>::unhook_event_callback_for_all_events(EVENT_FUNC_INDEX(T, EventSize) index)
	{
		// Already been taken care of, we done.
		if (index == nullptr)
			return;

		// Find a non const pointer so we can do operations on it
		const auto it = std::find(this->event_hooks_.begin(), this->event_hooks_.end(), index);

		// If it was not found just return, somebody passed us one that we didn't make, or that was deleted.
		if (it == event_hooks_.end())
			return;

		// Turn off all event flags
		for (auto & flag : (*it)->event_type_enable_flag)
			flag = false;

		// It's going to be useless now, so delete it.
		delete *it;
		*it = nullptr;

		this->event_hooks_.erase(it);
	}

	template <typename T, int EventSize>
	template <typename ... EventTypes>
	EVENT_FUNC_INDEX(T, EventSize) EventHandler<T, EventSize>::add_event_callback(EVENT_FUNC_TYPE(T) callback, EventTypes... types)
	{
		auto hook = new EventHook<T, EventSize>;
		hook->callback = callback;

		for (auto e : { types... })
		{
			if (e < hook->event_type_enable_flag.size())
				hook->event_type_enable_flag[e] = true;
		}

		// Add the hook onto the list to keep track of it for later
		this->event_hooks_.push_back(hook);

		return hook;
	}

	template <typename T, int EventSize>
	template <typename ... EventTypes>
	void EventHandler<T, EventSize>::unhook_event_callback(EVENT_FUNC_INDEX(T, EventSize) index, EventTypes... types)
	{
		if (index == nullptr)
			return;

		// Find a non const pointer so we can do operations on it
		const auto it = std::find(this->event_hooks_.begin(), this->event_hooks_.end(), index);

		// If it was not found just return, somebody passed us one that we didn't make, or that was deleted.
		if (it == event_hooks_.end())
			return;

		// Turn off all event flags requested.
		for (auto e : { types... })
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


#endif //ROV_EVENT_H
