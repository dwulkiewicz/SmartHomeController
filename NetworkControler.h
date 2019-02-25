#ifndef __NetworkControler_H__
#define __NetworkControler_H__

#include <Arduino.h>
#include <AsyncMqttClient.h>
#include <WiFi.h>

#define MQTT_PAYLOAD_BUF_SIZE 100

class NetworkControler
{
public:
	NetworkControler();
	void init();
	void loop();
	static String getHostName();
	void onSwitchChange(uint8_t src, uint8_t switchId, uint8_t switchState);
private:
	void initWiFi();
	void initMQTT();
	void initOTA();
	bool reconnect();
	static void mqttCallback(char* topic, byte* payload, unsigned int length);
	void mqttCallbackLoop();
	static String statusMqttToString(int status);
};
extern NetworkControler networkControler;
#endif /* #ifndef __NetworkControler_H__ */
