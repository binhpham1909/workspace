/*
 * FirmwareSetting.h
 *
 *  Created on: 24 thg 1, 2017
 *      Author: MyPC
 */

#ifndef FIRMWARESETTING_H_
#define FIRMWARESETTING_H_
#include "Singleton.h"
#include <EEPROM.h>

#include "GlobalConfigs.h"
#include "GPIOManager.h"

#define DV_SERIAL_LEN	16
#define DV_USER_LEN		16
#define DV_GPIO_MAX		9


typedef struct __attribute__((packed)){
	int firstBoot;
	char deviceSerial[DV_SERIAL_LEN + 1];
	char adminUser[DV_USER_LEN + 1];
	char adminPassword[DV_USER_LEN + 1];
	GPIO_t gpio[MAX_GPIO_PIN];
} FirmwareSetting_t;

class FirmwareSetting : public Singleton<FirmwareSetting> {
	public:
		FirmwareSetting();
		virtual ~FirmwareSetting();
		FirmwareSetting_t *_fw;
		bool saveSetting();
		bool resetSettings();
	private:
		GPIO_t getGPIOConfig(int index);
		bool setGPIOConfig(int index, GPIO_t gpio);
};

const char admin_user[] PROGMEM = "admin";
const char admin_password[] PROGMEM = "TNENVIBH1+";
#endif /* FIRMWARESETTING_H_ */
