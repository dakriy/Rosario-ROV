/**************************************************************************/
/*!
    @file     Sensor::TSL2591.cpp
    @author   KT0WN (adafruit.com)
    @modified dakriy (WWU)
    This is a library for the Adafruit TSL2591 breakout board
    This library works with the Adafruit TSL2591 breakout
    ----> https://www.adafruit.com/products/1980
    Check out the links above for our tutorials and wiring diagrams
    These chips use I2C to communicate
    Adafruit invests time and resources providing this open source code,
    please support Adafruit and open-source hardware by purchasing
    products from Adafruit!
    @section LICENSE
    Software License Agreement (BSD License)
    Copyright (c) 2014 Adafruit Industries
    All rights reserved.
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/

#include "TSL2591.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>

Sensor::TSL2591::TSL2591() {
	_initialized = false;
	_integration = TSL2591_INTEGRATIONTIME_100MS;
	_gain        = TSL2591_GAIN_MED;
}

/**************************************************************************/
/*!
    @brief  Setups the I2C interface and hardware, identifies if chip is found
    @returns True if a TSL2591 is found, false on any failure
*/
/**************************************************************************/
bool Sensor::TSL2591::begin()
{
	if (_initialized)
		return true;

	deviceHandle = wiringPiI2CSetup(TSL2591_ADDR);

	if (deviceHandle < 0)
		return false;

	uint8_t id = read8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_DEVICE_ID);
	if (id != 0x50 )
		return false;

	_initialized = true;

	// Set default integration time and gain
	setTiming(_integration);
	setGain(_gain);

	// Note: by default, the device is in power down mode on bootup
	disable();

	return true;
}

/**************************************************************************/
/*!
    @brief  Enables the chip, so it's ready to take readings
*/
/**************************************************************************/
void Sensor::TSL2591::enable()
{
	if (!_initialized)
	{
		if (!begin())
		{
			return;
		}
	}

	// Enable the device by setting the control bit to 0x01
	write8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_ENABLE,
		   TSL2591_ENABLE_POWERON | TSL2591_ENABLE_AEN);
}


/**************************************************************************/
/*!
    @brief Disables the chip, so it's in power down mode
*/
/**************************************************************************/
void Sensor::TSL2591::disable()
{
	if (!_initialized) {
		if (!begin()) {
			return;
		}
	}

	// Disable the device by setting the control bit to 0x00
	write8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_ENABLE, TSL2591_ENABLE_POWEROFF);
}

/************************************************************************/
/*!
    @brief  Setter for sensor light gain
    @param  gain {@link tsl2591Gain_t} gain value
*/
/**************************************************************************/
void Sensor::TSL2591::setGain(tsl2591Gain_t gain)
{
	if (!_initialized) {
		if (!begin()) {
			return;
		}
	}

	enable();
	_gain = gain;
	write8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_CONTROL, _integration | _gain);
	disable();
}

/************************************************************************/
/*!
    @brief  Getter for sensor light gain
    @returns {@link tsl2591Gain_t} gain value
*/
/**************************************************************************/
Sensor::tsl2591Gain_t Sensor::TSL2591::getGain()
{
	return _gain;
}

/************************************************************************/
/*!
    @brief  Setter for sensor integration time setting
    @param integration {@link tsl2591IntegrationTime_t} integration time setting
*/
/**************************************************************************/
void Sensor::TSL2591::setTiming(tsl2591IntegrationTime_t integration)
{
	if (!_initialized) {
		if (!begin()) {
			return;
		}
	}

	enable();
	_integration = integration;
	write8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_CONTROL, _integration | _gain);
	disable();
}

/************************************************************************/
/*!
    @brief  Getter for sensor integration time setting
    @returns {@link tsl2591IntegrationTime_t} integration time
*/
/**************************************************************************/
Sensor::tsl2591IntegrationTime_t Sensor::TSL2591::getTiming()
{
	return _integration;
}

