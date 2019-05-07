#pragma once

#include "Sensor.h"

namespace Sensor {
	class InternalTemperature : public Sensor {
	protected:
		const SensorInfo info{
				.id = static_cast<sf::Uint8>(SensorId::InternalTemperature),
				.maxFrequency = 25.f,
				.name = "Internal Temperature",
				.units = "Celsius"
		};
	public:
		const SensorInfo& getSensorInfo() override;
		bool setup() override;
		void initiateConversion() override;
		float queryDevice() override;
	};
}
