/***************************************************************************
Modified BSD License
====================
Copyright © 2016, Andrei Vainik
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. Neither the name of the organization nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
This piece of code was combined from several sources
https://github.com/adafruit/Adafruit_BME280_Library
https://cdn-shop.adafruit.com/datasheets/BST-BME280_DS001-10.pdf
https://projects.drogon.net/raspberry-pi/wiringpi/i2c-library/
Compensation functions and altitude function originally from:
https://github.com/adafruit/Adafruit_BME280_Library/blob/master/Adafruit_BME280.cpp
***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor
  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650
  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.
  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!
  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************
****************************************************************************/

#include <wiringPiI2C.h>
#include "bme280.h"


int Sensor::bme280::fd = -1;
Sensor::bme280_calib_data Sensor::bme280::calibData {};
Sensor::bme280_raw_data Sensor::bme280::rawData {};
int32_t Sensor::bme280::tempCalib = 0;
bool Sensor::bme280::initialized_ = false;
bool Sensor::bme280::queryied_ = false;

void Sensor::bme280::readCalibrationData() {
	calibData.dig_T1 = (uint16_t)wiringPiI2CReadReg16(fd, BME280_REGISTER_DIG_T1);
	calibData.dig_T2 = (int16_t)wiringPiI2CReadReg16(fd, BME280_REGISTER_DIG_T2);
	calibData.dig_T3 = (int16_t)wiringPiI2CReadReg16(fd, BME280_REGISTER_DIG_T3);

	calibData.dig_P1 = (uint16_t)wiringPiI2CReadReg16(fd, BME280_REGISTER_DIG_P1);
	calibData.dig_P2 = (int16_t)wiringPiI2CReadReg16(fd, BME280_REGISTER_DIG_P2);
	calibData.dig_P3 = (int16_t)wiringPiI2CReadReg16(fd, BME280_REGISTER_DIG_P3);
	calibData.dig_P4 = (int16_t)wiringPiI2CReadReg16(fd, BME280_REGISTER_DIG_P4);
	calibData.dig_P5 = (int16_t)wiringPiI2CReadReg16(fd, BME280_REGISTER_DIG_P5);
	calibData.dig_P6 = (int16_t)wiringPiI2CReadReg16(fd, BME280_REGISTER_DIG_P6);
	calibData.dig_P7 = (int16_t)wiringPiI2CReadReg16(fd, BME280_REGISTER_DIG_P7);
	calibData.dig_P8 = (int16_t)wiringPiI2CReadReg16(fd, BME280_REGISTER_DIG_P8);
	calibData.dig_P9 = (int16_t)wiringPiI2CReadReg16(fd, BME280_REGISTER_DIG_P9);

	calibData.dig_H1 = (uint8_t)wiringPiI2CReadReg8(fd, BME280_REGISTER_DIG_H1);
	calibData.dig_H2 = (int16_t)wiringPiI2CReadReg16(fd, BME280_REGISTER_DIG_H2);
	calibData.dig_H3 = (uint8_t)wiringPiI2CReadReg8(fd, BME280_REGISTER_DIG_H3);
	calibData.dig_H4 = (wiringPiI2CReadReg8(fd, BME280_REGISTER_DIG_H4) << 4) | (wiringPiI2CReadReg8(fd, BME280_REGISTER_DIG_H4+1) & 0xF);
	calibData.dig_H5 = (wiringPiI2CReadReg8(fd, BME280_REGISTER_DIG_H5+1) << 4) | (wiringPiI2CReadReg8(fd, BME280_REGISTER_DIG_H5) >> 4);
	calibData.dig_H6 = (int8_t)wiringPiI2CReadReg8(fd, BME280_REGISTER_DIG_H6);
}

void Sensor::bme280::getTemperatureCalibration() {
	if (queryied_) {
		int32_t var1  = ((((rawData.temperature>>3) - ((int32_t)calibData.dig_T1 <<1))) *
						 ((int32_t)calibData.dig_T2)) >> 11;

		int32_t var2  = (((((rawData.temperature>>4) - ((int32_t)calibData.dig_T1)) *
						   ((rawData.temperature>>4) - ((int32_t)calibData.dig_T1))) >> 12) *
						 ((int32_t)calibData.dig_T3)) >> 14;

		tempCalib = var1 + var2;
	}
}

