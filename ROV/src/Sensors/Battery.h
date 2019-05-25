#pragma once
#include "Sensor.h"
#include "lib/LTC2451.h"

namespace Sensor {
	class Battery : public Sensor {
	protected:
		const SensorInfo info{
				.id = static_cast<sf::Uint8>(SensorId::Battery),
				.maxFrequency = 30.f,
				.name = "Battery",
				.units = "%"
		};
		LTC2451 ADC;
	public:
		const SensorInfo& getSensorInfo() override;
		bool setup() override;
		void initiateConversion() override;
		float queryDevice() override;
	};
}
