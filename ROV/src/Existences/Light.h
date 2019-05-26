#pragma once

#include "Servo.h"

namespace External {
	class Light : public Camera::Servo {
		const float DIM = 5.5f;
		const float BRIGHT = 10.f;
		const float RANGE = BRIGHT - DIM;
		const int PIN = 2;

		// Refresh rate, in ms
		const int SLEEP_TIME = 10;

		std::atomic_bool on;
		std::atomic<float> percent;
	public:
		Light();
		void control() override;
		bool state();
		void setPercent(float p);
		void setState(bool s);
	};
}
