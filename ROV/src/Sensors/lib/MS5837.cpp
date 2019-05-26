#include "MS5837.h"
#include "../../Utilities/I2CAccess.h"
#include <wiringPi.h>
#include <cmath>
#include <wiringPiI2C.h>

#define I2C_SMBUS	0x0720	/* SMBus-level access */
#define MS5837_ADDR               0x76
#define MS5837_RESET              0x1E
#define MS5837_ADC_READ           0x00
#define MS5837_PROM_READ          0xA0
#define MS5837_CONVERT_D1_8192    0x4A
#define MS5837_CONVERT_D2_8192    0x5A

const float Sensor::MS5837::Pa = 100.0f;
const float Sensor::MS5837::bar = 0.001f;
const float Sensor::MS5837::mbar = 1.0f;

const uint8_t Sensor::MS5837::MS5837_30BA = 0;
const uint8_t Sensor::MS5837::MS5837_02BA = 1;

Sensor::MS5837::MS5837() {
	fluidDensity = 1029;
}

bool Sensor::MS5837::init() {
	deviceHandle = wiringPiI2CSetup(MS5837_ADDR);

	if (deviceHandle < 0) {
		return false;
	}

	// Reset the MS5837, per datasheet
	if (wiringPiI2CWrite(deviceHandle, MS5837_RESET) < 0) return false;
	// Wait for reset to complete
	delay(10);

	// Read calibration values and CRC
	for ( uint8_t i = 0 ; i < 7 ; i++ ) {
		auto result = wiringPiI2CReadReg16(deviceHandle, MS5837_PROM_READ+i*2);

		if (result < 0) {
			return false;
		}
		// SMBus is little-endian and I guess it sent it big endian, so need to swap it...
		C[i] = __builtin_bswap16(static_cast<uint16_t>(result));
//		C[i] = static_cast<uint16_t>(((result & 0xFF) << 8) | (result >> 8));
	}

	// Verify that data is correct with CRC
	auto crcRead = static_cast<uint8_t>(C[0] >> 12);
	auto crcCalculated = crc4(C);


	return crcCalculated == crcRead;
}

void Sensor::MS5837::setModel(uint8_t model) {
	_model = model;
}

void Sensor::MS5837::setFluidDensity(float density) {
	fluidDensity = density;
}

void Sensor::MS5837::read() {
	// Request D1 conversion
	wiringPiI2CWrite(deviceHandle, MS5837_CONVERT_D1_8192);

	delay(20); // Max conversion time per datasheet

	auto result = I2CReadReg24(deviceHandle, MS5837_ADC_READ);
	if (result < 0) {
		return;
	}

	// Endianness conversion? Hopefully... This is dumb
	D1 = __builtin_bswap32(result) >> 8;

	// Request D2 conversion
	wiringPiI2CWrite(deviceHandle, MS5837_CONVERT_D2_8192);

	delay(20); // Max conversion time per datasheet

	result = I2CReadReg24(deviceHandle, MS5837_ADC_READ);
	if (result < 0) {
		return;
	}

	D2 = __builtin_bswap32(result) >> 8;

	calculate();
}