float Sensor::bme280::compensateTemperature() {
	getTemperatureCalibration();
	queryied_ = false;
	float T  = (tempCalib * 5 + 128) >> 8;
	return T/100.f;
}

float Sensor::bme280::compensatePressure() {
	getTemperatureCalibration();
	queryied_ = false;
	int64_t var1, var2, p;

	var1 = ((int64_t)tempCalib) - 128000;
	var2 = var1 * var1 * (int64_t)calibData.dig_P6;
	var2 = var2 + ((var1*(int64_t)calibData.dig_P5)<<17);
	var2 = var2 + (((int64_t)calibData.dig_P4)<<35);
	var1 = ((var1 * var1 * (int64_t)calibData.dig_P3)>>8) +
		   ((var1 * (int64_t)calibData.dig_P2)<<12);
	var1 = (((((int64_t)1)<<47)+var1))*((int64_t)calibData.dig_P1)>>33;

	if (var1 == 0) {
		return 0;  // avoid exception caused by division by zero
	}
	p = 1048576 - rawData.pressure;
	p = (((p<<31) - var2)*3125) / var1;
	var1 = (((int64_t)calibData.dig_P9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((int64_t)calibData.dig_P8) * p) >> 19;

	p = ((p + var1 + var2) >> 8) + (((int64_t)calibData.dig_P7)<<4);
	return (float)p/256.f;
}

float Sensor::bme280::compensateHumidity() {
	getTemperatureCalibration();
	queryied_ = false;
	int32_t v_x1_u32r;

	v_x1_u32r = (tempCalib - ((int32_t)76800));

	v_x1_u32r = (((((rawData.humidity << 14) - (((int32_t)calibData.dig_H4) << 20) -
					(((int32_t)calibData.dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
				 (((((((v_x1_u32r * ((int32_t)calibData.dig_H6)) >> 10) *
					  (((v_x1_u32r * ((int32_t)calibData.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
					((int32_t)2097152)) * ((int32_t)calibData.dig_H2) + 8192) >> 14));

	v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
							   ((int32_t)calibData.dig_H1)) >> 4));

	v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
	v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
	float h = (v_x1_u32r>>12);
	return h / 1024.0;
}

void Sensor::bme280::getRawData() {
	if (queryied_) return;
	queryied_ = true;
	wiringPiI2CWrite(fd, 0xf7);

	rawData.pmsb = wiringPiI2CRead(fd);
	rawData.plsb = wiringPiI2CRead(fd);
	rawData.pxsb = wiringPiI2CRead(fd);

	rawData.tmsb = wiringPiI2CRead(fd);
	rawData.tlsb = wiringPiI2CRead(fd);
	rawData.txsb = wiringPiI2CRead(fd);

	rawData.hmsb = wiringPiI2CRead(fd);
	rawData.hlsb = wiringPiI2CRead(fd);

	rawData.temperature = 0;
	rawData.temperature = (rawData.temperature | rawData.tmsb) << 8;
	rawData.temperature = (rawData.temperature | rawData.tlsb) << 8;
	rawData.temperature = (rawData.temperature | rawData.txsb) >> 4;

	rawData.pressure = 0;
	rawData.pressure = (rawData.pressure | rawData.pmsb) << 8;
	rawData.pressure = (rawData.pressure | rawData.plsb) << 8;
	rawData.pressure = (rawData.pressure | rawData.pxsb) >> 4;

	rawData.humidity = 0;
	rawData.humidity = (rawData.humidity | rawData.hmsb) << 8;
	rawData.humidity = (rawData.humidity | rawData.hlsb);
}

bool Sensor::bme280::initialize() {
	if (initialized_)
		return true;
	fd = wiringPiI2CSetup(BME280_ADDRESS);
	if (fd < 0) {
		return false;
	}

	readCalibrationData();
	// pressure x16, temperature x2, humidity x1
	// Using recommended Indoor Mode
	wiringPiI2CWriteReg8(fd, 0xf2, 0x01);   // humidity oversampling x 1
	wiringPiI2CWriteReg8(fd, 0xf4, 0xAB); 	// pressure oversampling x 16, temperature oversampling x2, mode normal

	// normal mode, t standby = 0.5 ms, filter coefficient 16
	wiringPiI2CWriteReg8(fd, 0xf5, 0x08);
	initialized_ = true;
	return true;

}
