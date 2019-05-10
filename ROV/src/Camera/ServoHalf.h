#pragma once

#include <atomic>
#include <thread>
#include "../Core/EventHandler.h"
#include "../Core/Event.h"

namespace Camera {

	/**
	 * This is the controlling class for the 180 degree servo
	 *
	 * Documentation found from experimenting with the bad boy
	 *
	 * Operating frequency: 50 Hz
	 * Middle 		-> 6.3% duty cycle
	 * Left 		-> 3.4% duty cycle
	 * Right		->  10% duty cycle
	 *
	 * Ok so. Apparently, you only need to send it a minimum of 5 pulses of whatever duty cycle you want
	 * This is nice because you don't need to keep sending it pwm.
	 *
	 * I am not using the wiringPi pwm implementation for this for two reasons
	 * A. We do not need constant PWM, that is just a waste of limited resources on our Raspberry Pi
	 * B. Their way of doing PWM is dumb and doesn't give me the resolution I want.
	 *
	 *
	 * Ok some class documentation now, this is me thinking it out as I type, so bear with me.
	 * Use another thread but keep its work minimal and doing as little as possible
	 * Use lockless programming as we can use atomic variables for the number of pwm pulses to do and if there is a change
	 *
	 * atomic int to keep track of the pulses left, the thread only does anything if there are any pulses left
	 * We can make the worker thread do the calculations from %left/%right to actual pwm times as it's not doing much else
	 *
	 * I'm not going to use a condition variable as I want to be able to change it in the middle of
	 * another go.
	 *
	 * Ok I like this plan, lets do it gang
	 */
	class ServoHalf {
	protected:
		// Standard servo pwm frequency
		const float FREQ = 50;
		const float T = 1.f / FREQ;
		const float T_MICRO = T * 1000000;
		// This is 6 just in case it gets decremented right after a load, and we only get 4 pulses off as a result
		const char MIN_PULSES = 6;
		const float LEFT = 3.4f;
		const float RIGHT = 10.f;
		const float MID = 6.3f;
		// Make range positive to keep things simpler
		const float LEFT_HALF_RANGE = MID - LEFT;
		const float RIGHT_HALF_RANGE = RIGHT - MID;
		// Refresh rate, in ms
		const int SLEEP_TIME = 10;
		const int PIN = 0;


		// With floating point only the basic operations are supported
		std::atomic<float> percent;
		std::atomic_int_fast8_t pulses;
		std::atomic_bool done;

		std::thread controller;

		EVENT_FUNC_INDEX_CORE cameraMoveHook;
	public:
		ServoHalf();

		void control();

		void setPercent(float p);

		~ServoHalf();
	};
}
