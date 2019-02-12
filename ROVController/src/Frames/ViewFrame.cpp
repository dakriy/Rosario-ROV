#include "ViewFrame.h"
#include "../Core/GlobalContext.h"
#include <iostream>

Frames::ViewFrame::ViewFrame()
{
	frameHook = GlobalContext::get_core_event_handler()->add_event_callback([this](const Core::Event *e)->bool {
		image.create(e->f.w, e->f.h, e->f.data);
		if (!tex.loadFromImage(image))
		{
			std::cout << "NOOOOO" << std::endl;
			return false;
		}
		std::cout << "New Image" << std::endl;
		sprite.setTexture(tex);

		// Set scale only once
		if (!frame)
			sprite.scale(window_->getSize().x / (sprite.getLocalBounds().width), window_->getSize().y / sprite.getLocalBounds().height);

		frame = true;
		return true;
	}, Core::Event::EventType::VideoFrameReceived);
	GlobalContext::get_network()->send_packet(Core::PacketTypes::StartVideo);
}

void Frames::ViewFrame::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (frame)
		target.draw(sprite);
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
	GlobalContext::get_network()->send_packet(Core::PacketTypes::StopVideo);
	GlobalContext::get_core_event_handler()->unhook_event_callback_for_all_events(frameHook);
}
