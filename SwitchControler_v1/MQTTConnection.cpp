/*
 * MQTTConnection.cpp
 *
 *  Created on: 4 thg 2, 2017
 *      Author: MyPC
 */

#include "MQTTConnection.h"

MQTTConnection::MQTTConnection() {
	// TODO Auto-generated constructor stub
	const char *server = "mqtt.hbinvent.com";
	client = new PubSubClient(wclient);
	client->setServer(server, 1883);
	client->setCallback(std::bind(&MQTTConnection::callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

MQTTConnection::~MQTTConnection() {
	// TODO Auto-generated destructor stub
	delete client;
}


void MQTTConnection::loop() {
	if ((WiFi.status() == WL_CONNECTED)&&WifiManager::getInstance()->inSTAMode()) {
		if (!client->connected()) {
			if (client->connect("arduinoClient",ModuleSettings::getInstance()->getMqttAPI().c_str(),ModuleSettings::getInstance()->getMqttAPI().c_str())) {
				client->subscribe("private/binhpham/+");
			}
		}
		if (client->connected())
			client->loop();
	}
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
		Commander::getInstance()->process(msg);
};
