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

const char* sonoffCmndBathroom01 = "cmnd/lights/bathroom/POWER1";
const char* sonoffStatBathroom01 = "stat/lights/bathroom/POWER1";

const char* sonoffCmndBathroom02 = "cmnd/lights/bathroom/POWER2";
const char* sonoffStatBathroom02 = "stat/lights/bathroom/POWER2";

const char* switchesCmndSwitch01 = "cmnd/switch01/POWER";
const char* switchesStatSwitch01 = "stat/switch01/POWER";

#define MQTT_PAYLOAD_BUF_SIZE 100
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
			client.subscribe(sonoffStatBathroom01);
			client.subscribe(sonoffStatBathroom02);
			client.subscribe(switchesStatSwitch01);
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
	ArduinoOTA.handle();

	client.loop();
}

SemaphoreHandle_t xMutex = xSemaphoreCreateMutex();
typedef struct TaskParam
{
	String mqttTopic;
	String mqttMessage;
} TaskParam;

//-----------------------------------------------------------------------------------------
void TaskMqttCallback(void * pvParameters) {

	TaskParam* p = (TaskParam*)pvParameters;

	logger.log(debug, "TaskMqttCallback() running on core %d\r\n", xPortGetCoreID());
	xSemaphoreTake(xMutex, portMAX_DELAY);
	logger.log(debug, "TaskMqttCallback() enter\r\n");

	if (p->mqttTopic.equals(sonoffStatBathroom01)) {
		eventDispatcher.onMqttReceiveSwitchState(SWITCH_BATH_1_IDX, p->mqttMessage.equals("ON") ? SW_ON : SW_OFF);
	}
	else if (p->mqttTopic.equals(sonoffStatBathroom02)) {
		eventDispatcher.onMqttReceiveSwitchState(SWITCH_BATH_2_IDX, p->mqttMessage.equals("ON") ? SW_ON : SW_OFF);
	}
	else if (p->mqttTopic.equals(switchesStatSwitch01)) {
		eventDispatcher.onMqttReceiveSwitchState(SWITCH_SONOFF_1_IDX, p->mqttMessage.equals("ON") ? SW_ON : SW_OFF);
	}
	else if (p->mqttTopic.equals(sensorsBME280TemperatureTopic)) {
		float outdoorTemp = p->mqttMessage.toFloat();
		eventDispatcher.onRefreshOutdoorTemperature(outdoorTemp);
	}
	else if (p->mqttTopic.equals(sensorsBME280HumidityTopic)) {
		float outdoorHumidity = p->mqttMessage.toFloat();
		eventDispatcher.onRefreshOutdoorHumidity(outdoorHumidity);
	}
	else if (p->mqttTopic.equals(sensorsBME280PressureTopic)) {
		float outdoorPressure = p->mqttMessage.toFloat();
		eventDispatcher.onRefreshOutdoorPressure(outdoorPressure);
	}

	logger.log(debug, "TaskMqttCallback() leave\r\n");
	delete p;
	xSemaphoreGive(xMutex);
	vTaskDelete(0);
}

//----------------------------------------------------------------------------------------
void NetworkControler::mqttCallback(char* topic, byte* payload, unsigned int length) {
	
//	memcpy(payload_buf, payload, MIN(length, MQTT_PAYLOAD_BUF_SIZE));
//	payload_buf[MIN(length, MQTT_PAYLOAD_BUF_SIZE)] = '\0'; // Null terminator used to terminate the char array

	// Conver the incoming byte array to a string
	payload[length] = '\0'; // Null terminator used to terminate the char array
	//String mqttTopic = topic;
	//String mqttMessage = (char*)payload;

	TaskParam* p = new TaskParam();
	p->mqttTopic = topic;
	p->mqttMessage = (char*)payload;

	logger.log(info, "NetworkControler::mqttCallback() topic: %s, msg: %s\r\n", p->mqttTopic.c_str(), p->mqttMessage.c_str());

	xTaskCreatePinnedToCore(TaskMqttCallback, "TaskMqttCallback", 4096, (void*)p, 3, NULL, CORE_2);

/*
	if (mqttTopic.equals(switchesReqChannel01)) {
		eventDispatcher.onMqttReceiveSwitchState(SWITCH_BATH_1_IDX, mqttMessage.equals("on") ? SW_ON : SW_OFF);
	}
	else if (mqttTopic.equals(switchesReqChannel02)) {
		eventDispatcher.onMqttReceiveSwitchState(SWITCH_BATH_2_IDX, mqttMessage.equals("on") ? SW_ON : SW_OFF);
	}
	else if (mqttTopic.equals(switchesReqChannel03)) {
		eventDispatcher.onMqttReceiveSwitchState(SWITCH_BATH_3_IDX, mqttMessage.equals("on") ? SW_ON : SW_OFF);
	}
	else if (mqttTopic.equals(switchesStatSwitch01)) {
		eventDispatcher.onMqttReceiveSwitchState(SWITCH_SONOFF_1_IDX, mqttMessage.equals("ON") ? SW_ON : SW_OFF);
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
*/
	logger.log(info, "NetworkControler::mqttCallback() end\r\n");

}

//-----------------------------------------------------------------------------------------
/*
void TaskMqttPublish(void * pvParameters) {

	TaskParam* p = (TaskParam*)pvParameters;

	logger.log(debug, "TaskMqttPublish() running on core %d\r\n", xPortGetCoreID());
	//xSemaphoreTake(xMutex, portMAX_DELAY);
	logger.log(debug, "TaskMqttPublish() enter\r\n");

	bool res = client.publish(p->mqttTopic.c_str(), p->mqttMessage.c_str());
	vTaskDelay(pdMS_TO_TICKS(5));//szukanie rozwi¹zanania problemu
	
	logger.log(info, "TaskMqttPublish() MQTT send topic:[%s], msg: %s res: %d\r\n", p->mqttTopic.c_str(), p->mqttMessage.c_str(), res);


	logger.log(debug, "TaskMqttPublish() leave\r\n");
	delete p;
	//xSemaphoreGive(xMutex);
	vTaskDelete(0);
}
*/
//----------------------------------------------------------------------------------------
void NetworkControler::publishCmdSwitchChange(uint8_t switchId, uint8_t switchState) {

	TaskParam p;
	p.mqttMessage = (switchState == SW_ON ? "ON" : "OFF");
	switch (switchId) {
	case SWITCH_BATH_1_IDX:
		p.mqttTopic = sonoffCmndBathroom01;
		break;
	case SWITCH_BATH_2_IDX:
		p.mqttTopic = sonoffCmndBathroom02;
		break;
	case SWITCH_SONOFF_1_IDX:
		p.mqttTopic = switchesCmndSwitch01;
		break;
	}
	bool res = client.publish(p.mqttTopic.c_str(), p.mqttMessage.c_str());
	//vTaskDelay(pdMS_TO_TICKS(5));//szukanie rozwi¹zanania problemu
	logger.log(info, "NetworkControler::publishCmdSwitchChange() MQTT send topic:[%s], msg: %s res: %d\r\n", p.mqttTopic.c_str(), p.mqttMessage.c_str(),res);
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
