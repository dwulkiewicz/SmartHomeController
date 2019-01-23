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
  initWiFi();
  initMQTT();
  initOTA();
}
//----------------------------------------------------------------------------------------
void NetworkControler::initWiFi(){ 
  // We start by connecting to a WiFi network
  Serial.printf("\r\nConnecting to %s\r\n", configuration.wifiSSID.c_str());
  String hostname = NetworkControler::getHostName();
  WiFi.begin(configuration.wifiSSID.c_str(), configuration.wifiPassword.c_str());
  WiFi.setHostname(hostname.c_str());

  // Wait for connection
  //TODO: uwaga na wotchdoga, trzeba wyjść jak się nie uda połączyć i obsłużyć poprawnie brak połączenia z WiFi   
  while (WiFi.status() != WL_CONNECTED) {
    //TODO: przerobić na metodę z EventHandlera
    displayControler.showWiFiStatus(WiFi.status());
    delay(500);
    Serial.print(".");
  }
  //TODO: przerobić na metodę z EventHandlera
  displayControler.showWiFiStatus(WiFi.status());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.printf("IP address: %s Hostname: %s\r\n", WiFi.localIP().toString().c_str(), hostname.c_str());  
}
//----------------------------------------------------------------------------------------
void NetworkControler::initMQTT(){
  client.setServer(configuration.mqttServer.c_str(), 1883);
  client.setCallback(NetworkControler::mqttCallback);   
}
//----------------------------------------------------------------------------------------
void NetworkControler::initOTA(){
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
	displayControler.showMQTTConnected(false);
	// Loop until we're reconnected
  String stateStr; 
	if (!client.connected()) {
		int state = client.state();
		stateStr = NetworkControler::statusToString(state);
		Serial.printf("MQTT state %s, attempting connection...\r\n", stateStr.c_str());
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
      stateStr = NetworkControler::statusToString(state);  
			Serial.printf("MQTT reconnect failed, state %s\r\n", stateStr.c_str());
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
  ArduinoOTA.handle();  
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
    eventsHandler.onRefreshOutdoorTemperature(outdoorTemp);
	}
	else if (mqttTopic.equals(sensorsBME280HumidityTopic)) {
		float outdoorHumidity = mqttMessage.toFloat();
		eventsHandler.onRefreshOutdoorHumidity(outdoorHumidity);
	}
	else if (mqttTopic.equals(sensorsBME280PressureTopic)) {
		float outdoorPressure = mqttMessage.toFloat();
		eventsHandler.onRefreshOutdoorPressure(outdoorPressure);
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
String NetworkControler::statusToString(int status) {
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