/************************************************************************/
/*!
    @brief  Calculates the visible Lux based on the two light sensors
    @param  ch0 Data from channel 0 (IR+Visible)
    @param  ch1 Data from channel 1 (IR)
    @returns Lux, based on AMS coefficients
*/
/**************************************************************************/
float Sensor::TSL2591::calculateLux(uint16_t ch0, uint16_t ch1)
{
	float    atime, again;
	float    cpl, lux1, lux2, lux;
	uint32_t chan0, chan1;

	// Check for overflow conditions first
	if ((ch0 == 0xFFFF) | (ch1 == 0xFFFF))
	{
		// Signal an overflow
		return 0;
	}

	// Note: This algorithm is based on preliminary coefficients
	// provided by AMS and may need to be updated in the future

	switch (_integration)
	{
		case TSL2591_INTEGRATIONTIME_100MS :
			atime = 100.0F;
			break;
		case TSL2591_INTEGRATIONTIME_200MS :
			atime = 200.0F;
			break;
		case TSL2591_INTEGRATIONTIME_300MS :
			atime = 300.0F;
			break;
		case TSL2591_INTEGRATIONTIME_400MS :
			atime = 400.0F;
			break;
		case TSL2591_INTEGRATIONTIME_500MS :
			atime = 500.0F;
			break;
		case TSL2591_INTEGRATIONTIME_600MS :
			atime = 600.0F;
			break;
		default: // 100ms
			atime = 100.0F;
			break;
	}

	switch (_gain)
	{
		case TSL2591_GAIN_LOW :
			again = 1.0F;
			break;
		case TSL2591_GAIN_MED :
			again = 25.0F;
			break;
		case TSL2591_GAIN_HIGH :
			again = 428.0F;
			break;
		case TSL2591_GAIN_MAX :
			again = 9876.0F;
			break;
		default:
			again = 1.0F;
			break;
	}

	// cpl = (ATIME * AGAIN) / DF
	cpl = (atime * again) / TSL2591_LUX_DF;

	// Original lux calculation (for reference sake)
	//lux1 = ( (float)ch0 - (TSL2591_LUX_COEFB * (float)ch1) ) / cpl;
	//lux2 = ( ( TSL2591_LUX_COEFC * (float)ch0 ) - ( TSL2591_LUX_COEFD * (float)ch1 ) ) / cpl;
	//lux = lux1 > lux2 ? lux1 : lux2;

	// Alternate lux calculation 1
	// See: https://github.com/adafruit/Sensor::TSL2591_Library/issues/14
	lux = ( ((float)ch0 - (float)ch1 )) * (1.0F - ((float)ch1/(float)ch0) ) / cpl;

	// Alternate lux calculation 2
	//lux = ( (float)ch0 - ( 1.7F * (float)ch1 ) ) / cpl;

	// Signal I2C had no errors
	return lux;
}

void Sensor::TSL2591::startConversion() {
	if (!_initialized) {
		if (!begin()) {
			return;
		}
	}

	// Enable the device
	// I guess just enabling it starts a conversion?
	enable();
}

float Sensor::TSL2591::getLight() {
	// CHAN0 must be read before CHAN1
	// See: https://forums.adafruit.com/viewtopic.php?f=19&t=124176
	auto ch0 = read16(TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN0_LOW);
	auto ch1 = read16(TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN1_LOW);
	// Disable the chip again to save power...
	disable();

	/* Early silicon seems to have issues when there is a sudden jump in */
	/* light levels. :( To work around this for now sample the sensor 2x */
	// I really don't wanna sample it twice, cause that's a max of 2 seconds. Can't wait that long. That's stupid...
	// Also we'll be sampling it lots so we should be fine.

	return calculateLux(ch0, ch1);
}

/************************************************************************/
/*!
    @brief  Reads the raw data from both light channels
    @returns 32-bit raw count where high word is IR, low word is IR+Visible
*/
/**************************************************************************/
uint32_t Sensor::TSL2591::getFullLuminosity()
{
	if (!_initialized) {
		if (!begin()) {
			return 0;
		}
	}

	// Enable the device
	enable();

	// Wait x ms for ADC to complete
	for (uint8_t d=0; d<=_integration; d++)
	{
		delay(120);
	}

	// CHAN0 must be read before CHAN1
	// See: https://forums.adafruit.com/viewtopic.php?f=19&t=124176
	uint32_t x = 0;
	uint16_t y = 0;
	y |= read16(TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN0_LOW);
	x = read16(TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN1_LOW);
	x <<= 16;
	x |= y;

	disable();

	return x;
}

