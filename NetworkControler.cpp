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
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>
#include "NetworkControler.h"
#include "DisplayControler.h"
#include "EventDispatcher.h"
#include "Configuration.h"
#include "Constants.h"

WiFiClient espClient;
PubSubClient client(espClient);

//TODO: przenieœæ nazwy kolejek do konfiguracji
const char* sensorsBME280TemperatureTopic = "sensors/bme280/temperature";
const char* sensorsBME280HumidityTopic = "sensors/bme280/humidity";
const char* sensorsBME280PressureTopic = "sensors/bme280/pressure";

//const char* sonoffTele = "tele/#";
const char* sonoffStat = "stat/#";

const char* sonoffCmndBathroom01 = "cmnd/lights/bathroom/POWER1";
const char* sonoffStatBathroom01 = "stat/lights/bathroom/POWER1";
const char* sonoffCmndBathroom02 = "cmnd/lights/bathroom/POWER2";
const char* sonoffStatBathroom02 = "stat/lights/bathroom/POWER2";
const char* sonoffCmndBathroom03 = "cmnd/lights/bathroom/POWER3";
const char* sonoffStatBathroom03 = "stat/lights/bathroom/POWER3";
const char* sonoffCmndKitchen01 = "cmnd/lights/kitchen/POWER1";
const char* sonoffStatKitchen01 = "stat/lights/kitchen/POWER1";
const char* sonoffCmndKitchen02 = "cmnd/lights/kitchen/POWER2";
const char* sonoffStatKitchen02 = "stat/lights/kitchen/POWER2";
const char* switchesCmndSwitch01 = "cmnd/switch01/POWER";
const char* switchesStatSwitch01 = "stat/switch01/POWER";

