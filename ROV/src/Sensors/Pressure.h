#pragma once

#include "Sensor.h"
#include "lib/MS5837.h"

namespace Sensor {
	// http://www.te.com/commerce/DocumentDelivery/DDEController?Action=showdoc&DocId=Data+Sheet%7FMS5837-30BA%7FB1%7Fpdf%7FEnglish%7FENG_DS_MS5837-30BA_B1.pdf%7FCAT-BLPS0017
	class Pressure : public Sensor {
	protected:
		const SensorInfo info{
			.id = static_cast<sf::Uint8>(SensorId::Pressure),
			.maxFrequency = 50.f,
			.name = "External Pressure",
			.units = "mBar"
		};

		MS5837 pressure;
	public:
		const SensorInfo& getSensorInfo() override;
		bool setup() override;
		void initiateConversion() override;
		float queryDevice() override;
	};
}
