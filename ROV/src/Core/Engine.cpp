#include "Engine.h"
// This needs to be in here, because if it is placed in a header file you get an include loop
#include "GlobalContext.h"
#include <cassert>
#include "../Factories/PacketFactory.h"
#include "../Utilities/Utilities.h"

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
	if (missionInProgress) {
		for (auto & sensor : sensors) {
			sensor->initiateConversion();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1.f / sensorFrequency * 1000 - dataTimer.getElapsedTime().asMilliseconds())));
		dataTimer.restart();
		std::vector<float> data;
		for (auto sensor : requestedSensors) {
			data.push_back(sensors[sensor]->queryDevice());
		}
		GlobalContext::get_network()->sendPacket(Factory::PacketFactory::create_data_packet(data));
	} else {
		std::this_thread::sleep_for(std::chrono::milliseconds(defaultTimeout));
	}
}

Core::Engine::Engine(EventHandler<Core::Event, Core::Event::EventType::Count>* cev)
{
	assert(cev);
	cev_ = cev;

	GlobalContext::set_engine(this);

	watchForRequest = cev_->add_event_callback([&](const Event * e) -> bool {
		sensorFrequency = e->r.frequency;
		missionInProgress = true;
		for(auto requestedSensorId : e->r.sensors) {
			for (auto [sensorIndex, actualSensor] : enumerate(sensors)) {
				if (actualSensor->getSensorInfo().id == requestedSensorId) {
					requestedSensors.push_back(sensorIndex);
					// No need to look through the rest
					break;
				}
			}
		}

		// Don't think anything else needs to process this type of packet so go ahead and report it handled
		return true;
	}, Core::Event::MissionStart);

	watchForRequestStop = cev_->add_event_callback([&](const Event * e) -> bool {
		missionInProgress = false;
		requestedSensors.clear();
		return true;
	}, Core::Event::MissionStop);

	sensorRequest = cev_->add_event_callback([&](const Event * e) -> bool {
		std::vector<Sensor::SensorInfo> sensorInformation;
		sensorInformation.reserve(sensors.size());
		for (auto & sensor : sensors) {
			// Make sure sensor can be contacted, Only send functioning sensors.
			if (sensor->setup()) {
				sensorInformation.push_back(sensor->getSensorInfo());
			}
		}
		GlobalContext::get_network()->sendPacket(Factory::PacketFactory::create_sensor_list_packet(sensorInformation));
		return true;
	}, Core::Event::SensorRequest);

	camStarStopRequest = cev_->add_event_callback([&](const Event * e) -> bool {
		if (e->type == Core::Event::StartCamera) {
			camera.startVideoStream();
			return true;
		} else if (e->type == Core::Event::StopCamera) {
			camera.endVideoStream();
			return true;
		}
		return false;
	}, Core::Event::StartCamera, Core::Event::StopCamera);
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
	camera.endVideoStream();
	camera.endVideoRecord();
	cev_->unhook_event_callback_for_all_events(sensorRequest);
	cev_->unhook_event_callback_for_all_events(watchForRequest);
	cev_->unhook_event_callback_for_all_events(watchForRequestStop);
	cev_->unhook_event_callback_for_all_events(camStarStopRequest);
	missionInProgress = false;
	GlobalContext::clear_engine();
}