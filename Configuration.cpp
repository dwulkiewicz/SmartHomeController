/************************************************************************/
/*                                                                      */
/*              Klasa obslugujďż˝ca konfiguracjďż˝		                    */
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

#define EEPROM_ADDR_HEATING_WORKING_DAYS_MORNING_ON_H     0x03 
#define EEPROM_ADDR_HEATING_WORKING_DAYS_MORNING_ON_M     0x04
#define EEPROM_ADDR_HEATING_WORKING_DAYS_MORNING_OFF_H    0x05
#define EEPROM_ADDR_HEATING_WORKING_DAYS_MORNING_OFF_M    0x06
#define EEPROM_ADDR_HEATING_WORKING_DAYS_AFTERNOON_ON_H   0x07
#define EEPROM_ADDR_HEATING_WORKING_DAYS_AFTERNOON_ON_M   0x08
#define EEPROM_ADDR_HEATING_WORKING_DAYS_AFTERNOON_OFF_H  0x09
#define EEPROM_ADDR_HEATING_WORKING_DAYS_AFTERNOON_OFF_M  0x0A
#define EEPROM_ADDR_HEATING_WEEKEND_MORNING_ON_H          0x0B
#define EEPROM_ADDR_HEATING_WEEKEND_MORNING_ON_M          0x0C
#define EEPROM_ADDR_HEATING_WEEKEND_MORNING_OFF_H         0x0D
#define EEPROM_ADDR_HEATING_WEEKEND_MORNING_OFF_M         0x0E
#define EEPROM_ADDR_HEATING_WEEKEND_AFTERNOON_ON_H        0x0F
#define EEPROM_ADDR_HEATING_WEEKEND_AFTERNOON_ON_M        0x10
#define EEPROM_ADDR_HEATING_WEEKEND_AFTERNOON_OFF_H       0x11
#define EEPROM_ADDR_HEATING_WEEKEND_AFTERNOON_OFF_M       0x12

#define EEPROM_ADDR_PARAMS_OFFSET   0x30

#define OFFSET_TEMP  100  //10.0*C


