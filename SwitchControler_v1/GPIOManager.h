/*
 * GPIOManager.h
 *
 *  Created on: 24 thg 1, 2017
 *      Author: MyPC
 */

#ifndef GPIOMANAGER_H_
#define GPIOMANAGER_H_
#include "Arduino.h"
#include "FirmwareSetting.h"
#include "GlobalConfigs.h"
#include "Singleton.h"


class GPIOManager : public Singleton<GPIOManager> {
public:
	GPIOManager();
	~GPIOManager();
	void init();
	void update();
private:
	long lastUpdate[MAX_GPIO_PIN];

};
#endif /* GPIOMANAGER_H_ */
