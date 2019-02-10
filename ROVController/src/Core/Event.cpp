#include "Event.h"
#include "GlobalContext.h"
#include <iostream>

Core::EventHandler::EventHandler()
{
	GlobalContext::set_event_handler(this);
}

void Core::EventHandler::handle_event(sf::Event* e)
{
	// Process the event chain
	for (auto hook : event_hooks_)
		if (hook && hook->event_type_enable_flag[e->type])
			if (hook->callback(e))
				return;
}

EVENT_FUNC_INDEX Core::EventHandler::add_event_callback_for_all_events(const EVENT_FUNC_TYPE& callback)
{
	// We take care of deletion and control everything about the pointer
	// So not going to worry about memory leaks.
	auto hook = new EventHook;

	// Set everything onto the hook.
	hook->callback = callback;
	for (auto & flag : hook->event_type_enable_flag)
		flag = true;

	this->event_hooks_.push_back(hook);

	return hook;
}

void Core::EventHandler::unhook_event_callback_for_all_events(EVENT_FUNC_INDEX index)
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

Core::EventHandler::~EventHandler()
{
	GlobalContext::clear_event_handler();
}
