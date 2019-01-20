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

#include "NetworkControler.h"
#include "DisplayControler.h"
#include "EventsHandler.h"
#include "Configuration.h"
#include "Constants.h"

WiFiClient espClient;
PubSubClient client(espClient);

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
	// We start by connecting to a WiFi network
	Serial.printf("\r\nConnecting to %s\r\n", configuration.wifiSSID.c_str());
	String hostname = NetworkControler::getHostName();
	WiFi.begin(configuration.wifiSSID.c_str(), configuration.wifiPassword.c_str());
	WiFi.setHostname(hostname.c_str());

	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
		displayControler.showWiFiStatus(WiFi.status());
		delay(500);
		Serial.print(".");
	}
	displayControler.showWiFiStatus(WiFi.status());

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.printf("IP address: %s Hostname: %s\r\n", WiFi.localIP().toString().c_str(), hostname.c_str());

	client.setServer(configuration.mqttServer.c_str(), 1883);
	client.setCallback(NetworkControler::mqttCallback);
}
//----------------------------------------------------------------------------------------
bool NetworkControler::reconnect() {
	displayControler.showMQTTConnected(false);
	// Loop until we're reconnected
  String stateStr; 
	if (!client.connected()) {
		int state = client.state();
		//NetworkControler::statusToString(state,stateStr);
		Serial.printf("MQTT state %s [%d], attempting connection...\r\n", stateStr, state);
		// Attempt to connect
		String hostname = NetworkControler::getHostName();
		if (client.connect(hostname.c_str())) {
			displayControler.showMQTTConnected(true);
			Serial.printf("MQTT connected as %s\r\n", hostname.c_str());

			//TODO: przerobić na oczyt stanu 
			//Po ponownym podłączeniu wysyłam ostatni stan     
			//client.publish(switchesRespChannel01, "off");
			//client.publish(switchesRespChannel02, "off");

			// ... and resubscribe
			// można subskrybować wiele topików
			client.subscribe(sensorsBME280TemperatureTopic);
			client.subscribe(sensorsBME280HumidityTopic);
			client.subscribe(sensorsBME280PressureTopic);
			//client.subscribe(lightingCommandTopic);
			client.subscribe(switchesReqChannel01);
			client.subscribe(switchesReqChannel02);
			return true;
		}
		else {
      int state = client.state();
      //NetworkControler::statusToString(state,stateStr);      
			Serial.printf("MQTT reconnect failed, state %s [%d]\r\n", stateStr, state);
			return false;
		}
	}
}
//----------------------------------------------------------------------------------------
void NetworkControler::loop() {
	if (!client.connected() && !reconnect()) {
		return;
	}
	client.loop();
}
//----------------------------------------------------------------------------------------
void NetworkControler::mqttCallback(char* topic, byte* payload, unsigned int length) {

	// Conver the incoming byte array to a string
	payload[length] = '\0'; // Null terminator used to terminate the char array
	String mqttTopic = topic;
	String mqttMessage = (char*)payload;

	Serial.printf("MQTT received topic:[%s], msg: %s\r\n", topic, mqttMessage.c_str());

	if (mqttTopic.equals(switchesReqChannel01)) {
		eventsHandler.onSwitchChange(SWITCH_BATH_1_ID, mqttMessage.equals("on") ? SW_ON : SW_OFF);
	}
	else if (mqttTopic.equals(switchesReqChannel02)) {
		eventsHandler.onSwitchChange(SWITCH_BATH_2_ID, mqttMessage.equals("on") ? SW_ON : SW_OFF);
	}
	else if (mqttTopic.equals(switchesReqChannel03)) {
		eventsHandler.onSwitchChange(SWITCH_BATH_3_ID, mqttMessage.equals("on") ? SW_ON : SW_OFF);
	}
	else if (mqttTopic.equals(sensorsBME280TemperatureTopic)) {
		float outdoorTemp = mqttMessage.toFloat();
		displayControler.showOutdoorTemperature(outdoorTemp);
	}
	else if (mqttTopic.equals(sensorsBME280HumidityTopic)) {
		float outdoorHumidity = mqttMessage.toFloat();
		displayControler.showOutdoorHumidity(outdoorHumidity);
	}
	else if (mqttTopic.equals(sensorsBME280PressureTopic)) {
		float outdoorPressure = mqttMessage.toFloat();
		displayControler.showPressure(outdoorPressure);
	}
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
  Serial.printf("NetworkControler::onSwitchChanged() MQTT send topic:[%s], msg: %s\r\n", topic, msg.c_str()); 
	client.publish(topic, msg.c_str());
}
//----------------------------------------------------------------------------------------
String NetworkControler::statusToString(int status,String& statusStr) {
	;
	switch (status) {
	case MQTT_CONNECTION_TIMEOUT:		statusStr = "MQTT_CONNECTION_TIMEOUT"; break;
	case MQTT_CONNECTION_LOST:			statusStr = "MQTT_CONNECTION_LOST"; break;
	case MQTT_CONNECT_FAILED:			statusStr = "MQTT_CONNECT_FAILED"; break;
	case MQTT_DISCONNECTED:				statusStr = "MQTT_DISCONNECTED"; break;
	case MQTT_CONNECTED:				statusStr = "MQTT_CONNECTED"; break;
	case MQTT_CONNECT_BAD_PROTOCOL:		statusStr = "MQTT_CONNECT_BAD_PROTOCOL"; break;
	case MQTT_CONNECT_BAD_CLIENT_ID:	statusStr = "MQTT_CONNECT_BAD_CLIENT_ID"; break;
	case MQTT_CONNECT_UNAVAILABLE:		statusStr = "MQTT_CONNECT_UNAVAILABLE"; break;
	case MQTT_CONNECT_BAD_CREDENTIALS:	statusStr = "MQTT_CONNECT_BAD_CREDENTIALS"; break;
	case MQTT_CONNECT_UNAUTHORIZED:		statusStr = "MQTT_CONNECT_UNAUTHORIZED"; break;
	default: statusStr = "UNKNOWN"; break;
	}
}
NetworkControler networkControler;
