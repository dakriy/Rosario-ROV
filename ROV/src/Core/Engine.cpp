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
		if (readyForConversion) {
			// Don't want to immediately try to do another conversion on the sensors
			readyForConversion = false;
			for (auto sensor : requestedSensors) {
				sensors[sensor]->initiateConversion();
			}
		}

		if (1.f / sensorFrequency * 1000 < dataTimer.getElapsedTime().asMilliseconds()) {
			dataTimer.restart();
			std::vector<float> data;
			for (auto sensor : requestedSensors) {
				data.push_back(sensors[sensor]->queryDevice());
			}
			if (localInstance) {
				// Record data to csv
				std::vector<float> allVals;
				allVals.reserve(sensors.size() + 1);
				// Add time
				allVals.emplace_back(GlobalContext::get_clock()->getElapsedTime().asSeconds());
				// Add the rest
				allVals.insert(std::end(allVals), std::begin(data), std::end(data));
				std::vector<std::string> stringVals;
				stringVals.reserve(allVals.size());
				for (auto val : allVals) {
					stringVals.emplace_back(std::to_string(val));
				}
				auto csv = csv::Writer(localFile);
				csv.write_row(stringVals);
				csv.close();
			} else {
				// Send sensor data
				GlobalContext::get_network()->sendPacket(Factory::PacketFactory::create_data_packet(data));
			}
			// We are ready for another conversion
			readyForConversion = true;
		} else {
			std::this_thread::sleep_for(std::chrono::milliseconds(defaultTimeout));
		}
	} else {
		std::this_thread::sleep_for(std::chrono::milliseconds(defaultTimeout));
	}
}

Core::Engine::Engine(EventHandler<Core::Event, Core::Event::EventType::Count>* cev, bool local, const std::string& file) : localInstance(local), localFile(file)
{
	assert(cev);
	cev_ = cev;

	GlobalContext::set_engine(this);

	if (localInstance) {
		for (auto [sensorIndex, actualSensor] : enumerate(sensors)) {
			// Make sure sensor can be contacted, Only record functioning sensors.
			if (actualSensor->setup()) {
				requestedSensors.push_back(sensorIndex);
			}
		}

		// Lock to 1hz
		sensorFrequency = 1.f;
		missionInProgress = true;

		auto csv = csv::Writer(localFile);

		// Configure dialect
		csv.configure_dialect().column_names("Time (s)");
		for (auto s : requestedSensors) {
			auto str = sensors[s]->getSensorInfo().name + ' ' + '(' + sensors[s]->getSensorInfo().units + ')';
			csv.configure_dialect().column_names(str);
		}
		csv.write_header();
		csv.close();

		connectHook = cev_->add_event_callback([&](const Event * e) -> bool {
			missionInProgress = false;
			for (auto sensor : requestedSensors) {
				sensors[sensor]->queryDevice();
			}
			requestedSensors.clear();
			localInstance = false;
			return false;
		}, Core::Event::NewConnection);
	} else {
		watchForRequest = cev_->add_event_callback([&](const Event * e) -> bool {
			sensorFrequency = std::get<Core::Event::SensorsRequested>(e->data).frequency;
			missionInProgress = true;
			for(auto requestedSensorId : std::get<Core::Event::SensorsRequested>(e->data).sensors) {
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
			for (auto sensor : requestedSensors) {
				sensors[sensor]->queryDevice();
			}
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
	}
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
	cev_->unhook_event_callback_for_all_events(sensorRequest);
	cev_->unhook_event_callback_for_all_events(watchForRequest);
	cev_->unhook_event_callback_for_all_events(watchForRequestStop);
	cev_->unhook_event_callback_for_all_events(connectHook);
	missionInProgress = false;
	GlobalContext::clear_engine();
}

void Core::Engine::addExistence(std::unique_ptr <Existence> ex) {
	existences.push_back(std::move(ex));
}

void Core::Engine::log(const std::string &message) {
	GlobalContext::get_network()->sendPacket(Factory::PacketFactory::create_log_packet(message));
}
