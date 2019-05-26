#include "ViewFrame.h"
#include "../Core/GlobalContext.h"
#include "../Factories/PacketFactory.h"
#include <iostream>
#include <imgui.h>
#include "../Utilities/Utilities.h"

Frames::ViewFrame::ViewFrame() : offsets(0.f, 0.f), pos(0.f, 0.f)
{
	frameHook = GlobalContext::get_core_event_handler()->add_event_callback([this](const Core::Event *e)->bool {
		auto& jpg = std::get<std::vector<uint8_t>>(e->data);
		if (!image.loadFromMemory(jpg.data(), jpg.size())) {
			GlobalContext::get_log()->AddLog(
					"[%.1f] [%s] Corrupted JPG was sent from ROV\n",
					GlobalContext::get_clock()->getElapsedTime().asSeconds(), "log");
			return false;
		}

		if (!tex.loadFromImage(image))
		{
			GlobalContext::get_log()->AddLog(
					"[%.1f] [%s] Could not load the jpg into a texture.\n",
					GlobalContext::get_clock()->getElapsedTime().asSeconds(), "log");
			return false;
		}

		sprite.setTexture(tex);

		// Set scale only once
		if (!frame) {
			sprite.scale(window_->getSize().x / (sprite.getLocalBounds().width), window_->getSize().y / sprite.getLocalBounds().height);
		}

		frame = true;
		return false;
	}, Core::Event::EventType::VideoFrameReceived);
//	pressureHook = GlobalContext::get_core_event_handler()->add_event_callback([this](const Core::Event *e)->bool {
//	    if (e->sInfo.hasDataForSensor(Core::SensorInfo::Pressure)) {
//	        pressure = e->sInfo.getSensorData(Core::SensorInfo::Pressure)->p.pressure;
//	    }
//		return false;
//	}, Core::Event::EventType::SensorInfoReceived);
//
//	temperatureHook = GlobalContext::get_core_event_handler()->add_event_callback([this](const Core::Event *e)->bool {
//	    if (e->sInfo.hasDataForSensor(Core::SensorInfo::Temperature)) {
//	        temp = e->sInfo.getSensorData(Core::SensorInfo::Temperature)->t.temperature;
//	    }
//		return false;
//	}, Core::Event::EventType::SensorInfoReceived);

	recordButton = GlobalContext::get_event_handler()->add_event_callback([&](const sf::Event * e)-> bool {
		if (e->joystickButton.joystickId == 0 && e->joystickButton.button == 1) {
			record = !record;
			GlobalContext::get_network()->send_packet(Factory::PacketFactory::create_video_record_packet(record));
		}
		return true;
	}, sf::Event::EventType::JoystickButtonPressed);

	if (sf::Joystick::isConnected(0) &&
		sf::Joystick::hasAxis(0, sf::Joystick::Z) &&
		sf::Joystick::hasAxis(0, sf::Joystick::R))
	{
		offsets.x = sf::Joystick::getAxisPosition(0, sf::Joystick::Z);
		offsets.y = sf::Joystick::getAxisPosition(0, sf::Joystick::R);
	}

	GlobalContext::get_network()->send_packet(Factory::PacketFactory::create_start_video_stream_packet());
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

	if (sf::Joystick::isConnected(0)) {
		// Update lights
		// Triangle does it which is button 0
		if (sf::Joystick::getButtonCount(0) > 0) {
			bool button = sf::Joystick::isButtonPressed(0, 0);
			if (!lightChange && button) {
				lightChange = true;
				lightsOn = !lightsOn;
				GlobalContext::get_log()->AddLog("Lights are %s\n", lightsOn ? "On" : "Off");
				GlobalContext::get_network()->send_packet(Factory::PacketFactory::create_light_packet(lightsOn, lightPercent));
			} else if (!button) {
				lightChange = false;
			}
		}

		if (sf::Joystick::getButtonCount(0) > 4 && lightsOn) {
			bool update = false;
			if (sf::Joystick::isButtonPressed(0, 5)) {
				update = true;
				lightPercent += lightSpeed * dt.asSeconds();
				if (lightPercent > 100.f) {
					lightPercent = 100.f;
				}
			} else if (sf::Joystick::isButtonPressed(0, 4)) {
				update = true;
				lightPercent -= lightSpeed * dt.asSeconds();
				if (lightPercent < 0.f) {
					lightPercent = 0.f;
				}
			}

			if (update) {
				GlobalContext::get_network()->send_packet(Factory::PacketFactory::create_light_packet(lightsOn, lightPercent));
			}
		}

		// Update servos
		if (sf::Joystick::hasAxis(0, sf::Joystick::Z) &&
			sf::Joystick::hasAxis(0, sf::Joystick::R) &&
			updateFrequency == updateCounter)
		{
			// z, r
			sf::Vector2f posChange(sf::Joystick::getAxisPosition(0, sf::Joystick::Z) - offsets.x,
								   -(sf::Joystick::getAxisPosition(0, sf::Joystick::R) - offsets.y));

			pos += posChange * maxVelocity * dt.asSeconds();

			if (magnitude(pos) > virtualCircleRadius) {
				// Equations to rescale the vector
				if (pos.y == 0) {
					pos.x = sgn(pos.x) * 100;
				} else {
					auto ratio = pos.x / pos.y;
					pos.x = sgn(pos.x) * 100.f * ratio / (sqrt(ratio * ratio + 1));
					pos.y = sgn(pos.y) * 100.f / (sqrt(ratio * ratio + 1));
				}
			}

			if (sf::Joystick::getButtonCount(0) > 11) {
				if (sf::Joystick::isButtonPressed(0, 11)) {
					pos.x = 0.f;
					pos.y = 0.f;
				}
			}

			auto theta = 0.f;
			if (pos.x != 0 || pos.y != 0) {
				theta = atan2(pos.x, pos.y) * 180.f / PI;
			}

//		GlobalContext::get_log()->AddLog("x %f, y %f\n", pos.x, pos.y);
//			GlobalContext::get_log()->AddLog("theta %f, r %f\n", theta, magnitude(pos));
			GlobalContext::get_network()->send_packet(Factory::PacketFactory::create_camera_move_packet(theta, magnitude(pos)));
			updateCounter = 0;
		} else
		{
			++updateCounter;
		}
	}
}

Frames::FrameType Frames::ViewFrame::get_type() const
{
	return View;
}

Frames::ViewFrame::~ViewFrame()
{
	GlobalContext::get_network()->send_packet(Factory::PacketFactory::create_stop_video_stream_packet());
	GlobalContext::get_core_event_handler()->unhook_event_callback_for_all_events(frameHook);
	GlobalContext::get_core_event_handler()->unhook_event_callback_for_all_events(pressureHook);
	GlobalContext::get_core_event_handler()->unhook_event_callback_for_all_events(temperatureHook);
}
