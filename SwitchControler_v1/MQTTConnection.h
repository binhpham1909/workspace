/*
 * MQTTConnection.h
 *
 *  Created on: 4 thg 2, 2017
 *      Author: MyPC
 */

#ifndef MQTTCONNECTION_H_
#define MQTTCONNECTION_H_

#include "Singleton.h"
#include "ModuleSettings.h"
#include "Commander.h"
#include "WifiManager.h"
#include <PubSubClient.h>
#include "GlobalConfigs.h"


class MQTTConnection : public Singleton<MQTTConnection>{
public:
	MQTTConnection();
	virtual ~MQTTConnection();
	void loop();
private:
	void callback(char* topic, uint8_t* payload, unsigned int length);
	WiFiClient wclient;
	PubSubClient *client;
};

#endif /* MQTTCONNECTION_H_ */
