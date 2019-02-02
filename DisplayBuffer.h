#pragma once

#include <Arduino.h>
#include <RTClib.h>
#include "Types.h"

class DisplayBuffer
{
public:
	DisplayBuffer();
public:
	void set(const DateTime& dateTime);
public:
	//Time
	TDateTime dateTime;
	//Indoor sensor
	uint16_t indoorTemperature;
	uint8_t indoorHumidity;
	//Outdoor sensor
	int outdoorTemperature;
	uint8_t outdoorHumidity;
	//
	uint16_t pressure;
    //Heating
	uint8_t heatingStatus;
	uint8_t heatingPeriod;
	float heatingRequiredTemp;
	//Network
	uint8_t wifiStatus;
	uint8_t mqttStatus;
	//Switch
	uint8_t switch1State;
	uint8_t switch2State;
	uint8_t switch3State;
};
