/*
 * Commander.h
 *
 *  Created on: 4 thg 2, 2017
 *      Author: MyPC
 */

#ifndef COMMANDER_H_
#define COMMANDER_H_

#include "ArduinoJson.h"

#include "GlobalConfigs.h"
#include "Singleton.h"
#include "ModuleSettings.h"

#define COMM_COM	"cmd"
const char JsonTrue[] PROGMEM = "{\"status\":true}";
const char JsonFalse[] PROGMEM = "{\"status\":false}";
class Commander : public Singleton<Commander> {
	public:
		Commander();
		virtual ~Commander();
		String process(String commandJson);
	private:
};

#endif /* COMMANDER_H_ */
