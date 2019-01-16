
#include <Arduino.h>
#pragma once


class Configuration
{
public:
	static String temperatureAsString(float temp);
public:
	Configuration();
public:
	bool init();
  void print();
public:
	float getDayTemperature();
	bool incrementDayTemperature();
	bool decrementDayTemperature();

	float getNightTemperature();
	bool incrementNightTemperature();
	bool decrementNightTemperature();

	float getHisteresisTemp();
	bool incrementHisteresisTemperature();
	bool decrementHisteresisTemperature();
private:
	uint8_t offsetTemp;
	uint8_t dayTemp;
	uint8_t nightTemp;
	uint8_t hysteresisTemp;
public: //TODO: do zmiany na get
	String wifiSSID;
	String wifiPassword;
	String mqttServer;
};
