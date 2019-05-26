#pragma once

#include "Servo.h"
#include <atomic>

namespace Camera {
	class ServoTray : public Servo {
	protected:
		// This is 6 just in case it gets decremented right after a load, and we only get 4 pulses off as a result
		const char MIN_PULSES = 6;


		const float MAX = 3.4f;
		const float MIN = 10.92f;
		const float RANGE = MIN - MAX;

		// Refresh rate, in ms
		const int SLEEP_TIME = 10;

		// WIRING PI PIN can use gpio readall
		const int PIN = 30;
		std::atomic<float> percent;
		std::atomic_int_fast8_t pulses;
	public:
		ServoTray();

		void control() override;

		void setAngle(float a);
	};
}
