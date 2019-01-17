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
#include "Constants.h"

WiFiClient espClient;
PubSubClient client(espClient);

//TODO: przenieść nazwy kolejek do konfiguracji
const char* switchesReqChannel01 = "switches/req/channel01";
const char* switchesReqChannel02 = "switches/req/channel02";

const char* switchesRespChannel01 = "switches/resp/channel01";
const char* switchesRespChannel02 = "switches/resp/channel02";

const char* sensorsBME280TemperatureTopic = "sensors/bme280/temperature";
const char* sensorsBME280HumidityTopic = "sensors/bme280/humidity";
const char* sensorsBME280PressureTopic = "sensors/bme280/pressure";

extern DisplayControler displayControler;

//----------------------------------------------------------------------------------------

NetworkControler::NetworkControler(Configuration* configuration)
{
	this->configuration = configuration;
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
	Serial.printf("\r\nConnecting to %s\r\n", configuration->wifiSSID.c_str());
	String hostname = NetworkControler::getHostName();
	WiFi.begin(configuration->wifiSSID.c_str(), configuration->wifiPassword.c_str());
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


	client.setServer(configuration->mqttServer.c_str(), 1883);
	client.setCallback(NetworkControler::mqttCallback);
}
//----------------------------------------------------------------------------------------
void NetworkControler::reconnect() {
	displayControler.showMQTTConnected(false);
	// Loop until we're reconnected
	while (!client.connected()) {
		Serial.println("Attempting MQTT connection...");
		// Attempt to connect
		String hostname = NetworkControler::getHostName();
		if (client.connect(hostname.c_str())) {
			displayControler.showMQTTConnected(true);
			Serial.printf("connected as %s\r\n", hostname.c_str());

			// Po ponownym podłączeniu wysyłam ostatni kolor LEDów na MQTT
	  //      client.publish(lightingConfirmTopic, &lastColour[0]);
			client.publish(switchesRespChannel01, "off");
			client.publish(switchesRespChannel02, "off");

			// ... and resubscribe
			// można subskrybować wiele topiców
			client.subscribe(sensorsBME280TemperatureTopic);
			client.subscribe(sensorsBME280HumidityTopic);
			client.subscribe(sensorsBME280PressureTopic);
			//client.subscribe(lightingCommandTopic);

			client.subscribe(switchesReqChannel01);
			client.subscribe(switchesReqChannel02);
		}
		else {
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 5 seconds");

			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}
//----------------------------------------------------------------------------------------
void NetworkControler::loop() {
	if (!client.connected()) {
		reconnect();
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
		displayControler.setSwitch(0, mqttMessage);
		Serial.printf("MQTT send topic:[%s], msg: %s\r\n", switchesRespChannel01, mqttMessage.c_str());
		client.publish(switchesRespChannel01, mqttMessage.c_str());
	}
	else if (mqttTopic.equals(switchesReqChannel02)) {
		displayControler.setSwitch(1, mqttMessage);
		Serial.printf("MQTT send topic:[%s], msg: %s\r\n", switchesRespChannel02, mqttMessage.c_str());
		client.publish(switchesRespChannel02, mqttMessage.c_str());
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
