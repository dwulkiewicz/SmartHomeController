/************************************************************************/
/*                                                                      */
/*              Klasa obsluguj�ca konfiguracj�		                    */
/*              Hardware: ESP32 (Heltec Wifi-Lora-32)                   */
/*                                                                      */
/*              Author: Dariusz Wulkiewicz                              */
/*                      d.wulkiewicz@gmail.com                          */
/*                                                                      */
/*              Date: 11.2018                                           */
/************************************************************************/

#include "Configuration.h"
#include <EEPROM.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>

/*Temperatura */
#define EEPROM_SIZE 64
#define EEPROM_ADDR_DAY_TEMP        0x00
#define EEPROM_ADDR_NIGHT_TEMP      0x01
#define EEPROM_ADDR_HYSTERESIS_TEMP 0x02

#define OFFSET_TEMP  100  //10.0*C


/* You only need to format SPIFFS the first time you run a
test or else use the SPIFFS plugin to create a partition
https://github.com/me-no-dev/arduino-esp32fs-plugin */
#define FORMAT_SPIFFS_IF_FAILED true

void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
	Serial.printf("Listing directory: %s\r\n", dirname);

	File root = fs.open(dirname);
	if (!root) {
		Serial.println("- failed to open directory");
		return;
	}
	if (!root.isDirectory()) {
		Serial.println(" - not a directory");
		return;
	}

	File file = root.openNextFile();
	while (file) {
		if (file.isDirectory()) {
			Serial.print("  DIR : ");
			Serial.println(file.name());
			if (levels) {
				listDir(fs, file.name(), levels - 1);
			}
		}
		else {
			Serial.print("  FILE: ");
			Serial.print(file.name());
			Serial.print("\tSIZE: ");
			Serial.println(file.size());
		}
		file = root.openNextFile();
	}
}

Configuration::Configuration()
{
	dayTemp = 130; //100 + 130 -> 23.0*C
	nightTemp = 80; //100 + 80 -> 18.0*C
	hysteresisTemp = 10; //1*C
}

bool Configuration::init() {
  Serial.printf("Configuration ******************\r\n");
	//listDir(SPIFFS, "/", 0);

	if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
		Serial.println("SPIFFS Mount Failed");
		return false;
	}

	File configFile = SPIFFS.open("/config.json", "r");
	if (!configFile) {
		Serial.println("Configuration::init() Failed to open config file");
		return false;
	}
	else {
		Serial.println("config file opened");
	}


	size_t size = configFile.size();
	if (size > 1024) {
		Serial.println("Config file size is too large");
		return false;
	}
	
	// Allocate a buffer to store contents of the file.
	std::unique_ptr<char[]> buf(new char[size]);

	// We don't use String here because ArduinoJson library requires the input
	// buffer to be mutable. If you don't use ArduinoJson, you may as well
	// use configFile.readString instead.
	configFile.readBytes(buf.get(), size);

	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& json = jsonBuffer.parseObject(buf.get());

	if (!json.success()) {
		Serial.println("Failed to parse config file");
    Serial.println("Set default configuration..");	
    //TODO: wpisać 
	}
 else{
    wifiSSID = json["ssid"].asString();
    wifiPassword = json["password"].asString();
    mqttServer = json["mqtt_server"].asString();  
 }


	// Real world application would store these values in some variables for
	// later use.

	if (!EEPROM.begin(EEPROM_SIZE))
	{
		Serial.println("failed to initialise EEPROM");
		return false;
	}
	dayTemp = EEPROM.read(EEPROM_ADDR_DAY_TEMP);
	nightTemp = EEPROM.read(EEPROM_ADDR_NIGHT_TEMP);
	hysteresisTemp = EEPROM.read(EEPROM_ADDR_HYSTERESIS_TEMP);

  print();
  Serial.printf("End Configuration ******************\r\n");
	return true;
}

float Configuration::getDayTemperature() {
	return (dayTemp + OFFSET_TEMP) / 10.0;
}

String Configuration::temperatureAsString(float temp) {
	char buf[10];
	return dtostrf(temp, 4, 1 /*2*/, buf);
}

bool Configuration::incrementDayTemperature() {
	if (dayTemp <= 250) {
		dayTemp += 5;
		EEPROM.write(EEPROM_ADDR_DAY_TEMP, dayTemp);
		EEPROM.commit();
		return true;
	}
	return false;
}

bool Configuration::decrementDayTemperature() {
	if (dayTemp >= 5) {
		dayTemp -= 5;
		EEPROM.write(EEPROM_ADDR_DAY_TEMP, dayTemp);
		EEPROM.commit();
		return true;
	}
	return false;
}

float Configuration::getNightTemperature() {
	return (nightTemp + OFFSET_TEMP) / 10.0;
}

bool Configuration::incrementNightTemperature() {
	if (nightTemp <= 250) {
		nightTemp += 5;
		EEPROM.write(EEPROM_ADDR_NIGHT_TEMP, nightTemp);
		EEPROM.commit();
		return true;
	}
	return false;
}

bool Configuration::decrementNightTemperature() {
	if (nightTemp >= 5) {
		nightTemp -= 5;
		EEPROM.write(EEPROM_ADDR_NIGHT_TEMP, nightTemp);
		EEPROM.commit();
		return true;
	}
	return false;
}

float Configuration::getHisteresisTemp() {
	return (hysteresisTemp) / 10.0;
}

bool Configuration::incrementHisteresisTemperature() {
	//max histereza 2.0*C (+/-1.0*C)  
	if (hysteresisTemp <= 18) {
		hysteresisTemp += 2;
		EEPROM.write(EEPROM_ADDR_HYSTERESIS_TEMP, hysteresisTemp);
		EEPROM.commit();
		return true;
	}
	return false;
}

bool Configuration::decrementHisteresisTemperature() {
	//min histereza 0.4*C (+/-0.2*C)
	if (hysteresisTemp >= 6) {
		hysteresisTemp -= 2;
		EEPROM.write(EEPROM_ADDR_HYSTERESIS_TEMP, hysteresisTemp);
		EEPROM.commit();
		return true;
	}
	return false;
}

void Configuration::print(){ 
  Serial.printf("wifiSSID: %s\r\n", wifiSSID.c_str());
  Serial.printf("wifiPassword: %s\r\n", wifiPassword.c_str());
  Serial.printf("mqttServer: %s\r\n", mqttServer.c_str());
  Serial.printf("dayTemp: %.1f\r\n", getDayTemperature());
  Serial.printf("nightTemp: %.1f\r\n", getNightTemperature());
  Serial.printf("hysteresisTemp: %.1f\r\n", getHisteresisTemp());
}
