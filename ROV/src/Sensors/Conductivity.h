#pragma once

#include "Sensor.h"
#include "lib/ECEZ0.h"
#include "../Core/Event.h"
#include "../Core/EventHandler.h"

namespace Sensor {
	class Conductivity : public Sensor{
	protected:
		const SensorInfo info{
				.id = static_cast<sf::Uint8>(SensorId::Conductivity),
				.maxFrequency = 1.f / 0.64f,
				.name = "Salinity",
				.units = "unitless"
		};
		ECEZ0 conductivity;
		EVENT_FUNC_INDEX_CORE tempTaken = nullptr;
	public:
		Conductivity();
		const SensorInfo& getSensorInfo() override;
		bool setup() override;
		void initiateConversion() override;
		float queryDevice() override;
		~Conductivity() override;
	};
}
