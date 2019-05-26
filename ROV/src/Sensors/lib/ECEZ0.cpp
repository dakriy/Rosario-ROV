#include "ECEZ0.h"
#include <wiringPiI2C.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <wiringPi.h>

/**
 * Takes the measuremnt in uS and multiplies it by 100,
 * it then takes each byte of the number and puts it in
 * subsequent registers of the device
 * @param uS
 */
void Sensor::ECEZ0::calibrate(float lowuS, float highuS) {
	// Clear calibrations
	writeReg(CALIBRATION_REQUEST_REGISTER, 0x01);
	delay(2000);
	// Do dry calibration
	writeReg(CALIBRATION_REQUEST_REGISTER, 0x02);
	// Wait 2 seconds
	delay(2000);

	auto valToSend = static_cast<unsigned>(lowuS * 100.f);
	std::vector<unsigned char> bytes(4);
	for (unsigned i = 0; i < 4; ++i) {
		bytes[3-i] = (valToSend >> (i * 8));
	}

	for (unsigned i = 0; i < 4; ++i) {
		writeReg(CALIBRATION_REGISTER_START + i, bytes[i]);
	}

	delay(2000);

	// Do low calibration
	writeReg(CALIBRATION_REQUEST_REGISTER, 0x04);

	delay(2000);

	// Write conductivity of high solution
	valToSend = static_cast<unsigned>(highuS * 100.f);
	for (unsigned i = 0; i < 4; ++i) {
		bytes[3-i] = (valToSend >> (i * 8));
	}

	for (unsigned i = 0; i < 4; ++i) {
		writeReg(CALIBRATION_REGISTER_START + i, bytes[i]);
	}
	delay(2000);
	// Do high calibration
	writeReg(CALIBRATION_REQUEST_REGISTER, 0x05);
	delay(1000);

	// Confirm
	auto v = readReg(CALIBRATION_CONFIRMATION_REGISTER);
	return;
}

bool Sensor::ECEZ0::init() {
	fd = wiringPiI2CSetup(ADDRESS);
	return fd > -1;
}

bool Sensor::ECEZ0::writeReg(unsigned char addr, unsigned char val) {
	unsigned char b[2];
	b[0] = addr;
	b[1] = val;
	return write(fd, b, 2) == 2;
}

uint8_t Sensor::ECEZ0::readReg(unsigned char addr) {
	if (write(fd, &addr, 1) != 1) {
		return 0xFF;
	}
	uint8_t b = 0xFF;
	if (read(fd, &b, 1) != 1) {
		return 0XFF;
	}
	return b;
}

float Sensor::ECEZ0::getEC() {
	// Put into hibernate mode
	writeReg(MODE_REGISTER, 0x00);

	return read4Reg(EC_REGISTER_START);
}

void Sensor::ECEZ0::singleCalibrate(float uS) {
	// Clear calibrations
	writeReg(CALIBRATION_REQUEST_REGISTER, 0x01);
	delay(2000);
	// Do dry calibration
	writeReg(CALIBRATION_REQUEST_REGISTER, 0x02);
	// Wait 2 seconds
	delay(2000);

	auto valToSend = static_cast<unsigned>(uS * 100.f);
	std::vector<unsigned char> bytes(4);
	for (unsigned i = 0; i < 4; ++i) {
		bytes[3-i] = (valToSend >> (i * 8));
	}

	for (unsigned i = 0; i < 4; ++i) {
		writeReg(CALIBRATION_REGISTER_START + i, bytes[i]);
	}

	delay(2000);

	// Do single point calibration
	writeReg(CALIBRATION_REQUEST_REGISTER, 0x03);

	delay(1000);

	// Confirm
	auto v = readReg(CALIBRATION_CONFIRMATION_REGISTER);
	return;
}

float Sensor::ECEZ0::read4Reg(uint8_t startAddr) {
	uint8_t bytes[4];
	for (unsigned i = 0; i < 4; ++i) {
		bytes[i] = readReg(startAddr + i);
	}

	uint32_t num = (bytes[3] << 0) | (bytes[2] << 8) | (bytes[1] << 16) | (bytes[0] << 24);

	return static_cast<float>(num) / 100.f;
}

void Sensor::ECEZ0::activate() {
	// One puts it in active mode
	writeReg(MODE_REGISTER, 0x01);

}

float Sensor::ECEZ0::getTDM() {
	// Put into hibernate mode
	writeReg(MODE_REGISTER, 0x00);

	return read4Reg(TDS_REGISTER_START);
}
