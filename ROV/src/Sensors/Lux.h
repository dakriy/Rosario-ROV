#pragma once

#include "Sensor.h"
#include "lib/TSL2591.h"

namespace Sensor {
	class Lux : public Sensor {
	protected:
		const SensorInfo info{
			.id = static_cast<sf::Uint8 >(SensorId::Lux),
			.maxFrequency = 50.f,
			.name = "Light/Lux Sensor",
			.units = "Lux"
		};

		TSL2591 lightSensor;
	public:
		const SensorInfo& getSensorInfo() override;
		bool setup() override;
		void initiateConversion() override;
		float queryDevice() override;
	};
}

