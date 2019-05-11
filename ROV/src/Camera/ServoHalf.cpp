#include "ServoHalf.h"
#include "../Core/GlobalContext.h"

Camera::ServoHalf::ServoHalf(): pulses(MIN_PULSES), percent(0), done(false), controller(&ServoHalf::control, this) {
	digitalWrite(PIN, LOW);
	pinMode(PIN, OUTPUT);

	cameraMoveHook = GlobalContext::get_core_event_handler()->add_event_callback([&](const Core::Event * e) ->bool {
		setPercent(e->c.s1);
		return false;
	}, Core::Event::CameraMove);
}

void Camera::ServoHalf::control() {
	while (!done) {
		if (pulses > 0) {
			--pulses;
			float p = percent.load() / 100.f;


			float cyclePercent;
			if (p > 0) {
				// Move left

				// For left + percent but - off mid
				cyclePercent = MID - LEFT_HALF_RANGE * p;
			} else {
				// Move right

				// For right - percent but + off mid
				cyclePercent = MID - RIGHT_HALF_RANGE * p;
			}

			digitalWrite(PIN, HIGH);
			std::this_thread::sleep_for(std::chrono::microseconds(static_cast<int>(cyclePercent / 100.f * T_MICRO)));
			digitalWrite(PIN, LOW);
			std::this_thread::sleep_for(std::chrono::microseconds(static_cast<int>(T_MICRO - cyclePercent / 100.f * T_MICRO)));
		} else {
			std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
		}
	}
}

Camera::ServoHalf::~ServoHalf() {
	done = true;
	if (controller.joinable()) controller.join();
	GlobalContext::get_core_event_handler()->unhook_event_callback_for_all_events(cameraMoveHook);
}

void Camera::ServoHalf::setPercent(float p) {
	// Cap it to +/- 100%
	if (p > 100.f) p = 100.f;
	if (p < -100.f) p = -100.f;

	float old = percent.exchange(p, std::memory_order_release);
	if (old != p) {
		pulses.store(MIN_PULSES, std::memory_order_release);
	}
}
