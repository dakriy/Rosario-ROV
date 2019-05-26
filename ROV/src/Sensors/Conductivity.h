#pragma once

#include "Sensor.h"
#include "lib/ECEZ0.h"

namespace Sensor {
	class Conductivity : public Sensor{
	protected:
		const SensorInfo info{
				.id = static_cast<sf::Uint8>(SensorId::Conductivity),
				.maxFrequency = 1.f / 0.64f,
				.name = "Conductivity",
				.units = "ppm"
		};
		ECEZ0 conductivity;
	public:
		const SensorInfo& getSensorInfo() override;
		bool setup() override;
		void initiateConversion() override;
		float queryDevice() override;
	};
}
