#include "Light.h"
#include "../Core/GlobalContext.h"
#include <wiringPi.h>

void External::Light::control() {
	if (on) {
		float p = percent.load() / 100.f;

		float cyclePercent = DIM + p * RANGE;

		digitalWrite(PIN, HIGH);
		std::this_thread::sleep_for(std::chrono::microseconds(static_cast<int>(cyclePercent / 100.f * T_MICRO)));
		digitalWrite(PIN, LOW);
		std::this_thread::sleep_for(std::chrono::microseconds(static_cast<int>(T_MICRO - cyclePercent / 100.f * T_MICRO)));
	} else {
		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
	}
}

bool External::Light::state() {
	return on.load();
}

void External::Light::setPercent(float p) {
	if (p > 100.f) p = 100.f;
	if (p < 0.f) p = 0.f;

	percent.store(p, std::memory_order_release);
}

External::Light::Light() : on(false), percent(0.f) {
	digitalWrite(PIN, LOW);
	pinMode(PIN, OUTPUT);
	updateHook = GlobalContext::get_core_event_handler()->add_event_callback([&](const Core::Event * e) ->bool {
		setState(std::get<Core::Event::LightChangeDetails>(e->data).on);
		setPercent(std::get<Core::Event::LightChangeDetails>(e->data).percent);
		return false;
	}, Core::Event::LightChange);
}

void External::Light::setState(bool s) {
	on = s;
}
