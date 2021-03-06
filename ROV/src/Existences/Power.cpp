#include "Power.h"
#include "../Core/GlobalContext.h"
#include "../Factories/PacketFactory.h"

Power::Power::Power() : monitorThread(&Power::monitor, this), done(false)  {
	initialized = battery.setup();

	lightHook = GlobalContext::get_core_event_handler()->add_event_callback([&](const Core::Event * e) ->bool {
		lightsOn = std::get<Core::Event::LightChangeDetails>(e->data).on;
		return false;
	}, Core::Event::LightChange);

	if (!initialized) {
		GlobalContext::get_engine()->log("Battery not initialized! Cannot monitor battery! Be careful!");
	}
}

void Power::Power::monitor() {
	while (!done) {
		if (!initialized) {
			std::this_thread::sleep_for(std::chrono::seconds(5));
			GlobalContext::get_engine()->log("Battery not initialized, cannot monitor battery attempting to communicate, be careful!");
			initialized = battery.setup();
			continue;
		}

		// Convert to a current via datasheet
		// V * A/V = A
		// It likes to read zero up to lik .5-1 A and it's always at least 1 A lagging on current I guess?
		// 1.06 but .5 for now
		float current;
		if (lightsOn) {
			current = battery.queryDevice() * conversionFactor + 1.f;
		} else {
			current = battery.queryDevice() * conversionFactor + 0.5f;
		}

		// Integrate from one point to the next.
		asUsed += (current + last) * pollTimer.restart().asSeconds() / 2.f;

		last = current;
		if (sendTimer.getElapsedTime().asSeconds() > 10.f) {
			sendTimer.restart();
			auto percent = (asTotal - asUsed) / asTotal * 100;
			GlobalContext::get_network()->sendPacket(Factory::PacketFactory::create_battery_percentage_packet(percent));


			if (percent < batteryCuttof) {
				GlobalContext::get_engine()->log("Low battery, shutting down. Please pull me up as there are things still taking power, and if I am drained too far, I might catch fire.");
				std::this_thread::sleep_for(std::chrono::milliseconds(batteryPollTime));
				GlobalContext::get_engine()->add_event(std::make_unique<Core::Event>(Core::Event::Shutdown));
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(batteryPollTime));
	}
}

Power::Power::~Power() {
	done = true;
	if (monitorThread.joinable()) monitorThread.join();
	GlobalContext::get_core_event_handler()->unhook_event_callback_for_all_events(lightHook);
}
