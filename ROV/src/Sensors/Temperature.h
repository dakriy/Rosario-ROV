#pragma once

#include "Sensor.h"
#include "lib/TSYS01.h"

namespace Sensor {
	class Temperature : public Sensor {
	protected:
		const SensorInfo info{
				.id = static_cast<sf::Uint8>(SensorId::Temperature),
				.maxFrequency = 100.f,
				.name = "External Temperature",
				.units = "Celsius"
		};

		TSYS01 temp;
	public:
		const SensorInfo& getSensorInfo() override;
		bool setup() override;
		void initiateConversion() override;
		float queryDevice() override;
	};
}
