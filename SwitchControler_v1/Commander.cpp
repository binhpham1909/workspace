/*
 * Commander.cpp
 *
 *  Created on: 4 thg 2, 2017
 *      Author: MyPC
 */

#include "Commander.h"

Commander::Commander() {
	// TODO Auto-generated constructor stub

}

Commander::~Commander() {
	// TODO Auto-generated destructor stub
}

String Commander::process(String commandJson, CmdProvider_t cmdr ) {
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(commandJson);
	if(cmdr == MQTT){
		String pass = root["cmdPassword"].asString();
		if(!(pass == DeviceSetting::getInstance()->getUserPassword()))
			return FPSTR(JsonFalse);
	}
	String cmd = root["cmd"].asString();
	DBG(root["cmd"].asString());
	if(cmd=="control"){
		DBG("control");
	}
	else if(cmd=="networkSetup") {
		DBG(root["dhcp"].asString());
		DBG(root["ssid"].asString());
		DBG(root["wifiPassword"].asString());
		DBG(root["port"].asString());
		DBG(root["ip"].asString());
		DBG(root["mask"].asString());
		DBG(root["gw"].asString());
		bool dhcp = root["dhcp"];
		bool result;
		if(dhcp){
			result = DeviceSetting::getInstance()->setSTAssid(root["ssid"].asString())&
					DeviceSetting::getInstance()->setSTApassword(root["wifiPassword"].asString())&
					DeviceSetting::getInstance()->setHTTPPort(root["port"].as<int>())&
					DeviceSetting::getInstance()->setDHCP(dhcp);
		}else{
			result = DeviceSetting::getInstance()->setSTAssid(root["ssid"].asString())&
					DeviceSetting::getInstance()->setSTApassword(root["wifiPassword"].asString())&
					DeviceSetting::getInstance()->setHTTPPort(root["port"].as<int>())&
					DeviceSetting::getInstance()->setStaticIP(root["ip"].asString())&
					DeviceSetting::getInstance()->setMask(root["mask"].asString())&
					DeviceSetting::getInstance()->setGateway(root["gw"].asString())&
					DeviceSetting::getInstance()->setDHCP(dhcp);
		}
		if(result){
			DeviceSetting::getInstance()->saveSetting();
			return FPSTR(JsonTrue);
		}
		else return FPSTR(JsonFalse);
	}
	else if(cmd=="debug") {
		DBG("debug");
	}else {
		DBG("exit");
	}
	return FPSTR(JsonFalse);
}
