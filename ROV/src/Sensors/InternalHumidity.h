#pragma once

#include "Sensor.h"

namespace Sensor {
	class InternalHumidity : public Sensor {
	protected:
		const SensorInfo info{
				.id = static_cast<sf::Uint8>(SensorId::InternalHumidity),
				.maxFrequency = 25.f,
				.name = "Internal Humidity",
				.units = "%"
		};
	public:
		const SensorInfo& getSensorInfo() override;
		bool setup() override;
		void initiateConversion() override;
		float queryDevice() override;
	};
}
