#include "TSYS01.h"

#include "TSYS01.h"
#include "../../Utilities/I2CAccess.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define TSYS01_ADDR                        0x77
#define TSYS01_RESET                       0x1E
#define TSYS01_ADC_READ                    0x00
#define TSYS01_ADC_TEMP_CONV               0x48
#define TSYS01_PROM_READ                   0XA0

bool Sensor::TSYS01::init() {
	deviceHandle = wiringPiI2CSetup(TSYS01_ADDR);

	if (deviceHandle < 0) {
		return false;
	}

	// Reset the TSYS01, per datasheet
	if (wiringPiI2CWrite(deviceHandle, TSYS01_RESET) < 0) return false;

	delay(10);

	// Read calibration values
	for ( uint8_t i = 0 ; i < 8 ; i++ ) {
		auto result = wiringPiI2CReadReg16(deviceHandle, TSYS01_PROM_READ+i*2);

		if (result < 0) return false;

		// Swap endianness as it was sent Big endian and interpreted it as little endian...
		C[i] = __builtin_bswap16(static_cast<uint16_t>(result));
	}

	return true;
}

void Sensor::TSYS01::read() {

	wiringPiI2CWrite(deviceHandle, TSYS01_ADC_TEMP_CONV);

	delay(10); // Max conversion time per datasheet

	auto result = I2CReadReg24(deviceHandle, TSYS01_ADC_READ);

	if (result < 0) {
		return;
	}

	// Endianness again.
	D1 = __builtin_bswap32(result) >> 8;

	calculate();
}

void Sensor::TSYS01::readTestCase() {
	C[0] = 0;
	C[1] = 28446;  //0xA2 K4
	C[2] = 24926;  //0XA4 k3
	C[3] = 36016;  //0XA6 K2
	C[4] = 32791;  //0XA8 K1
	C[5] = 40781;  //0XAA K0
	C[6] = 0;
	C[7] = 0;

	D1 = 9378708;

	adc = D1/256;

	calculate();
}

void Sensor::TSYS01::calculate() {
	adc = D1/256;

	TEMP = static_cast<float>((-2) * double(C[1]) / 1000000000000000000000.0 * adc*adc*adc*adc +
		   4 * double(C[2]) / 10000000000000000.0 * adc*adc*adc +
		   (-2) * double(C[3]) / 100000000000.0 * adc*adc +
		   1 * double(C[4]) / 1000000.0 * adc +
		   (-1.5) * double(C[5]) / 100);

}

float Sensor::TSYS01::temperature() {
	return TEMP;
}