/* You only need to format SPIFFS the first time you run a
test or else use the SPIFFS plugin to create a partition
https://github.com/me-no-dev/arduino-esp32fs-plugin */
#define FORMAT_SPIFFS_IF_FAILED true
//-----------------------------------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------------------------------
Configuration::Configuration(){
	dayTemp = 130; //100 + 130 -> 23.0*C
	nightTemp = 80; //100 + 80 -> 18.0*C
	hysteresisTemp = 10; //1*C
}
//-----------------------------------------------------------------------------------------------------
bool Configuration::init() {
	Serial.printf("Configuration ******************\r\n");

	if(!loadFromJSON())
		return false;

	if (!loadFromEEPROM())
		return false;

	print();
	Serial.printf("End Configuration ******************\r\n");
	return true;
}
//-----------------------------------------------------------------------------------------------------
bool Configuration::loadFromJSON() {
	//listDir(SPIFFS, "/", 0);

	if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
		Serial.println("SPIFFS Mount Failed");
		return false;
	}

	File file = SPIFFS.open("/config.json", "r");
	if (!file) {
		Serial.println("Configuration::init() Failed to open config file");
		return false;
	}
	else {
		Serial.println("config file opened");
	}
	StaticJsonDocument<512> doc;
	DeserializationError error = deserializeJson(doc, file);
	if (error)
		Serial.println(F("Failed to read file, using default configuration"));
	else {
		JsonObject root = doc.as<JsonObject>();
		wifiSSID = root["ssid"].as<String>();
		wifiPassword = root["password"].as<String>();
		mqttServer = root["mqtt_server"].as<String>();
	}
	file.close();
	return true;
}
//-----------------------------------------------------------------------------------------------------
bool Configuration::loadFromEEPROM() {
	if (!EEPROM.begin(EEPROM_SIZE)) {
		Serial.println("failed to initialise EEPROM");
		return false;
	}
	dayTemp = EEPROM.read(EEPROM_ADDR_DAY_TEMP);
	nightTemp = EEPROM.read(EEPROM_ADDR_NIGHT_TEMP);
	hysteresisTemp = EEPROM.read(EEPROM_ADDR_HYSTERESIS_TEMP);
	heatingTimes[HEATING_WORKING_DAYS_MORNING_ON].hour = EEPROM.read(EEPROM_ADDR_HEATING_WORKING_DAYS_MORNING_ON_H);
	heatingTimes[HEATING_WORKING_DAYS_MORNING_ON].minute = EEPROM.read(EEPROM_ADDR_HEATING_WORKING_DAYS_MORNING_ON_M);
	heatingTimes[HEATING_WORKING_DAYS_MORNING_OFF].hour = EEPROM.read(EEPROM_ADDR_HEATING_WORKING_DAYS_MORNING_OFF_H);
	heatingTimes[HEATING_WORKING_DAYS_MORNING_OFF].minute = EEPROM.read(EEPROM_ADDR_HEATING_WORKING_DAYS_MORNING_OFF_M);
	heatingTimes[HEATING_WORKING_DAYS_AFTERNOON_ON].hour = EEPROM.read(EEPROM_ADDR_HEATING_WORKING_DAYS_AFTERNOON_ON_H);
	heatingTimes[HEATING_WORKING_DAYS_AFTERNOON_ON].minute = EEPROM.read(EEPROM_ADDR_HEATING_WORKING_DAYS_AFTERNOON_ON_M);
	heatingTimes[HEATING_WORKING_DAYS_AFTERNOON_OFF].hour = EEPROM.read(EEPROM_ADDR_HEATING_WORKING_DAYS_AFTERNOON_OFF_H);
	heatingTimes[HEATING_WORKING_DAYS_AFTERNOON_OFF].minute = EEPROM.read(EEPROM_ADDR_HEATING_WORKING_DAYS_AFTERNOON_OFF_M);
	heatingTimes[HEATING_WEEKEND_MORNING_ON].hour = EEPROM.read(EEPROM_ADDR_HEATING_WEEKEND_MORNING_ON_H);
	heatingTimes[HEATING_WEEKEND_MORNING_ON].minute = EEPROM.read(EEPROM_ADDR_HEATING_WEEKEND_MORNING_ON_M);
	heatingTimes[HEATING_WEEKEND_MORNING_OFF].hour = EEPROM.read(EEPROM_ADDR_HEATING_WEEKEND_MORNING_OFF_H);
	heatingTimes[HEATING_WEEKEND_MORNING_OFF].minute = EEPROM.read(EEPROM_ADDR_HEATING_WEEKEND_MORNING_OFF_M);
	heatingTimes[HEATING_WEEKEND_AFTERNOON_ON].hour = EEPROM.read(EEPROM_ADDR_HEATING_WEEKEND_AFTERNOON_ON_H);
	heatingTimes[HEATING_WEEKEND_AFTERNOON_ON].minute = EEPROM.read(EEPROM_ADDR_HEATING_WEEKEND_AFTERNOON_ON_M);
	heatingTimes[HEATING_WEEKEND_AFTERNOON_OFF].hour = EEPROM.read(EEPROM_ADDR_HEATING_WEEKEND_AFTERNOON_OFF_H);
	heatingTimes[HEATING_WEEKEND_AFTERNOON_OFF].minute = EEPROM.read(EEPROM_ADDR_HEATING_WEEKEND_AFTERNOON_OFF_M);
	return true;
}
//-----------------------------------------------------------------------------------------------------
float Configuration::getDayTemperature() {
	return (dayTemp + OFFSET_TEMP) / 10.0;
}
//-----------------------------------------------------------------------------------------------------
String Configuration::temperatureAsString(float temp) {
	char buf[10];
	return dtostrf(temp, 4, 1 /*2*/, buf);
}
//-----------------------------------------------------------------------------------------------------
bool Configuration::incrementDayTemperature() {
	if (dayTemp <= 250) {
		dayTemp += 5;
		EEPROM.write(EEPROM_ADDR_DAY_TEMP, dayTemp);
		EEPROM.commit();
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------------------------------------
bool Configuration::decrementDayTemperature() {
	if (dayTemp >= 5) {
		dayTemp -= 5;
		EEPROM.write(EEPROM_ADDR_DAY_TEMP, dayTemp);
		EEPROM.commit();
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------------------------------------
float Configuration::getNightTemperature() {
	return (nightTemp + OFFSET_TEMP) / 10.0;
}
//-----------------------------------------------------------------------------------------------------
bool Configuration::incrementNightTemperature() {
	if (nightTemp <= 250) {
		nightTemp += 5;
		EEPROM.write(EEPROM_ADDR_NIGHT_TEMP, nightTemp);
		EEPROM.commit();
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------------------------------------
bool Configuration::decrementNightTemperature() {
	if (nightTemp >= 5) {
		nightTemp -= 5;
		EEPROM.write(EEPROM_ADDR_NIGHT_TEMP, nightTemp);
		EEPROM.commit();
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------------------------------------
float Configuration::getHisteresisTemp() {
	return (hysteresisTemp) / 10.0;
}
//-----------------------------------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------------------------------
bool Configuration::decrementHisteresisTemperature() {
	//min histereza 0.4*C (+/-0.2*C)
	if (hysteresisTemp >= 6) {
		if((hysteresisTemp % 2) == 0)
			hysteresisTemp -= 2;
		else
			hysteresisTemp -= 1;
		EEPROM.write(EEPROM_ADDR_HYSTERESIS_TEMP, hysteresisTemp);
		EEPROM.commit();
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------------------------------------
TShortTime Configuration::getHeatingTime(uint8_t idx) {
	return heatingTimes[idx];
}
//-----------------------------------------------------------------------------------------------------
bool Configuration::incrementHeatingTime(uint8_t idx) {
	if (heatingTimes[idx].minute >= 0 && heatingTimes[idx].minute < 15)
		heatingTimes[idx].minute = 15;
	else if (heatingTimes[idx].minute >= 15 && heatingTimes[idx].minute < 30)
		heatingTimes[idx].minute = 30;
	else if (heatingTimes[idx].minute >= 30 && heatingTimes[idx].minute < 45)
		heatingTimes[idx].minute = 45;
	else if (heatingTimes[idx].minute >= 45) {
		heatingTimes[idx].minute = 0;
		if (heatingTimes[idx].hour < 23)
			heatingTimes[idx].hour++;
		else
			heatingTimes[idx].hour = 0;
	}
	saveHeatingTime(idx);
	return true;
}
//-----------------------------------------------------------------------------------------------------
bool Configuration::decrementHeatingTime(uint8_t idx) {
	if (heatingTimes[idx].minute == 0) {
		heatingTimes[idx].minute = 45;
		if (heatingTimes[idx].hour > 0)
			heatingTimes[idx].hour--;
		else
			heatingTimes[idx].hour = 23;
	}
	else if (heatingTimes[idx].minute > 0 && heatingTimes[idx].minute <= 15)
		heatingTimes[idx].minute = 0;
	else if (heatingTimes[idx].minute > 15 && heatingTimes[idx].minute <= 30)
		heatingTimes[idx].minute = 15;
	else if (heatingTimes[idx].minute > 30 && heatingTimes[idx].minute <= 45)
		heatingTimes[idx].minute = 30;
	else if (heatingTimes[idx].minute > 45)
		heatingTimes[idx].minute = 45;
	saveHeatingTime(idx);
	return true;
}
//-----------------------------------------------------------------------------------------------------
void Configuration::saveHeatingTime(uint8_t idx) {
	//TODO: jakoś to elegancko zrobić
	switch (idx) {
	case HEATING_WORKING_DAYS_MORNING_ON:
		EEPROM.write(EEPROM_ADDR_HEATING_WORKING_DAYS_MORNING_ON_H, heatingTimes[idx].hour);
		EEPROM.write(EEPROM_ADDR_HEATING_WORKING_DAYS_MORNING_ON_M, heatingTimes[idx].minute);
		break;
	case HEATING_WORKING_DAYS_MORNING_OFF:
		EEPROM.write(EEPROM_ADDR_HEATING_WORKING_DAYS_MORNING_OFF_H, heatingTimes[idx].hour);
		EEPROM.write(EEPROM_ADDR_HEATING_WORKING_DAYS_MORNING_OFF_M, heatingTimes[idx].minute);
		break;
	case HEATING_WORKING_DAYS_AFTERNOON_ON:
		EEPROM.write(EEPROM_ADDR_HEATING_WORKING_DAYS_AFTERNOON_ON_H, heatingTimes[idx].hour);
		EEPROM.write(EEPROM_ADDR_HEATING_WORKING_DAYS_AFTERNOON_ON_M, heatingTimes[idx].minute);
		break;
	case HEATING_WORKING_DAYS_AFTERNOON_OFF:
		EEPROM.write(EEPROM_ADDR_HEATING_WORKING_DAYS_AFTERNOON_OFF_H, heatingTimes[idx].hour);
		EEPROM.write(EEPROM_ADDR_HEATING_WORKING_DAYS_AFTERNOON_OFF_M, heatingTimes[idx].minute);
		break;
	case HEATING_WEEKEND_MORNING_ON:
		EEPROM.write(EEPROM_ADDR_HEATING_WEEKEND_MORNING_ON_H, heatingTimes[idx].hour);
		EEPROM.write(EEPROM_ADDR_HEATING_WEEKEND_MORNING_ON_M, heatingTimes[idx].minute);
		break;
	case HEATING_WEEKEND_MORNING_OFF:
		EEPROM.write(EEPROM_ADDR_HEATING_WEEKEND_MORNING_OFF_H, heatingTimes[idx].hour);
		EEPROM.write(EEPROM_ADDR_HEATING_WEEKEND_MORNING_OFF_M, heatingTimes[idx].minute);
		break;
	case HEATING_WEEKEND_AFTERNOON_ON:
		EEPROM.write(EEPROM_ADDR_HEATING_WEEKEND_AFTERNOON_ON_H, heatingTimes[idx].hour);
		EEPROM.write(EEPROM_ADDR_HEATING_WEEKEND_AFTERNOON_ON_M, heatingTimes[idx].minute);
		break;
	case HEATING_WEEKEND_AFTERNOON_OFF:
		EEPROM.write(EEPROM_ADDR_HEATING_WEEKEND_AFTERNOON_OFF_H, heatingTimes[idx].hour);
		EEPROM.write(EEPROM_ADDR_HEATING_WEEKEND_AFTERNOON_OFF_M, heatingTimes[idx].minute);
		break;
	}
	EEPROM.commit();
}
//-----------------------------------------------------------------------------------------------------
void Configuration::setParam(uint8_t paramId, uint8_t value) {
	Serial.printf("Configuration::setParam() paramId: %d value: %d\r\n", paramId, value);
	EEPROM.write(EEPROM_ADDR_PARAMS_OFFSET + paramId, value);
	EEPROM.commit();
}
//-----------------------------------------------------------------------------------------------------
uint8_t Configuration::getParam(uint8_t paramId) {
	uint8_t value = EEPROM.read(EEPROM_ADDR_PARAMS_OFFSET + paramId);
	Serial.printf("Configuration::getParam() paramId: %d value: %d\r\n", paramId, value);
	return value;
}
//-----------------------------------------------------------------------------------------------------
void Configuration::print() {
	Serial.printf("wifiSSID: %s\r\n", wifiSSID.c_str());
	Serial.printf("wifiPassword: %s\r\n", wifiPassword.c_str());
	Serial.printf("mqttServer: %s\r\n", mqttServer.c_str());
	Serial.printf("dayTemp: %.1f\r\n", getDayTemperature());
	Serial.printf("nightTemp: %.1f\r\n", getNightTemperature());
	Serial.printf("hysteresisTemp: %.1f\r\n", getHisteresisTemp());
	Serial.printf("HEATING_WORKING_DAYS_MORNING_ON: %02d:%02d\r\n", heatingTimes[HEATING_WORKING_DAYS_MORNING_ON].hour, heatingTimes[HEATING_WORKING_DAYS_MORNING_ON].minute);
	Serial.printf("HEATING_WORKING_DAYS_MORNING_OFF: %02d:%02d\r\n", heatingTimes[HEATING_WORKING_DAYS_MORNING_OFF].hour, heatingTimes[HEATING_WORKING_DAYS_MORNING_OFF].minute);
	Serial.printf("HEATING_WORKING_DAYS_AFTERNOON_ON: %02d:%02d\r\n", heatingTimes[HEATING_WORKING_DAYS_AFTERNOON_ON].hour, heatingTimes[HEATING_WORKING_DAYS_AFTERNOON_ON].minute);
	Serial.printf("HEATING_WORKING_DAYS_AFTERNOON_OFF: %02d:%02d\r\n", heatingTimes[HEATING_WORKING_DAYS_AFTERNOON_OFF].hour, heatingTimes[HEATING_WORKING_DAYS_AFTERNOON_OFF].minute);
	Serial.printf("HEATING_WEEKEND_MORNING_ON: %02d:%02d\r\n", heatingTimes[HEATING_WEEKEND_MORNING_ON].hour, heatingTimes[HEATING_WEEKEND_MORNING_ON].minute);
	Serial.printf("HEATING_WEEKEND_MORNING_OFF: %02d:%02d\r\n", heatingTimes[HEATING_WEEKEND_MORNING_OFF].hour, heatingTimes[HEATING_WEEKEND_MORNING_OFF].minute);
	Serial.printf("HEATING_WEEKEND_AFTERNOON_ON: %02d:%02d\r\n", heatingTimes[HEATING_WEEKEND_AFTERNOON_ON].hour, heatingTimes[HEATING_WEEKEND_AFTERNOON_ON].minute);
	Serial.printf("HEATING_WEEKEND_AFTERNOON_OFF: %02d:%02d\r\n", heatingTimes[HEATING_WEEKEND_AFTERNOON_OFF].hour, heatingTimes[HEATING_WEEKEND_AFTERNOON_OFF].minute);
}

Configuration configuration;
