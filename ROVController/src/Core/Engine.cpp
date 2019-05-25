#include "Engine.h"
// This needs to be in here, because if it is placed in a header file you get an include loop
#include "GlobalContext.h"
#include <cassert>
#include "../Frames/TitleFrame.h"
#include "../Globals.h"
#include "../Utilities/Utilities.h"
#include "../Factories/PacketFactory.h"
#include <imgui-SFML.h>
#include <imgui.h>

void Core::Engine::Events()
{
	sf::Event event {
		sf::Event::EventType::Count
	};
	while (window_->pollEvent(event)) {
		// Let ImGUI have a round at the event
		ImGui::SFML::ProcessEvent(event);

		if (ImGui::IsAnyItemFocused() || ImGui::IsAnyItemActive()) {
			// Don't process it on our end as it is ImGui's to handle...
			continue;
		}

		ev_->handle_event(&event);
	}
	
	ProcessCustomEvents();
}

void Core::Engine::Update()
{
	auto dt = sf::Time::Zero;
	
	// Update ImGUI
	ImGui::SFML::Update(*window_, dt += rate_clock_.restart());

//	ImGui::ShowDemoWindow();
	
	// Update all frames except paused ones.
	for (auto &f : frame_stack_)
		if(!f->isPaused())
			f->update(dt += rate_clock_.restart());

	for (auto u : update_stack_)
		u->update(dt += rate_clock_.restart());
}

void Core::Engine::Render()
{
	window_->clear();

	auto t = false;
	
	for (auto & f : frame_stack_)
	{
		t = t || !f->isHidden();
		if (!f->isHidden())
			window_->draw(*f);
	}
	// Make sure all frames are not hidden.
	assert(t);
	
	// Render imgui on top
	ImGui::SFML::Render(*window_);

	window_->display();
}

void Core::Engine::ProcessCustomEvents()
{
	// Not locking *should* be safe because we always lock before we do any actual writing
	while (!core_events_.empty()) {
		coreEventHandlerLock.lock();
		// Pull off event as quick as possible while we have the lock
        std::unique_ptr<Core::Event> e = std::move(core_events_.front());
		core_events_.pop();
		// Release control of the lock
		coreEventHandlerLock.unlock();
		cev_->handle_event(e.get());
	}
}

void Core::Engine::ProcessFrameAction(FAction& f_action)
{
	switch (f_action.action)
	{
	case PopFrame:
		frame_stack_.pop_back();
		frame_stack_.back()->show();
		frame_stack_.back()->unpause();
		break;
	case PushFrame:
		frame_stack_.emplace_back(std::move(f_action.frame));
		break;
	case ReplaceTopFrame:
		frame_stack_.pop_back();
		frame_stack_.emplace_back(std::move(f_action.frame));
	case FrameActionCount:
	default:
		break;
	}
}

Core::Engine::Engine(sf::RenderWindow* w, EventHandler<sf::Event, sf::Event::EventType::Count>* ev, EventHandler<Core::Event, Core::Event::EventType::Count>* cev, sf::Clock* glbClk)
{
	assert(w);
	assert(ev);
	assert(cev);
	assert(glbClk);
	window_ = w;
	ev_ = ev;
	cev_ = cev;
	global_clock_ = glbClk;

	// Enable vertical sync enabled. Don't do framerate
	window_->setVerticalSyncEnabled(true);
	// Initialize it to the first main menu frame.
	frame_stack_.emplace_back(std::make_unique<Frames::TitleFrame>());

	messageLog = cev_->add_event_callback([&](const Core::Event * e) -> bool {
		auto message = std::get<std::string>(e->data);
		GlobalContext::get_log()->AddLog("ROV says: %s\n", message.c_str());
		return false;
	}, Core::Event::NewMessage);

	batteryLog = cev_->add_event_callback([&](const Core::Event * e) -> bool {
		auto percent = std::get<float>(e->data);
		GlobalContext::get_log()->AddLog("Battery is at %f percent", percent);
		return false;
	}, Core::Event::BatteryUpdate);

	GlobalContext::set_engine(this);
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

	// Process frame swapping
	for (auto& element : frame_action_list_)
		ProcessFrameAction(element);

	// If we changed frames, we need to update again.
	if (!frame_action_list_.empty())
	{
		// Need to end imgui frame before we update, otherwise it thinks we skipped a frame, which... ok... That's kinda fair
		// I could probably do this slightly better... But I haven't thought of a better way yet.
		ImGui::EndFrame();
		Update();
	}

	frame_action_list_.clear();

	Render();
}

void Core::Engine::frame_action(FrameAction action, Frames::IFrame* frame)
{
	assert(action < FrameActionCount && action >= 0);
	
	frame_action_list_.emplace_back(action, frame);
}

Core::Engine::~Engine()
{
	cev_->unhook_event_callback_for_all_events(batteryLog);
	cev_->unhook_event_callback_for_all_events(messageLog);
	GlobalContext::clear_engine();
}

void Core::Engine::addUpdateableEntitiy(Interfaces::IUpdateable * updateable) {
	update_stack_.push_back(updateable);
}

void Core::Engine::removeUpdateableEntity(Interfaces::IUpdateable *updateable) {
	update_stack_.erase(std::remove(update_stack_.begin(), update_stack_.end(), updateable), update_stack_.end());
}

