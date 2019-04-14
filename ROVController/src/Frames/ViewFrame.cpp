#include "ViewFrame.h"
#include "../Core/GlobalContext.h"
#include <iostream>
#include <imgui.h>

Frames::ViewFrame::ViewFrame()
{
	frameHook = GlobalContext::get_core_event_handler()->add_event_callback([this](const Core::Event *e)->bool {
		//image.create(e->f.w, e->f.h, e->f.data);
		if(!image.loadFromMemory(e->f.data, e->f.len))
		{
			std::cout << "NOOO but here" << std::endl;
			return false;
		}

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

	pressureHook = GlobalContext::get_core_event_handler()->add_event_callback([this](const Core::Event *e)->bool {
		pressure = e->p.pressure;
		return true;
	}, Core::Event::EventType::PressureReceived);

	temperatureHook = GlobalContext::get_core_event_handler()->add_event_callback([this](const Core::Event *e)->bool {
		temp = e->t.temp;
		return true;
	}, Core::Event::EventType::TemperatureReceived);


	sf::Packet vidP;
	sf::Packet pressP;
	sf::Packet tempP;
	vidP << static_cast<unsigned char>(Core::PacketTypes::StartVideo);
	pressP << static_cast<unsigned char>(Core::PacketTypes::StartPressure);
	tempP << static_cast<unsigned char>(Core::PacketTypes::StartTemp);
	GlobalContext::get_network()->send_packet(vidP, Core::PacketTypes::StartVideo);
	GlobalContext::get_network()->send_packet(pressP, Core::PacketTypes::StartPressure);
	GlobalContext::get_network()->send_packet(tempP, Core::PacketTypes::StartTemp);
}

void Frames::ViewFrame::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (frame)
		target.draw(sprite);
}

void Frames::ViewFrame::update(const sf::Time& dt)
{
	auto network = GlobalContext::get_network();
	ImGui::Begin("Access");
	if (ImGui::Button("Main Menu"))
	{
		GlobalContext::get_engine()->frame_action(Core::FrameAction::PopFrame);
	}
	ImGui::End();

	if (network->isConnected())
	{
		ImGui::Begin("Pressure");
		ImGui::Text("Pressure: = %f mbar", pressure);
		ImGui::End();


		ImGui::Begin("Temperature");
		ImGui::Text("Temperature: = %f degrees F", temp);
		ImGui::End();
	} else
	{
		frame = false;
	}

	if (sf::Joystick::isConnected(0) && sf::Joystick::hasAxis(0, sf::Joystick::Y) && updateFrequency == updateCounter)
	{
		auto pos = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
		let up = 10.f;
		let down = 3.4f;
		let range = up - down;
		var amount = (pos + 100.f) / 200.f * range + down;
		sf::Packet p;
		p << static_cast<unsigned char>(Core::PacketTypes::Move);
		p << amount;
		GlobalContext::get_network()->send_packet(p, Core::PacketTypes::Move);
		updateCounter = 0;
	} else
	{
		++updateCounter;
	}
}

Frames::FrameType Frames::ViewFrame::get_type() const
{
	return View;
}

Frames::ViewFrame::~ViewFrame()
{
	sf::Packet vidP;
	sf::Packet pressP;
	sf::Packet tempP;
	vidP << static_cast<unsigned char>(Core::PacketTypes::StopVideo);
	pressP << static_cast<unsigned char>(Core::PacketTypes::StopPressure);
	tempP << static_cast<unsigned char>(Core::PacketTypes::StopTemp);
	GlobalContext::get_network()->send_packet(vidP, Core::PacketTypes::StopVideo);
	GlobalContext::get_network()->send_packet(tempP, Core::PacketTypes::StopTemp);
	GlobalContext::get_network()->send_packet(pressP, Core::PacketTypes::StopPressure);
	GlobalContext::get_core_event_handler()->unhook_event_callback_for_all_events(frameHook);
	GlobalContext::get_core_event_handler()->unhook_event_callback_for_all_events(pressureHook);
	GlobalContext::get_core_event_handler()->unhook_event_callback_for_all_events(temperatureHook);
}
