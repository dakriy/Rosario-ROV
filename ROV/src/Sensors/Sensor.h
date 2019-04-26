#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/Config.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <unordered_map>

namespace Sensor {
	struct SensorInfo {
		enum Sensor {
			// Sensor List
			Temperature = 0,
			Pressure = 1,
			Conductivity = 2,
			InternalTemperature = 3,
			InternalPressure = 4,
			InternalHumidity = 5,
			Lux = 6,
			Video = 7,

			Count
		};

		struct TemperatureMeas {
			enum Units {
				Fahrenheit,
				Celsius
			};
			Units units = Celsius;
			float temperature = 0.f;
		};

		struct PressureMeas {
			enum Units {
				PSI,
				Bar,
				Pascal
			};
			Units units = PSI;
			float pressure = 0.f;
		};

		struct ConductivityMeas {
			enum Units {
				// Siemens per meter
				SiemensM,
				// Ohm Meters
				OhmM,
			};
			Units units = SiemensM;
			float conductivity = 0.f;
		};

		struct InternalTemperatureMeas {
			enum Units {
				Fahrenheit,
				Celsius
			};
			Units units = Celsius;
			float temperature = 0.f;
		};

		struct InternalPressureMeas {
			enum Units {
				PSI,
				Bar,
				Pascal
			};
			Units units = PSI;
			float pressure = 0.f;
		};

		struct InternalHumidityMeas {
			// Only one unit
			// Grams of vapor / m^3 of air
			float humidity = 0.f;
		};

		struct LightMeas {
			// Only one unit
			// Lux
			float lux = 0.f;
		};

		struct VideoFrame {
			unsigned len = 0;
			const sf::Uint8 * data = nullptr;
		};

		sf::Time time = sf::Time::Zero;

		union Measurement {
			TemperatureMeas t;
			PressureMeas p;
			ConductivityMeas c;
			InternalTemperatureMeas iT;
			InternalPressureMeas iP;
			InternalHumidityMeas iH;
			LightMeas l;
			VideoFrame f;
		};

		std::unordered_map<Sensor, Measurement> intelligence;

		bool hasDataForSensor(Sensor s) const;

		const Measurement * getSensorData(Sensor s) const;
	};

	class Sensor {
	protected:
		float period = 1;
		sf::Clock timer;
	public:
		float getPeriod();
		void setPeriod(float T);
		float getFrequency();
		void setFrequency(float F);
		bool needsPolled();
	};
}