void Sensor::MS5837::calculate() {
	// Given C1-C6 and D1, D2, calculated TEMP and P
	// Do conversion first and then second order temp compensation

	int32_t dT = 0;
	int64_t SENS = 0;
	int64_t OFF = 0;
	int32_t SENSi = 0;
	int32_t OFFi = 0;
	int32_t Ti = 0;
	int64_t OFF2 = 0;
	int64_t SENS2 = 0;

	// Terms called
	dT = D2-uint32_t(C[5])*256l;
	if ( _model == MS5837_02BA ) {
		SENS = static_cast<int64_t>(C[1])*65536l+(static_cast<int64_t>(C[3])*dT)/128l;
		OFF = static_cast<int64_t>(C[2])*131072l+(static_cast<int64_t>(C[4])*dT)/64l;
		P = static_cast<int32_t>((D1*SENS/(2097152l)-OFF)/(32768l));
	} else {
		SENS = static_cast<int64_t>(C[1])*32768l+(static_cast<int64_t>(C[3])*dT)/256l;
		OFF = static_cast<int64_t>(C[2])*65536l+(static_cast<int64_t>(C[4])*dT)/128l;
		P = static_cast<int32_t>((D1*SENS/(2097152l)-OFF)/(8192l));
	}

	// Temp conversion
	TEMP = static_cast<int32_t>(2000l+int64_t(dT)*C[6]/8388608LL);

	//Second order compensation
	if ( _model == MS5837_02BA ) {
		if((TEMP/100)<20){         //Low temp
			Ti = static_cast<int32_t>((11*static_cast<int64_t>(dT)*static_cast<int64_t>(dT))/(34359738368LL));
			OFFi = (31*(TEMP-2000)*(TEMP-2000))/8;
			SENSi = (63*(TEMP-2000)*(TEMP-2000))/32;
		}
	} else {
		if((TEMP/100)<20){         //Low temp
			Ti = static_cast<int32_t>((3*static_cast<int64_t>(dT)*static_cast<int64_t>(dT))/(8589934592LL));
			OFFi = (3*(TEMP-2000)*(TEMP-2000))/2;
			SENSi = (5*(TEMP-2000)*(TEMP-2000))/8;
			if((TEMP/100)<-15){    //Very low temp
				OFFi = OFFi+7*(TEMP+1500l)*(TEMP+1500l);
				SENSi = SENSi+4*(TEMP+1500l)*(TEMP+1500l);
			}
		}
		else if((TEMP/100)>=20){    //High temp
			Ti = static_cast<int32_t>(2*(dT*dT)/(137438953472LL));
			OFFi = (1*(TEMP-2000)*(TEMP-2000))/16;
			SENSi = 0;
		}
	}

	OFF2 = OFF-OFFi;           //Calculate pressure and temp second order
	SENS2 = SENS-SENSi;

	TEMP = (TEMP-Ti);

	if ( _model == MS5837_02BA ) {
		P = static_cast<uint32_t>(((D1*SENS2)/2097152l-OFF2)/32768l);
	} else {
		P = static_cast<uint32_t>(((D1*SENS2)/2097152l-OFF2)/8192l);
	}
}

float Sensor::MS5837::pressure(float conversion) {
	if ( _model == MS5837_02BA ) {
		return P*conversion/100.0f;
	}
	else {
		return P*conversion/10.0f;
	}
}

float Sensor::MS5837::temperature() {
	return TEMP/100.0f;
}

float Sensor::MS5837::depth() {
	return static_cast<float>((pressure(Sensor::MS5837::Pa)-101300)/(fluidDensity*9.80665));
}

float Sensor::MS5837::altitude() {
	return static_cast<float>((1-pow((pressure()/1013.25),.190284))*145366.45*.3048);
}


uint8_t Sensor::MS5837::crc4(uint16_t n_prom[]) {
	uint16_t n_rem = 0;

	n_prom[0] = static_cast<uint16_t>(((n_prom[0]) & 0x0FFF));
	n_prom[7] = 0;

	for ( uint8_t i = 0 ; i < 16; i++ ) {
		if ( i%2 == 1 ) {
			n_rem ^= (uint16_t)((n_prom[i>>1]) & 0x00FF);
		} else {
			n_rem ^= (uint16_t)(n_prom[i>>1] >> 8);
		}
		for ( uint8_t n_bit = 8 ; n_bit > 0 ; n_bit-- ) {
			if ( n_rem & 0x8000 ) {
				n_rem = static_cast<uint16_t>((n_rem << 1) ^ 0x3000);
			} else {
				n_rem = (n_rem << 1);
			}
		}
	}

	n_rem = static_cast<uint16_t>(((n_rem >> 12) & 0x000F));

	return static_cast<uint8_t>(n_rem ^ 0x00);
}
