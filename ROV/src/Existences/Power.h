#pragma once

#include <SFML/System/Clock.hpp>
#include "../Utilities/Existence.h"
#include <thread>
#include <atomic>
#include "../Sensors/Battery.h"
#include "../Core/EventHandler.h"
#include "../Core/Event.h"

namespace Power {
	class Power : public Existence {
	protected:
		// 5000 mAh -> 5 Ah -> 5 Ah * 3600 s/h -> As
		const float asTotal = 5.f * 3600;
		// In milliseconds
		const int batteryPollTime = 50;

		const float batteryCuttof = 10.f;

		// A/V
		const float conversionFactor = 55.7575757576;

		std::thread monitorThread;

		sf::Clock sendTimer;
		sf::Clock pollTimer;

		Sensor::Battery battery;

		std::atomic_bool done;

		bool initialized = false;

		float last = 0.f;

		float asUsed = 0.f;

		bool lightsOn = false;

		EVENT_FUNC_INDEX_CORE lightHook = nullptr;

		void monitor();
	public:
		Power();
		~Power() override;
	};
}