/************************************************************************/
/*!
    @brief  Reads the raw data from the channel
    @param  channel Can be 0 (IR+Visible, 1 (IR) or 2 (Visible only)
    @returns 16-bit raw count, or 0 if channel is invalid
*/
/**************************************************************************/
uint16_t Sensor::TSL2591::getLuminosity(uint8_t channel)
{
	uint32_t x = getFullLuminosity();

	if (channel == TSL2591_FULLSPECTRUM)
	{
		// Reads two byte value from channel 0 (visible + infrared)
		return static_cast<uint16_t>(x & 0xFFFF);
	}
	else if (channel == TSL2591_INFRARED)
	{
		// Reads two byte value from channel 1 (infrared)
		return static_cast<uint16_t>(x >> 16);
	}
	else if (channel == TSL2591_VISIBLE)
	{
		// Reads all and subtracts out just the visible!
		return static_cast<uint16_t>( (x & 0xFFFF) - (x >> 16));
	}

	// unknown channel!
	return 0;
}

/************************************************************************/
/*!
    @brief  Set up the interrupt to go off when light level is outside the lower/upper range.
    @param  lowerThreshold Raw light data reading level that is the lower value threshold for interrupt
    @param  upperThreshold Raw light data reading level that is the higher value threshold for interrupt
    @param  persist How many counts we must be outside range for interrupt to fire, default is any single value
*/
/**************************************************************************/
//void Sensor::TSL2591::registerInterrupt(uint16_t lowerThreshold, uint16_t upperThreshold, tsl2591Persist_t persist = TSL2591_PERSIST_ANY)
//{
//	if (!_initialized) {
//		if (!begin()) {
//			return;
//		}
//	}
//
//	enable();
//	write8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_PERSIST_FILTER,  persist);
//	write8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_THRESHOLD_AILTL, lowerThreshold);
//	write8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_THRESHOLD_AILTH, lowerThreshold >> 8);
//	write8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_THRESHOLD_AIHTL, upperThreshold);
//	write8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_THRESHOLD_AIHTH, upperThreshold >> 8);
//	disable();
//}

/************************************************************************/
/*!
    @brief  Clear interrupt status
*/
/**************************************************************************/
//void Sensor::TSL2591::clearInterrupt()
//{
//	if (!_initialized) {
//		if (!begin()) {
//			return;
//		}
//	}
//
//	enable();
//	write8(TSL2591_CLEAR_INT);
//	disable();
//}


/************************************************************************/
/*!
    @brief  Gets the most recent sensor event from the hardware status register.
    @return Sensor status as a byte. Bit 0 is ALS Valid. Bit 4 is ALS Interrupt. Bit 5 is No-persist Interrupt.
*/
/**************************************************************************/
//uint8_t Sensor::TSL2591::getStatus()
//{
//	if (!_initialized) {
//		if (!begin()) {
//			return 0;
//		}
//	}
//
//	// Enable the device
//	enable();
//	uint8_t x;
//	x = read8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_DEVICE_STATUS);
//	disable();
//	return x;
//}


uint8_t Sensor::TSL2591::read8(uint8_t reg)
{
	auto result = wiringPiI2CReadReg8(deviceHandle, reg);

	if (result < 0) {
		return 0;
	}

	return static_cast<uint8_t>(result);
}

uint16_t Sensor::TSL2591::read16(uint8_t reg)
{
	auto result = wiringPiI2CReadReg16(deviceHandle, reg);

	if (result < 0) {
		return 0;
	}
	return __builtin_bswap16(static_cast<uint16_t>(result));
}

void Sensor::TSL2591::write8(uint8_t reg, uint8_t value)
{
	wiringPiI2CWriteReg8(deviceHandle, reg, value);
}


void Sensor::TSL2591::write8(uint8_t reg)
{
	wiringPiI2CWrite(deviceHandle, reg);
}
