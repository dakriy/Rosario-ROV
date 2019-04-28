#include "Engine.h"
// This needs to be in here, because if it is placed in a header file you get an include loop
#include "GlobalContext.h"
#include <cassert>
#include "../Factories/PacketFactory.h"

void Core::Engine::Events()
{
	// Not locking *should* be safe because we always lock before we do any actual writing
	while (!core_events_.empty()) {
		coreEventHandlerLock.lock();
		// Pull off event as quick as possible while we have the lock
		std::unique_ptr<Event> e = std::move(core_events_.front());
		core_events_.pop();
		// Release control of the lock
		coreEventHandlerLock.unlock();
		cev_->handle_event(e.get());
	}
}

void Core::Engine::Update()
{
	// Update here.

}

Core::Engine::Engine(EventHandler<Core::Event, Core::Event::EventType::Count>* cev)
{
	assert(cev);
	cev_ = cev;

	GlobalContext::set_engine(this);

	cev_->add_event_callback([&](const Event * e) -> bool {
		dataRequested = true;


		return false;
	}, Core::Event::DataRequested);
}

void Core::Engine::add_event(std::unique_ptr<Event> e)
{
	// Make sure we are not sent a nullptr or an invalid type
	if (e && e->type != Core::Event::EventType::Count) {
		coreEventHandlerLock.lock();
		core_events_.push(std::move(e));
		coreEventHandlerLock.unlock();
	}
}

void Core::Engine::Loop()
{
	Events();

	Update();
}

Core::Engine::~Engine()
{
	GlobalContext::clear_engine();
}