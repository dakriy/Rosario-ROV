#include "Engine.h"
// This needs to be in here, because if it is placed in a header file you get an include loop
#include "GlobalContext.h"
#include <cassert>
#include "../Frames/TitleFrame.h"
#include "../Globals.h"
#include "../Utilities/Utilities.h"
#include <imgui-SFML.h>
#include <imgui.h>

void Core::Engine::Events()
{
	sf::Event event;
	while (window_->pollEvent(event)) {
		// Let ImGUI have a round at the event
		ImGui::SFML::ProcessEvent(event);

		ev_->handle_event(&event);
	}
	
	ProcessCustomEvents();
}

void Core::Engine::Update()
{	
	auto dt = sf::Time::Zero;
	
	// Update ImGUI
	ImGui::SFML::Update(*window_, dt += rate_clock_.restart());

	// Connected window
	auto network = GlobalContext::get_network();
	if (network->isConnected())
	{
		std::string title = "Connected to ";
		title += network->connectedHost().toString();
		ImGui::Begin(title.c_str());
		ImGui::Text("Round Trip Ping: = %f ms", network->get_ping_time());
		if (ImGui::Button("Disconnect"))
		{
			network->disconnect();
		}
		if (ImGui::Button("Shutdown ROV"))
		{
			sf::Packet p;
			p << static_cast<unsigned char>(PacketTypes::Shutdown);
			network->send_packet(p, PacketTypes::Shutdown);
		}
		ImGui::End();
	}
	
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

void Core::Engine::ProcessCustomEvents()
{
	for (auto e : core_events_)
	{
		cev_->handle_event(e);
		delete e;
	}
	core_events_.clear();
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
	let firstFrame = new Frames::TitleFrame;
	frame_stack_.emplace_back(firstFrame);

	GlobalContext::set_engine(this);
}

void Core::Engine::add_event(Core::Event *e)
{
	if (e->type != Core::Event::EventType::Count)
		core_events_.emplace_back(e);
}

void Core::Engine::Loop()
{
	GlobalContext::get_network()->process_packets();
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

	// Process events again cause why not
	GlobalContext::get_network()->process_packets();
	ProcessCustomEvents();

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
