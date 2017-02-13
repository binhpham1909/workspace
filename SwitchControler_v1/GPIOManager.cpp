/*
 * GPIOManager.cpp
 *
 *  Created on: 24 thg 1, 2017
 *      Author: MyPC
 */
#include "GPIOManager.h"

GPIOManager::GPIOManager() {

}

GPIOManager::~GPIOManager() {
}
void GPIOManager::update() {
	for (int i = 0; i < MAX_GPIO_PIN; ++i) {
		GPIO_t _gpio = FirmwareSetting::getInstance()->_fw.gpio[i];
		int _pin = GPIOList[i];
		if (_gpio.type == IN) {
			_gpio.value = digitalRead(_pin);
		}else if(_gpio.type == FLIP){
			if(_gpio.value){
				long _curr = millis();
				if(_gpio.FlipState){
					if((_curr-lastUpdate[i]) > _gpio.FlipON){
						_gpio.FlipState = LOW;
					}
				}else{
					if((_curr-lastUpdate[i]) > _gpio.FlipOff){
						_gpio.FlipState = HIGH;
					}
				}
			}else{
				_gpio.FlipState = LOW;
			}
			digitalWrite(_pin, _gpio.FlipState);
		} else {
			digitalWrite(_pin, _gpio.value);
		}
	}
}

void GPIOManager::init() {
	for (int i = 0; i < MAX_GPIO_PIN; ++i) {
		int _pin = GPIOList[i];
		if (FirmwareSetting::getInstance()->_fw.gpio[i].type == IN) {
			pinMode(_pin, INPUT);
		} else{
			pinMode(_pin, OUTPUT);
		}
	}
}
