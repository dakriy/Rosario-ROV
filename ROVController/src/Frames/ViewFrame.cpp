#include "ViewFrame.h"
#include "../Core/GlobalContext.h"
#include "../Factories/PacketFactory.h"
#include <iostream>
#include <imgui.h>

Frames::ViewFrame::ViewFrame()
{
	frameHook = GlobalContext::get_core_event_handler()->add_event_callback([this](const Core::Event *e)->bool {
		//image.create(e->f.w, e->f.h, e->f.data);
		if (e->sInfo.hasDataForSensor(Core::SensorInfo::Video)) {
		    auto newImg = e->sInfo.getSensorData(Core::SensorInfo::Video);
            if(!image.loadFromMemory(newImg->f.data, newImg->f.len))
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
		}
		return false;
	}, Core::Event::EventType::SensorInfoReceived);

	pressureHook = GlobalContext::get_core_event_handler()->add_event_callback([this](const Core::Event *e)->bool {
	    if (e->sInfo.hasDataForSensor(Core::SensorInfo::Pressure)) {
	        pressure = e->sInfo.getSensorData(Core::SensorInfo::Pressure)->p.pressure;
	    }
		return false;
	}, Core::Event::EventType::SensorInfoReceived);

	temperatureHook = GlobalContext::get_core_event_handler()->add_event_callback([this](const Core::Event *e)->bool {
	    if (e->sInfo.hasDataForSensor(Core::SensorInfo::Temperature)) {
	        temp = e->sInfo.getSensorData(Core::SensorInfo::Temperature)->t.temperature;
	    }
		return false;
	}, Core::Event::EventType::SensorInfoReceived);

	GlobalContext::get_network()->send_packet(Factory::PacketFactory::create_video_stream_start_packet());
	GlobalContext::get_network()->send_packet(Factory::PacketFactory::create_pressure_data_start_packet());
	GlobalContext::get_network()->send_packet(Factory::PacketFactory::create_temperature_data_start_packet());
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
		GlobalContext::get_network()->send_packet(Factory::PacketFactory::create_camera_move_packet(amount));
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
	GlobalContext::get_network()->send_packet(Factory::PacketFactory::create_video_stream_stop_packet());
	GlobalContext::get_network()->send_packet(Factory::PacketFactory::create_pressure_data_stop_packet());
	GlobalContext::get_network()->send_packet(Factory::PacketFactory::create_temperature_data_stop_packet());
	GlobalContext::get_core_event_handler()->unhook_event_callback_for_all_events(frameHook);
	GlobalContext::get_core_event_handler()->unhook_event_callback_for_all_events(pressureHook);
	GlobalContext::get_core_event_handler()->unhook_event_callback_for_all_events(temperatureHook);
}
