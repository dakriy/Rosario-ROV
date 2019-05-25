#include "LTC2451.h"
#include <cmath>
#include <cstdint>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>


void Sensor::LTC2451::setMode(Sensor::LTC2451Mode m) {
	wiringPiI2CWrite(deviceHandle, static_cast<int>(m));
}

bool Sensor::LTC2451::init() {
	deviceHandle = wiringPiI2CSetup(ADDRESS);

	if (deviceHandle < 0)
		return false;

	setMode(LTC2451Mode::ThirtyHz);

	return true;
}

float Sensor::LTC2451::getConversion() {
	uint16_t data;
	if (read(deviceHandle, &data, 2) != 2)
		return 3.3f;
	data = __builtin_bswap16(data);
	return 3.3f / 65535.f * static_cast<float>(data);
}
