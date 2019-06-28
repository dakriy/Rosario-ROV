#include "Engine.h"
// This needs to be in here, because if it is placed in a header file you get an include loop
#include "GlobalContext.h"
#include <cassert>
#include "../Factories/PacketFactory.h"
#include "../Utilities/Utilities.h"
#include <filesystem>

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

std::vector<std::pair<sf::Uint8, float>> Core::Engine::querySensors(bool allSensors) {
	if (readyForConversion) {
		// Don't want to immediately try to do another conversion on the sensors
		readyForConversion = false;
		if (allSensors) {
			for (const auto & sensor : sensors) {
				sensor->initiateConversion();
			}
		} else {
			for (auto sensor : requestedSensors) {
				sensors[sensor]->initiateConversion();
			}
		}
	}

	auto sleepTime = static_cast<int>(1.f / sensorFrequency * 1000 - dataTimer.getElapsedTime().asMilliseconds());

	if (sleepTime > 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
	}

	dataTimer.restart();

	std::vector<std::pair<sf::Uint8, float>> data;

	if (allSensors) {
		for (auto & sensor : sensors) {
			data.emplace_back(std::make_pair(sensor->getSensorInfo().id, sensor->queryDevice()));
		}
	} else {
		for (auto sensor : requestedSensors) {
			data.emplace_back(std::make_pair(sensors[sensor]->getSensorInfo().id, sensors[sensor]->queryDevice()));
		}
	}
	// We are ready for another conversion
	readyForConversion = true;
	return data;
}

void Core::Engine::Update()
{
	// This is the state machine logic to actually figure out outputs.
	switch (rovState) {
		case ROVState::Idle:
			std::this_thread::sleep_for(std::chrono::milliseconds(defaultTimeout));
			break;
		case ROVState::Connected: {
			if (sensorsInitialized) {
				auto data = querySensors(true);

				GlobalContext::get_network()->sendPacket(Factory::PacketFactory::create_data_packet(data));
			} else {
				std::this_thread::sleep_for(std::chrono::milliseconds(defaultTimeout));
			}
			break;
		}
		case ROVState::ConnectedPaused:
			std::this_thread::sleep_for(std::chrono::milliseconds(defaultTimeout));
			break;
		case ROVState::MissionConnected: {
			if (sensorsInitialized) {
				auto data = querySensors();

				GlobalContext::get_network()->sendPacket(Factory::PacketFactory::create_data_packet(data));

				if (!localFile.empty()) {
					recordDataSet(data);
				}
			} else {
				std::this_thread::sleep_for(std::chrono::milliseconds(defaultTimeout));
			}

			break;
		}
		case ROVState::MissionDisconnected: {
			if (sensorsInitialized) {
				auto data = querySensors();

				if (!localFile.empty()) {
					recordDataSet(data);
				}
			} else {
				std::this_thread::sleep_for(std::chrono::milliseconds(defaultTimeout));
			}
			break;
		}
		case ROVState::COUNT:
			// Shouldn't be here, go to idle state to get out of this state.
			rovState = ROVState::Idle;
			break;
	}
}

