#pragma once

#include <cstdint>

namespace Sensor {
	class ECEZ0 {
		const int ADDRESS = 0x64;

		const unsigned char CALIBRATION_REGISTER_START = 0x0A;
		const unsigned char CALIBRATION_REQUEST_REGISTER = 0x0E;
		const unsigned char CALIBRATION_CONFIRMATION_REGISTER = 0x0F;
		const unsigned char MODE_REGISTER = 0x06;
		// 4 bytes long
		const unsigned char EC_REGISTER_START = 0x18;
		const unsigned char TDS_REGISTER_START = 0x1C;

		int fd = -1;

		bool writeReg(unsigned char addr, unsigned char val);
		uint8_t readReg(unsigned char addr);

		float read4Reg(uint8_t startAddr);

	public:
		// This function can calibrate, when you run it, it is advised
		// to step through it one at a time.
		void calibrate(float lowuS, float highuS);
		void singleCalibrate(float uS);
		bool init();
		void activate();
		float getEC();
		float getTDM();
	};
}
