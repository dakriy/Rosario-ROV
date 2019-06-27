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

std::vector<float> Core::Engine::querySensors() {
	if (readyForConversion) {
		// Don't want to immediately try to do another conversion on the sensors
		readyForConversion = false;
		for (auto sensor : requestedSensors) {
			sensors[sensor]->initiateConversion();
		}
	}

	auto sleepTime = static_cast<int>(1.f / sensorFrequency * 1000 - dataTimer.getElapsedTime().asMilliseconds());

	if (sleepTime > 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
	}

	dataTimer.restart();

	std::vector<float> data;

	for (auto sensor : requestedSensors) {
		data.push_back(sensors[sensor]->queryDevice());
	}
	// We are ready for another conversion
	readyForConversion = true;
	return data;
}

void Core::Engine::Update()
{
	// This is the state machine logic to actually figure out outputs.
	switch (dataState) {

		case ROVDataState::Idle:
			std::this_thread::sleep_for(std::chrono::milliseconds(defaultTimeout));
			break;
		case ROVDataState::Connected: {
			auto data = querySensors();

			GlobalContext::get_network()->sendPacket(Factory::PacketFactory::create_data_packet(data));

			break;
		}
		case ROVDataState::ConnectedPaused:
			std::this_thread::sleep_for(std::chrono::milliseconds(defaultTimeout));
			break;
		case ROVDataState::MissionConnected: {
			auto data = querySensors();

			GlobalContext::get_network()->sendPacket(Factory::PacketFactory::create_data_packet(data));

			if (!localFile.empty()) {
				recordDataSet(data);
			}

			break;
		}
		case ROVDataState::MissionDisconnected: {
			auto data = querySensors();

			if (!localFile.empty()) {
				recordDataSet(data);
			}

			break;
		}
		case ROVDataState::COUNT:
			// Shouldn't be here, go to idle state to get out of this state.
			dataState = ROVDataState::Idle;
			break;
	}
}

Core::Engine::Engine(EventHandler<Core::Event, Core::Event::EventType::Count>* cev)
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
	cev_->unhook_event_callback_for_all_events(disconnectHook);
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
	connectHook = cev_->add_event_callback([&](const Event * e) -> bool {
		switch (dataState) {
			case ROVDataState::Idle:
				dataState = ROVDataState::Connected;
				break;
			case ROVDataState::ConnectedPaused:
				dataState = ROVDataState::Connected;
				break;
			case ROVDataState::MissionDisconnected:
				dataState = ROVDataState::MissionConnected;
				break;
			default:
				break;
		}

		return false;
	}, Core::Event::NewConnection);

	disconnectHook = cev_->add_event_callback([&](const Event * e) -> bool {
		switch (dataState) {
			case ROVDataState::Connected:
				dataState = ROVDataState::Idle;
				break;
			case ROVDataState::ConnectedPaused:
				dataState = ROVDataState::Idle;
				break;
			case ROVDataState::MissionConnected:
				dataState = ROVDataState::MissionDisconnected;
				break;
			default:
				break;
		}

		return false;
	}, Core::Event::Disconnected);

	watchForRequest = cev_->add_event_callback([&](const Event * e) -> bool {
		auto dat = std::get<Core::Event::SensorsRequested>(e->data);
		sensorFrequency = dat.frequency;
		localFile = dat.fileToRecord;

		if (!localFile.empty()) {
			initializeRecordFile(localFile);
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

		switch (dataState) {
			case ROVDataState::Connected:
				dataState = ROVDataState::MissionConnected;
				break;
			case ROVDataState::ConnectedPaused:
				dataState = ROVDataState::MissionConnected;
				break;
			case ROVDataState::MissionConnected:
				dataState = ROVDataState::MissionConnected:
				break;
			default:
				break;
		}

		// Don't think anything else needs to process this type of packet so go ahead and report it handled
		return true;
	}, Core::Event::MissionStart);

	watchForRequestStop = cev_->add_event_callback([&](const Event * e) -> bool {
		// Query one last time for devices that might be expecting it.
		for (auto sensor : requestedSensors) {
			sensors[sensor]->queryDevice();
		}

		localFile.clear();

		// Clear the requested sensors
		requestedSensors.clear();

		switch (dataState) {
			case ROVDataState::MissionConnected:
				dataState = ROVDataState::Connected;
				break;
			case ROVDataState::MissionDisconnected:
				dataState = ROVDataState::Idle;
				break;
			default:
				break;
		}
		return true;
	}, Core::Event::MissionStop);

	watchForRequestStop = cev_->add_event_callback([&](const Event * e) -> bool {
		auto state = std::get<bool>(e->data);
		switch (dataState) {
			case ROVDataState::Connected:
				if (!state) {
					dataState = ROVDataState::ConnectedPaused;
				}
				break;
			case ROVDataState::ConnectedPaused:
				if (state) {
					dataState = ROVDataState::Connected;
				}
				break;
			default:
				break;
		}
		return true;
	}, Core::Event::DataSendState);

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

void Core::Engine::recordDataSet(const std::vector<float> & data) {
	// Record data to csv
	std::vector<float> allVals;
	allVals.reserve(sensors.size() + 1);
	// Add time to the columns
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
}

void Core::Engine::initializeRecordFile(const std::string &fileName) {
	localFile = fileName;
	auto csv = csv::Writer(localFile);

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
