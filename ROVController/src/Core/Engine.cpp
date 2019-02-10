#include "Engine.h"
// This needs to be in here, because if it is placed in a header file you get an include loop
#include "GlobalContext.h"
#include <cassert>
#include "../Frames/TitleFrame.h"
#include "../Globals.h"
#include "../Utilities.h"
#include <imgui-SFML.h>

void Core::Engine::Events() const
{
	sf::Event event;
	while (window_->pollEvent(event)) {
		ev_->handle_event(&event);
	}
}

void Core::Engine::Update()
{	
	auto dt = sf::Time::Zero;
	
	// Update ImGUI
	ImGui::SFML::Update(*window_, dt += rate_clock_.restart());
	
	// Update all frames except paused ones.
	for (auto f : frame_stack_)
		if(!f->isPaused())
			f->update(dt += rate_clock_.restart());
}

void Core::Engine::Render()
{
	window_->clear();

	auto t = false;
	
	for (auto f : frame_stack_)
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

void Core::Engine::ProcessFrameAction(FAction& f_action)
{
	switch (f_action.action)
	{
	case PopFrame:
		delete frame_stack_.back();
		frame_stack_.pop_back();
		frame_stack_.back()->show();
		frame_stack_.back()->unpause();
		break;
	case PushFrame:
		frame_stack_.emplace_back(f_action.frame);
		break;
	case ReplaceTopFrame:
		delete frame_stack_.back();
		frame_stack_.pop_back();
		frame_stack_.emplace_back(f_action.frame);
	case FrameActionCount:
	default:
		break;
	}
}

Core::Engine::Engine(sf::RenderWindow* w, EventHandler* ev, sf::Clock* glbClk)
{
	assert(w);
	assert(ev);
	assert(glbClk);
	window_ = w;
	ev_ = ev;
	global_clock_ = glbClk;

	// Set default framerate to 60.
	window_->setFramerateLimit(60);
	// Initialize it to the first main menu frame.
	let firstFrame = new Frames::TitleFrame;
	assert(firstFrame);
	if (!firstFrame)
		QuitWithError("Could not initialze the game", EXIT_FAILURE);
	frame_stack_.emplace_back(firstFrame);

	GlobalContext::set_engine(this);
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
		Update();

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
	while (!frame_stack_.empty())
	{
		delete frame_stack_.back();
		frame_stack_.pop_back();
	}

	GlobalContext::clear_engine();
}
