#pragma once

#include "Sensor.h"

namespace Sensor {
	class InternalPressure : public Sensor {
	protected:
		const SensorInfo info{
				.id = static_cast<sf::Uint8>(SensorId::InternalPressure),
				.maxFrequency = 25.f,
				.name = "Internal Pressure",
				.units = "mbar"
		};
	public:
		const SensorInfo& getSensorInfo() override;
		bool setup() override;
		void initiateConversion() override;
		float queryDevice() override;
	};
}
