
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>


#include "NetworkControler.h"
#include "Constants.h"

const char* switchesReqChannel01 = "switches/req/channel01";
const char* switchesReqChannel02 = "switches/req/channel02";

const char* switchesRespChannel01 = "switches/resp/channel01";
const char* switchesRespChannel02 = "switches/resp/channel02";


WiFiClient espClient;
PubSubClient client(espClient);

//----------------------------------------------------------------------------------------

NetworkControler::NetworkControler(Configuration* configuration,DisplayControler* displayControler)
{
	this->configuration = configuration;
  this->displayControler = displayControler;
  // Set Hostname.
  char buf[15];
  uint64_t chipid = ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  sprintf(buf, "%04x%08x", (uint16_t)(chipid >> 32) /*High 2 bytes*/, (uint32_t)chipid /*Low 4bytes*/);
  hostname = HOSTNAME_PREFIX + String(buf);
}
//----------------------------------------------------------------------------------------
void NetworkControler::init() {
  // We start by connecting to a WiFi network
  Serial.printf("\r\nConnecting to %s\r\n", configuration->wifiSSID.c_str());

  WiFi.begin(configuration->wifiSSID.c_str(), configuration->wifiPassword.c_str());
  WiFi.setHostname(hostname.c_str());

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    displayControler->picWiFiStatus->setPic(PICTURE_WIFI_ON);
    delay(250);
    displayControler->picWiFiStatus->setPic(PICTURE_WIFI_OFF);
    delay(250);
    Serial.print(".");
  }
  displayControler->picWiFiStatus->setPic(PICTURE_WIFI_ON);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.printf("IP address: %s Hostname: %s\r\n", WiFi.localIP().toString().c_str(), hostname.c_str());


  client.setServer(configuration->mqttServer.c_str(), 1883);
  client.setCallback(NetworkControler::mqttCallback);
  
}
//----------------------------------------------------------------------------------------
void NetworkControler::reconnect() {
  displayControler->picWiFiStatus->setPic(PICTURE_WIFI_OFF);  
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(hostname.c_str())) {
      displayControler->picWiFiStatus->setPic(PICTURE_WIFI_ON);  
      Serial.printf("connected as %s\r\n", hostname.c_str());

      // Po ponownym podłączeniu wysyłam ostatni kolor LEDów na MQTT
//      client.publish(lightingConfirmTopic, &lastColour[0]);
      client.publish(switchesRespChannel01, "off");
      client.publish(switchesRespChannel02, "off");

      // ... and resubscribe
      // można subskrybować wiele topiców
//      client.subscribe(sensorsBME280CommandTopic);
//      client.subscribe(sensorsDS18B20CommandTopic);
//      client.subscribe(sensorsDHT11CommandTopic);
//      client.subscribe(lightingCommandTopic);
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

extern NetworkControler networkControler;

//----------------------------------------------------------------------------------------
void NetworkControler::mqttCallback(char* topic, byte* payload, unsigned int length) {

  // Conver the incoming byte array to a string
  payload[length] = '\0'; // Null terminator used to terminate the char array
  String mqttTopic = topic;
  String mqttMessage = (char*)payload;

  Serial.printf("MQTT received topic:[%s], msg: %s\r\n", topic, mqttMessage.c_str());
  
  if (mqttTopic.equals(switchesReqChannel01)) {
    networkControler.setSwitch(0, mqttMessage);
    Serial.printf("MQTT send topic:[%s], msg: %s\r\n", switchesRespChannel01, mqttMessage.c_str());
    client.publish(switchesRespChannel01, mqttMessage.c_str());
  }
  else if (mqttTopic.equals(switchesReqChannel02)) {
    networkControler.setSwitch(1, mqttMessage);
    Serial.printf("MQTT send topic:[%s], msg: %s\r\n", switchesRespChannel02, mqttMessage.c_str());
    client.publish(switchesRespChannel02, mqttMessage.c_str());
  }  
}

extern NexPicture switchBathroomMainLight;

//----------------------------------------------------------------------------------------
void NetworkControler::setSwitch(uint8_t item, String value) {
  Serial.printf("Switch: %2d state: %s\r\n", item, value.c_str());
  if (value.equals("on")) {
    switchBathroomMainLight.setPic(PICTURE_SWITCH_ON); 
  }
  else {
    switchBathroomMainLight.setPic(PICTURE_SWITCH_OFF); 
  }
}
