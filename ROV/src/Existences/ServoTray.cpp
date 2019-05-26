#include "ServoTray.h"
#include "../Core/GlobalContext.h"
#include <wiringPi.h>

Camera::ServoTray::ServoTray() {
	digitalWrite(PIN, LOW);
	pinMode(PIN, OUTPUT);

	updateHook = GlobalContext::get_core_event_handler()->add_event_callback([&](const Core::Event * e) ->bool {
		// radius is the radius
		auto theta = std::get<Core::Event::CameraMovement>(e->data).theta;
		while (theta < 0.f) {
			theta += 360.f;
		}

		if (theta > 350.f) {
			theta = 0.f;
		}

		if (theta > 185.f) {
			theta -= 180.f;
		}

		setAngle(theta);
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
void Camera::ServoTray::control() {
	if (pulses > 0) {
		--pulses;
		float p = percent.load() / 100.f;

		// Rotate based on the angle
		float cyclePercent = MIN - RANGE * p;

		digitalWrite(PIN, HIGH);
		std::this_thread::sleep_for(std::chrono::microseconds(static_cast<int>(cyclePercent / 100.f * T_MICRO)));
		digitalWrite(PIN, LOW);
		std::this_thread::sleep_for(std::chrono::microseconds(static_cast<int>(T_MICRO - cyclePercent / 100.f * T_MICRO)));
	} else {
		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
	}
}

void Camera::ServoTray::setAngle(float a) {
	if (a > 180.f) a = 180.f;
	if (a < 0.f) a = 0.f;

	// Rescale to 0 - 100 as a percent
	a = a * 5.f / 9.f;

	float old = percent.exchange(a, std::memory_order_release);
	if (old != a) {
		pulses.store(MIN_PULSES, std::memory_order_release);
	}

}
