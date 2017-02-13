/*
 * FirmwareSetting.cpp
 *
 *  Created on: 24 thg 1, 2017
 *      Author: MyPC
 */

#include "FirmwareSetting.h"

FirmwareSetting::FirmwareSetting() {
	// TODO Auto-generated constructor stub
	_fw = new FirmwareSetting_t;
	EEPROM.begin(sizeof(FirmwareSetting_t));
	EEPROM.get(0, _fw );
	EEPROM.end();
}

FirmwareSetting::~FirmwareSetting() {
	// TODO Auto-generated destructor stub
	saveSetting();
	delete _fw;
}

bool FirmwareSetting::saveSetting() {
	EEPROM.begin(sizeof(FirmwareSetting_t));
	EEPROM.put(0, _fw );
	EEPROM.commit();
	EEPROM.end();
	return true;
}

bool FirmwareSetting::resetSettings() {
	//strncpy(_fw.adminUser, admin_user, 16);
	//strncpy(_fw.adminPassword, admin_password, 16);
}

GPIO_t FirmwareSetting::getGPIOConfig(int index) {
	return _fw.gpio[index];
}

bool FirmwareSetting::setGPIOConfig(int index, GPIO_t gpio) {
	_fw.gpio[index] = gpio;
	return true;
}
