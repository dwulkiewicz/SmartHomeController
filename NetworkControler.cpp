/************************************************************************/
/*                                                                      */
/*              Project:  SmartHomeControler							*/
/*              Hardware: ESP32, Nextion 7.0, DS1307, BME280            */
/*                                                                      */
/*              Author: Dariusz Wulkiewicz                              */
/*                      d.wulkiewicz@gmail.com                          */
/*                                                                      */
/*              Date: 01.2019                                           */
/************************************************************************/

#include <Arduino.h>

extern "C" {
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
}

#include "PubSubClient.h"

#include <ArduinoOTA.h>
#include "NetworkControler.h"
#include "DisplayControler.h"
#include "EventDispatcher.h"
#include "Configuration.h"
#include "Constants.h"

//TODO: przenie nazwy kolejek do konfiguracji
const char* switchesReqChannel01 = "switches/req/channel01";
const char* switchesReqChannel02 = "switches/req/channel02";
const char* switchesReqChannel03 = "switches/req/channel03";

const char* switchesRespChannel01 = "switches/resp/channel01";
const char* switchesRespChannel02 = "switches/resp/channel02";
const char* switchesRespChannel03 = "switches/resp/channel03";

const char* sensorsBME280TemperatureTopic = "sensors/bme280/temperature";
const char* sensorsBME280HumidityTopic = "sensors/bme280/humidity";
const char* sensorsBME280PressureTopic = "sensors/bme280/pressure";

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;
char payload_buf[MQTT_PAYLOAD_BUF_SIZE];
//----------------------------------------------------------------------------------------
NetworkControler::NetworkControler() {
}
//----------------------------------------------------------------------------------------
String NetworkControler::getHostName() {
	// Set Hostname.
	char buf[15];
	uint64_t chipid = ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
	sprintf(buf, "%04x%08x", (uint16_t)(chipid >> 32) /*High 2 bytes*/, (uint32_t)chipid /*Low 4bytes*/);
	return HOSTNAME_PREFIX + String(buf);
}
//----------------------------------------------------------------------------------------
void NetworkControler::init() {
	wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));
	mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));

	WiFi.onEvent(WiFiEvent);

	mqttClient.onConnect(NetworkControler::onMqttConnect);
	mqttClient.onDisconnect(NetworkControler::onMqttDisconnect);
	mqttClient.onSubscribe(NetworkControler::onMqttSubscribe);
	mqttClient.onUnsubscribe(NetworkControler::onMqttUnsubscribe);
	mqttClient.onMessage(NetworkControler::onMqttMessage);
	mqttClient.onPublish(NetworkControler::onMqttPublish);
	mqttClient.setServer(configuration.mqttServer.c_str(), configuration.mqttPort);
	//String hostname = NetworkControler::getHostName();
	//logger.log(info, "NetworkControler::init() ClientId: %s\r\n", hostname.c_str());
	//mqttClient.setClientId(hostname.c_str());

	connectToWifi();

	initOTA();
}
//----------------------------------------------------------------------------------------
void NetworkControler::initOTA() {
	ArduinoOTA
		.onStart([]() {
		String type;
		if (ArduinoOTA.getCommand() == U_FLASH)
			type = "sketch";
		else // U_SPIFFS
			type = "filesystem";

		// NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
		Serial.println("Start updating " + type);
	})
		.onEnd([]() {
		Serial.println("\nEnd");
	})
		.onProgress([](unsigned int progress, unsigned int total) {
		Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
	})
		.onError([](ota_error_t error) {
		Serial.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
		else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
		else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
		else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
		else if (error == OTA_END_ERROR) Serial.println("End Failed");
	});
	ArduinoOTA.setHostname((const char *)NetworkControler::getHostName().c_str());
	ArduinoOTA.begin();
}
//----------------------------------------------------------------------------------------
void NetworkControler::loop() {

	//Handle OTA server.
	ArduinoOTA.handle();
}
//----------------------------------------------------------------------------------------
void NetworkControler::connectToWifi() {
	logger.log(info, "NetworkControler::connectToWifi() SSID: %s\r\n", configuration.wifiSSID.c_str());
	String hostname = NetworkControler::getHostName();
	WiFi.begin(configuration.wifiSSID.c_str(), configuration.wifiPassword.c_str());
	WiFi.setHostname(hostname.c_str());
}
//----------------------------------------------------------------------------------------
void NetworkControler::connectToMqtt() {
	logger.log(info, "NetworkControler::connectToMqtt() Host: %s\r\n", configuration.mqttServer.c_str());
	mqttClient.connect();
}
//----------------------------------------------------------------------------------------
void NetworkControler::WiFiEvent(WiFiEvent_t event) {
	logger.log(info, "NetworkControler::WiFiEvent() event: %d status: %s\r\n", event, wifiStatus(WiFi.status()).c_str());
	eventDispatcher.onWiFiStatusChange(WiFi.status());
	switch (event) {
	case SYSTEM_EVENT_STA_GOT_IP:
		logger.log(info, "NetworkControler::WiFiEvent() connected, IP: %s\r\n", WiFi.localIP().toString().c_str());
		connectToMqtt();
		break;
	case SYSTEM_EVENT_STA_DISCONNECTED:
		logger.log(info, "NetworkControler::WiFiEvent() lost connection");
		xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
		xTimerStart(wifiReconnectTimer, 0);
		break;
	}
}
//----------------------------------------------------------------------------------------
void NetworkControler::onMqttConnect(bool sessionPresent) {
	logger.log(info, "NetworkControler::onMqttConnect() Connected to MQTT, Session present: %d\r\n", sessionPresent);
	eventDispatcher.onMQTTStatusChange(MQTT_CONNECTED);

	uint16_t packetIdSub = mqttClient.subscribe(sensorsBME280TemperatureTopic, 0);
	logger.log(debug, "Subscribing ""%s"" at QoS 0, packetId: %d\r\n", sensorsBME280TemperatureTopic, packetIdSub);
	packetIdSub = mqttClient.subscribe(sensorsBME280HumidityTopic, 0);
	logger.log(debug, "Subscribing ""%s"" at QoS 0, packetId: %d\r\n", sensorsBME280HumidityTopic, packetIdSub);
	packetIdSub = mqttClient.subscribe(sensorsBME280PressureTopic, 0);
	logger.log(debug, "Subscribing ""%s"" at QoS 0, packetId: %d\r\n", sensorsBME280PressureTopic, packetIdSub);
	packetIdSub = mqttClient.subscribe(switchesReqChannel01, 0);
	logger.log(debug, "Subscribing ""%s"" at QoS 0, packetId: %d\r\n", switchesReqChannel01, packetIdSub);
	packetIdSub = mqttClient.subscribe(switchesReqChannel02, 0);
	logger.log(debug, "Subscribing ""%s"" at QoS 0, packetId: %d\r\n", switchesReqChannel02, packetIdSub);
	packetIdSub = mqttClient.subscribe(switchesReqChannel03, 0);
	logger.log(debug, "Subscribing ""%s"" at QoS 0, packetId: %d\r\n", switchesReqChannel03, packetIdSub);
}
//----------------------------------------------------------------------------------------
void NetworkControler::onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
	logger.log(info, "NetworkControler::onMqttDisconnect() reason: %d\r\n", reason);
	eventDispatcher.onMQTTStatusChange(MQTT_DISCONNECTED);
	if (WiFi.isConnected()) {
		xTimerStart(mqttReconnectTimer, 0);
	}
}
//----------------------------------------------------------------------------------------
void NetworkControler::onMqttSubscribe(uint16_t packetId, uint8_t qos) {
	logger.log(debug, "NetworkControler::onMqttSubscribe() packetId: %d qos: %d\r\n", packetId, qos);
}
//----------------------------------------------------------------------------------------
void NetworkControler::onMqttUnsubscribe(uint16_t packetId) {
	logger.log(debug, "NetworkControler::onMqttUnsubscribe() packetId: %d\r\n", packetId);
}
//----------------------------------------------------------------------------------------
void NetworkControler::onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
	memcpy(payload_buf, payload, MIN(len, MQTT_PAYLOAD_BUF_SIZE));
	payload_buf[MIN(len, MQTT_PAYLOAD_BUF_SIZE)] = '\0'; // Null terminator used to terminate the char array
	String mqttTopic = topic;
	String mqttMessage = payload_buf;

	logger.log(info, "NetworkControler::onMqttMessage() topic:[%s], qos: %d, dup %d, retain: %d, len %d, total: %d, index: %d,  msg: %s\r\n", topic, properties.qos, properties.dup, properties.retain, len, total, index, payload_buf);

	if (mqttTopic.equals(switchesReqChannel01)) {
		eventDispatcher.onSwitchChange(SWITCH_BATH_1_ID, mqttMessage.equals("on") ? SW_ON : SW_OFF);
	}
	else if (mqttTopic.equals(switchesReqChannel02)) {
		eventDispatcher.onSwitchChange(SWITCH_BATH_2_ID, mqttMessage.equals("on") ? SW_ON : SW_OFF);
	}
	else if (mqttTopic.equals(switchesReqChannel03)) {
		eventDispatcher.onSwitchChange(SWITCH_BATH_3_ID, mqttMessage.equals("on") ? SW_ON : SW_OFF);
	}
	else if (mqttTopic.equals(sensorsBME280TemperatureTopic)) {
		float outdoorTemp = mqttMessage.toFloat();
		eventDispatcher.onRefreshOutdoorTemperature(outdoorTemp);
	}
	else if (mqttTopic.equals(sensorsBME280HumidityTopic)) {
		float outdoorHumidity = mqttMessage.toFloat();
		eventDispatcher.onRefreshOutdoorHumidity(outdoorHumidity);
	}
	else if (mqttTopic.equals(sensorsBME280PressureTopic)) {
		float outdoorPressure = mqttMessage.toFloat();
		eventDispatcher.onRefreshOutdoorPressure(outdoorPressure);
	}
}
//----------------------------------------------------------------------------------------
void NetworkControler::onMqttPublish(uint16_t packetId) {
	logger.log(debug, "NetworkControler::onMqttPublish() packetId: %d\r\n", packetId);
}
//----------------------------------------------------------------------------------------
void NetworkControler::onSwitchChanged(uint8_t switchId, uint8_t switchState) {
	String msg = (switchState == SW_ON ? "on" : "off");
	const char* topic = NULL;
	switch (switchId) {
	case SWITCH_BATH_1_ID:
		topic = switchesRespChannel01;
		break;
	case SWITCH_BATH_2_ID:
		topic = switchesRespChannel02;
		break;
	case SWITCH_BATH_3_ID:
		topic = switchesRespChannel03;
		break;
	}
	logger.log(info, "NetworkControler::onSwitchChanged() MQTT send topic:[%s], msg: %s\r\n", topic, msg.c_str());

	mqttClient.publish(topic, 0, true, msg.c_str());
	//client.publish(topic, msg.c_str());
}
//----------------------------------------------------------------------------------------
String NetworkControler::wifiStatus(uint8_t status) {
	switch (status) {
	case WL_IDLE_STATUS:		  return "WL_IDLE_STATUS";
	case WL_NO_SSID_AVAIL:		  return "WL_NO_SSID_AVAIL";
	case WL_SCAN_COMPLETED:		  return "WL_SCAN_COMPLETED";
	case WL_CONNECTED:		  return "WL_CONNECTED";
	case WL_CONNECT_FAILED:		  return "WL_CONNECT_FAILED";
	case WL_CONNECTION_LOST:		  return "WL_CONNECTION_LOST";
	case WL_DISCONNECTED:		  return "WL_DISCONNECTED";
	default:                          return "UNKNOWN[" + String(status) + "]";
	}
}
//----------------------------------------------------------------------------------------
/*
String NetworkControler::discMqttReasons(AsyncMqttClientDisconnectReason reason) {
	switch (reason) {
	case TCP_DISCONNECTED:		  return "TCP_DISCONNECTED";
	case MQTT_UNACCEPTABLE_PROTOCOL_VERSION:			  return "MQTT_UNACCEPTABLE_PROTOCOL_VERSION";
	case MQTT_IDENTIFIER_REJECTED:			    return "MQTT_IDENTIFIER_REJECTED";
	case MQTT_SERVER_UNAVAILABLE:				    return "MQTT_SERVER_UNAVAILABLE";
	case MQTT_MALFORMED_CREDENTIALS:				      return "MQTT_MALFORMED_CREDENTIALS";
	case MQTT_NOT_AUTHORIZED:		return "MQTT_NOT_AUTHORIZED";
	case ESP8266_NOT_ENOUGH_SPACE:	return "ESP8266_NOT_ENOUGH_SPACE";
	case TLS_BAD_FINGERPRINT:		return "TLS_BAD_FINGERPRINT";
	default:                          return "UNKNOWN[" + String(reason) + "]";
	}
}
*/
NetworkControler networkControler;
