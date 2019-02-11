#include "ViewFrame.h"
#include "../Core/GlobalContext.h"
#include <iostream>

Frames::ViewFrame::ViewFrame()
{
	frameHook = GlobalContext::get_core_event_handler()->add_event_callback([this](const Core::Event *e)->bool {
		std::cout << "IT WORKS YO" << std::endl;
		return true;
	}, Core::Event::EventType::PingReceived);
}

void Frames::ViewFrame::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}

void Frames::ViewFrame::update(const sf::Time& dt)
{
}

Frames::FrameType Frames::ViewFrame::get_type() const
{
	return View;
}

Frames::ViewFrame::~ViewFrame()
{
	GlobalContext::get_core_event_handler()->unhook_event_callback_for_all_events(frameHook);
}
