/*
 * MQTTConnection.cpp
 *
 *  Created on: 4 thg 2, 2017
 *      Author: MyPC
 */

#include "MQTTConnection.h"

MQTTConnection::MQTTConnection() {
	// TODO Auto-generated constructor stub
}

MQTTConnection::~MQTTConnection() {
	// TODO Auto-generated destructor stub
	delete _client;
}


void MQTTConnection::loop() {
	if (WifiManager::getInstance()->isSTAConnected()) {
		if (!_client->connected()) {
			if (_client->connect(DeviceSetting::getInstance()->getDeviceSerial().c_str(),DeviceSetting::getInstance()->getMqttAPI().c_str(),DeviceSetting::getInstance()->getMqttAPI().c_str())) {
				_client->subscribe(_topic.c_str());
			}
		}
		if (_client->connected())
			_client->loop();
	}
}

void MQTTConnection::init() {
	_server = DeviceSetting::getInstance()->getMqttServer();
	DBG(_server);
	_topic = "hbinvent/";
	_topic += DeviceSetting::getInstance()->getDeviceSerial();
	DBG(_topic);
	_client = new PubSubClient(_wclient);
	_client->setServer(_server.c_str(), DeviceSetting::getInstance()->getMQTTPort());
	_client->setCallback(std::bind(&MQTTConnection::callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	// Lay thong tin topic: hbinvent/deviceSerial

}

void MQTTConnection::callback(char* topic, uint8_t* payload, unsigned int length){
		String msg;
		//char msg[length];
		DBG0(topic);
		DBG0(" => ");
		for (unsigned int i = 0; i < length; i++) {
			msg += (char)(payload[i]);
		}
		DBG(msg);
		Commander::getInstance()->process(msg, MQTT);
}
;

bool MQTTConnection::publish(String message) {
	if (_client->connected()){
		_client->publish(_topic.c_str(), message.c_str());
		return true;
	}else
		return false;
}
