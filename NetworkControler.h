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
	void onSwitchChanged(uint8_t switchId, uint8_t switchState);
private:
	void initWiFi();
	void initMQTT();
	void initOTA();
private:
	static void connectToWifi();
	static void connectToMqtt();
	static void WiFiEvent(WiFiEvent_t event);
	static void onMqttConnect(bool sessionPresent);
	static void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
	static void onMqttSubscribe(uint16_t packetId, uint8_t qos);
	static void onMqttUnsubscribe(uint16_t packetId);
	static void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
	static void onMqttPublish(uint16_t packetId);
	//static String discMqttReasons(AsyncMqttClientDisconnectReason reason);
	static String wifiStatus(uint8_t status);
};
extern NetworkControler networkControler;
#endif /* #ifndef __NetworkControler_H__ */
