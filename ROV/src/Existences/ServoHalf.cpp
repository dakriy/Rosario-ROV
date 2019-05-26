#include "ServoHalf.h"
#include "../Core/GlobalContext.h"
#include <wiringPi.h>

Camera::ServoHalf::ServoHalf(): pulses(MIN_PULSES), percent(0) {
	digitalWrite(PIN, LOW);
	pinMode(PIN, OUTPUT);

	updateHook = GlobalContext::get_core_event_handler()->add_event_callback([&](const Core::Event * e) ->bool {
		// radius is the radius
		auto angle = std::get<Core::Event::CameraMovement>(e->data).theta;
		while (angle < 0) {
			angle += 360.f;
		}
		if (angle > 185 && angle < 350) {
			setPercent(-std::get<Core::Event::CameraMovement>(e->data).radius);
		} else {
			setPercent(std::get<Core::Event::CameraMovement>(e->data).radius);
		}
		return false;
	}, Core::Event::CameraMove);
}

/**
 * Up/left is the lower number on the servo
 *
 * Twist starts at lowest and highest brings it to horizontal
 *
 * First packet number is going to be theta measured from the positive counter clockwise y axis
 * Second packet number is the R valuse, from 100 to -100
 *
 * Assume Start oriented in this position
 * +-------------+
 * |     |       |
 * |     |       |
 * |     o       |
 * |     |       |
 * |     |       |
 * +-------------+
 *
 * Goes this direction
 *
 * +-------------+
 * |        /    |
 * |       /     |
 * |     o       |
 * |   /         |
 * | /           |
 * +-------------+
 *
 *
 * To:
 *
 * +-------------+
 * |             |
 * |             |
 * |-----o-------|
 * |             |
 * |             |
 * +-------------+
 *
 *
 */
void Camera::ServoHalf::control() {
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

void Camera::ServoHalf::setPercent(float p) {
	// Cap it to +/- 100%
	if (p > 100.f) p = 100.f;
	if (p < -100.f) p = -100.f;

	float old = percent.exchange(p, std::memory_order_release);
	if (old != p) {
		pulses.store(MIN_PULSES, std::memory_order_release);
	}
}
