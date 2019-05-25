#pragma once

namespace Sensor {
	enum class LTC2451Mode {
		SixtyHz,
		ThirtyHz
	};

	class LTC2451 {
	protected:
		const int ADDRESS = 0x14;
		const float VOLTAGE = 3.3f;
		int deviceHandle = -1;
		void setMode(LTC2451Mode mode);
	public:
		bool init();
		float getConversion();
	};
}