#define MQTT_PAYLOAD_BUF_SIZE 200
//char payload_buf[MQTT_PAYLOAD_BUF_SIZE];

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
	initWiFi();
	initMQTT();
	initOTA();
}
//----------------------------------------------------------------------------------------
void NetworkControler::initWiFi() {
	// We start by connecting to a WiFi network
	logger.log(info, "\r\nConnecting to %s\r\n", configuration.wifiSSID.c_str());
	String hostname = NetworkControler::getHostName();
	WiFi.begin(configuration.wifiSSID.c_str(), configuration.wifiPassword.c_str());
	WiFi.setHostname(hostname.c_str());

	// Wait for connection
	//TODO: uwaga na wotchdoga, trzeba wyjœæ jak siê nie uda po³¹czyæ i obs³u¿yæ poprawnie brak po³¹czenia z WiFi, do przerobienia po³¹czenie do sieci WiFi   
	while (WiFi.status() != WL_CONNECTED) {
		eventDispatcher.onWiFiStatusChange(WiFi.status());
		delay(500);
		Serial.print(".");
	}
	eventDispatcher.onWiFiStatusChange(WiFi.status());
	logger.log(info, "WiFi connected: IP: %s Host: %s\r\n", WiFi.localIP().toString().c_str(), hostname.c_str());
}
//----------------------------------------------------------------------------------------
void NetworkControler::initMQTT() {
	client.setServer(configuration.mqttServer.c_str(), configuration.mqttPort);
	client.setCallback(NetworkControler::mqttCallback);
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
bool NetworkControler::reconnect() {
	logger.log(info, "NetworkControler::reconnect() enter\r\n");
	if (!client.connected()) {
		logger.log(info, "MQTT state: %s, attempting connection...\r\n", NetworkControler::statusMqttToString(client.state()).c_str());
		eventDispatcher.onMQTTStatusChange(client.state());
		// Attempt to connect
		String hostname = NetworkControler::getHostName();
		if (client.connect(hostname.c_str())) {
			logger.log(info, "MQTT connected as %s\r\n", hostname.c_str());
			eventDispatcher.onMQTTStatusChange(client.state());
			client.subscribe(sensorsBME280TemperatureTopic);
			client.subscribe(sensorsBME280HumidityTopic);
			client.subscribe(sensorsBME280PressureTopic);
						
			//client.subscribe(sonoffTele);
			//client.subscribe(sonoffStat);

			//client.subscribe(sonoffStatBathroom01);
			//client.subscribe(sonoffStatBathroom02);
			//client.subscribe(switchesStatSwitch01);
			return true;
		}
		else {
			logger.log(warning, "MQTT reconnect failed, state: %s\r\n", NetworkControler::statusMqttToString(client.state()).c_str());
			eventDispatcher.onMQTTStatusChange(client.state());
			return false;
		}
	}
}
//----------------------------------------------------------------------------------------
void NetworkControler::loop() {
	if (!client.connected() && !reconnect()) {
		return;
	}
	//Handle OTA server.
	//ArduinoOTA.handle();

	client.loop();

	//mqttCallbackLoop();
}

typedef struct TaskParam
{
	char topic[MQTT_PAYLOAD_BUF_SIZE];
	char msg[MQTT_PAYLOAD_BUF_SIZE];
} TaskParam;

QueueHandle_t mqttQueue = xQueueCreate(5, sizeof(TaskParam));

//-----------------------------------------------------------------------------------------
void NetworkControler::mqttCallbackLoop() {
	while (uxQueueMessagesWaiting(mqttQueue) > 0){
		TaskParam p;
		xQueueReceive(mqttQueue, &p, 0);

		String topic = String((char*)p.topic);
		String msg = String((char*)p.msg);

		logger.log(debug, "mqttCallbackLoop() topic: %s, msg: %s\r\n", topic.c_str(), msg.c_str());

		if (topic.equals(sonoffStatBathroom01)) {
			eventDispatcher.onSwitchChange(EVENT_SRC_MQTT, SW_BATHROOM_CH1, msg.equals("ON") ? SW_STATE_ON : SW_STATE_OFF);
		}
		else if (topic.equals(sonoffStatBathroom02)) {
			eventDispatcher.onSwitchChange(EVENT_SRC_MQTT, SW_BATHROOM_CH2, msg.equals("ON") ? SW_STATE_ON : SW_STATE_OFF);
		}
		else if (topic.equals(sonoffCmndBathroom03)) {
			eventDispatcher.onSwitchChange(EVENT_SRC_MQTT, SW_BATHROOM_CH3, msg.equals("ON") ? SW_STATE_ON : SW_STATE_OFF);
		}
		else if (topic.equals(sonoffStatKitchen01)) {
			eventDispatcher.onSwitchChange(EVENT_SRC_MQTT, SW_KITCHEN_CH1, msg.equals("ON") ? SW_STATE_ON : SW_STATE_OFF);
		}
		else if (topic.equals(sonoffStatKitchen02)) {
			eventDispatcher.onSwitchChange(EVENT_SRC_MQTT, SW_KITCHEN_CH2, msg.equals("ON") ? SW_STATE_ON : SW_STATE_OFF);
		}
		else if (topic.equals(switchesStatSwitch01)) {
			eventDispatcher.onSwitchChange(EVENT_SRC_MQTT, SW_SOCKET_01, msg.equals("ON") ? SW_STATE_ON : SW_STATE_OFF);
		}
		else if (topic.equals(sensorsBME280TemperatureTopic)) {
			float outdoorTemp = msg.toFloat();
			eventDispatcher.onRefreshOutdoorTemperature(outdoorTemp);
		}
		else if (topic.equals(sensorsBME280HumidityTopic)) {
			float outdoorHumidity = msg.toFloat();
			eventDispatcher.onRefreshOutdoorHumidity(outdoorHumidity);
		}
		else if (topic.equals(sensorsBME280PressureTopic)) {
			float outdoorPressure = msg.toFloat();
			eventDispatcher.onRefreshOutdoorPressure(outdoorPressure);
		}
		logger.log(debug, "mqttCallbackLoop() leave\r\n");
	}
}

//----------------------------------------------------------------------------------------
void NetworkControler::mqttCallback(char* topic, byte* payload, unsigned int length) {	

	logger.log(info, "NetworkControler::mqttCallback() topic: %s, msg: %s\r\n", topic, payload);

	return;

	TaskParam p;
	for (uint8_t i = 0; i < MQTT_PAYLOAD_BUF_SIZE; i++) {		
		if (i < MQTT_PAYLOAD_BUF_SIZE - 1)
			p.topic[i] = topic[i];
		else
			p.topic[i] = '\0';		
		if (topic[i] == '\0')
			break;
	}
	memcpy(p.msg, payload, MIN(length, MQTT_PAYLOAD_BUF_SIZE));
	p.msg[MIN(length, MQTT_PAYLOAD_BUF_SIZE)] = '\0'; // Null terminator used to terminate the char array

	logger.log(info, "NetworkControler::mqttCallback() topic: %s, msg: %s\r\n", p.topic, p.msg);
	//if (xQueueSend(mqttQueue, &p, 0) != pdTRUE)
	//	logger.log(info, "NetworkControler::mqttCallback() xQueueSend error\r\n");
}
//----------------------------------------------------------------------------------------
void NetworkControler::onSwitchChange(uint8_t src, uint8_t switchId, uint8_t switchState) {
	if (src == EVENT_SRC_SCREEN) {
		String topic;
		String msg = (switchState == SW_STATE_ON ? "ON" : "OFF");
		switch (switchId) {
		case SW_BATHROOM_CH1:
			topic = sonoffCmndBathroom01;
			break;
		case SW_BATHROOM_CH2:
			topic = sonoffCmndBathroom02;
			break;
		case SW_BATHROOM_CH3:
			topic = sonoffStatBathroom03;
			break;
		case SW_KITCHEN_CH1:
			topic = sonoffCmndKitchen01;
			break;
		case SW_KITCHEN_CH2:
			topic = sonoffCmndKitchen02;
			break;
		case SW_SOCKET_01:
			topic = switchesCmndSwitch01;
			break;
		default:
			return;
		}
		bool res = client.publish(topic.c_str(), msg.c_str());
		vTaskDelay(pdMS_TO_TICKS(5));//szukanie rozwi¹zanania problemu
		logger.log(info, "NetworkControler::onSwitchChange() MQTT send topic:[%s], msg: %s res: %d\r\n", topic.c_str(), msg.c_str(), res);
	}
}
//----------------------------------------------------------------------------------------
String NetworkControler::statusMqttToString(int status) {
	switch (status) {
	case MQTT_CONNECTION_TIMEOUT:		  return "MQTT_CONNECTION_TIMEOUT";
	case MQTT_CONNECTION_LOST:			  return "MQTT_CONNECTION_LOST";
	case MQTT_CONNECT_FAILED:			    return "MQTT_CONNECT_FAILED";
	case MQTT_DISCONNECTED:				    return "MQTT_DISCONNECTED";
	case MQTT_CONNECTED:				      return "MQTT_CONNECTED";
	case MQTT_CONNECT_BAD_PROTOCOL:		return "MQTT_CONNECT_BAD_PROTOCOL";
	case MQTT_CONNECT_BAD_CLIENT_ID:	return "MQTT_CONNECT_BAD_CLIENT_ID";
	case MQTT_CONNECT_UNAVAILABLE:		return "MQTT_CONNECT_UNAVAILABLE";
	case MQTT_CONNECT_BAD_CREDENTIALS:return "MQTT_CONNECT_BAD_CREDENTIALS";
	case MQTT_CONNECT_UNAUTHORIZED:		return "MQTT_CONNECT_UNAUTHORIZED";
	default:                          return "UNKNOWN[" + String(status) + "]";
	}
}
NetworkControler networkControler;
