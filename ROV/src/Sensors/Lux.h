#pragma once

#include "Sensor.h"

namespace Sensor {
	class Lux : public Sensor {
	protected:
		const SensorInfo info{
			.id = static_cast<sf::Uint8 >(SensorId::Lux),
			.maxFrequency = 50.f,
			.name = "Light/Lux Sensor",
			.units = "Lux"
		};


	};
}