Core::Engine::Engine(EventHandler<Core::Event, Core::Event::EventType::Count>* cev, const std::string& missionDirectory) : missionDir(missionDirectory)
{
	assert(cev);
	cev_ = cev;
	GlobalContext::set_engine(this);
	setHooks();
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

void Core::Engine::Drive()
{
	Events();

	Update();
}

Core::Engine::~Engine()
{
	for (auto hook : hooks) {
		cev_->unhook_event_callback_for_all_events(hook);
	}
	GlobalContext::clear_engine();
}

void Core::Engine::addExistence(std::unique_ptr <Existence> ex) {
	existences.push_back(std::move(ex));
}

void Core::Engine::log(const std::string &message) {
	GlobalContext::get_network()->sendPacket(Factory::PacketFactory::create_log_packet(message));
}

void Core::Engine::setHooks() {
	// These hooks modify the state, as such all states need to be covered.
	// These lambdas basically contain the next state logic for the state machine, and get processed on the events
	hooks.push_back(cev_->add_event_callback([&](const Event * e) -> bool {
		switch (rovState) {
			case ROVState::Idle:
				rovState = ROVState::Connected;
				break;
			case ROVState::ConnectedPaused:
				rovState = ROVState::Connected;
				break;
			case ROVState::MissionDisconnected:
				rovState = ROVState::MissionConnected;
				break;
			default:
				break;
		}

		GlobalContext::get_network()->sendPacket(
				Factory::PacketFactory::create_rov_state_packet(
						static_cast<unsigned>(rovState)));

		return false;
	}, Core::Event::NewConnection));

	hooks.push_back(cev_->add_event_callback([&](const Event * e) -> bool {
		switch (rovState) {
			case ROVState::Connected:
				rovState = ROVState::Idle;
				break;
			case ROVState::ConnectedPaused:
				rovState = ROVState::Idle;
				break;
			case ROVState::MissionConnected:
				rovState = ROVState::MissionDisconnected;
				break;
			default:
				break;
		}

		return false;
	}, Core::Event::Disconnected));

	hooks.push_back(cev_->add_event_callback([&](const Event * e) -> bool {
		auto dat = std::get<Core::Event::SensorsRequested>(e->data);
		sensorFrequency = dat.frequency;
		if (!dat.fileToRecord.empty()) {
			localFile = missionDir + '/' + dat.fileToRecord;
		} else {
			localFile = "";
		}

		// Clear sensors in case somehow we got here without them being cleared. This avoids sensor duplicates
		requestedSensors.clear();

		// Get the requested sensors.
		for(auto requestedSensorId : dat.sensors) {
			for (auto [sensorIndex, actualSensor] : enumerate(sensors)) {
				if (actualSensor->getSensorInfo().id == requestedSensorId) {
					requestedSensors.push_back(sensorIndex);
					// No need to look through the rest
					break;
				}
			}
		}

		switch (rovState) {
			case ROVState::Connected:
				rovState = ROVState::MissionConnected;
				break;
			case ROVState::ConnectedPaused:
				rovState = ROVState::MissionConnected;
				break;
			case ROVState::MissionConnected:
				rovState = ROVState::MissionConnected;
				break;
			default:
				break;
		}

		std::filesystem::create_directory(missionDir);

		if (!localFile.empty()) {
			initializeRecordFile(localFile);
		}

		startTime = GlobalContext::get_clock()->getElapsedTime();

		// Don't think anything else needs to process this type of packet so go ahead and report it handled
		return true;
	}, Core::Event::MissionStart));

	hooks.push_back(cev_->add_event_callback([&](const Event * e) -> bool {
		// Query one last time for devices that might be expecting it.
		for (auto sensor : requestedSensors) {
			sensors[sensor]->queryDevice();
		}

		localFile.clear();

		// Clear the requested sensors
		requestedSensors.clear();

		switch (rovState) {
			case ROVState::MissionConnected:
				rovState = ROVState::Connected;
				break;
			case ROVState::MissionDisconnected:
				rovState = ROVState::Idle;
				break;
			default:
				break;
		}
		return true;
	}, Core::Event::MissionStop));

	hooks.push_back(cev_->add_event_callback([&](const Event * e) -> bool {
		auto state = std::get<bool>(e->data);
		switch (rovState) {
			case ROVState::Connected:
				if (!state) {
					rovState = ROVState::ConnectedPaused;
				}
				break;
			case ROVState::ConnectedPaused:
				if (state) {
					rovState = ROVState::Connected;
				}
				break;
			default:
				break;
		}
		return true;
	}, Core::Event::DataSendState));

	hooks.push_back(cev_->add_event_callback([&](const Event * e) -> bool {
		std::vector<Sensor::SensorInfo> sensorInformation;
		sensorInformation.reserve(sensors.size());
		for (auto & sensor : sensors) {
			// Make sure sensor can be contacted, Only send functioning sensors.
			if (sensor->setup()) {
				sensorInformation.push_back(sensor->getSensorInfo());
			}
		}
		GlobalContext::get_network()->sendPacket(Factory::PacketFactory::create_sensor_list_packet(sensorInformation));
		sensorsInitialized = true;
		return true;
	}, Core::Event::SensorRequest));

}

void Core::Engine::recordDataSet(const std::vector<std::pair<sf::Uint8, float>> & data) {
	// Record data to csv
	std::vector<float> allVals;
	allVals.reserve(sensors.size() + 1);
	// Add time to the columns
	allVals.emplace_back((GlobalContext::get_clock()->getElapsedTime() - startTime).asSeconds());
	// Add the rest
	for (const auto & d : data) {
		allVals.emplace_back(std::get<float>(d));
	}
	std::vector<std::string> stringVals;
	stringVals.reserve(allVals.size());
	for (auto val : allVals) {
		stringVals.emplace_back(std::to_string(val));
	}
	auto csv = csv::Writer(localFile);
	csv.write_row(stringVals);
	csv.close();
}

void Core::Engine::initializeRecordFile(const std::string &fileName) {
	auto csv = csv::Writer(fileName);

	// Configure dialect
	csv.configure_dialect().column_names("Time (s)");
	for (auto s : requestedSensors) {
		// Setup column headers
		auto str = sensors[s]->getSensorInfo().name + ' ' + '(' + sensors[s]->getSensorInfo().units + ')';
		csv.configure_dialect().column_names(str);
	}
	// write the header
	csv.write_header();

	// Release file
	csv.close();
}
