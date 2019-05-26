#pragma once

#include <thread>
#include <atomic>
#include "../Core/EventHandler.h"
#include "../Core/Event.h"
#include "../Utilities/Existence.h"

namespace Camera {
	class Servo : public Existence {
	private:
		void thread();
	protected:
		// Standard servo pwm frequency
		const float FREQ = 50;
		const float T = 1.f / FREQ;
		const float T_MICRO = T * 1000000;
		std::thread controller;

		EVENT_FUNC_INDEX_CORE updateHook = nullptr;

		std::atomic_bool done;
	public:
		Servo();
		virtual void control() = 0;
		virtual ~Servo();
	};
}
