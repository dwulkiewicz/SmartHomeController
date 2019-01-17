#pragma once
#include "OnOffDevice.h"
class LightDevice :
	public OnOffDevice
{
public:
	LightDevice();
	~LightDevice();
public:
	void setBrightnessPercent(uint8_t brightness); //0-100
	uint8_t getBrightnessPercent(); //0-100
	void setBrightness(uint8_t brightness); //0-255
	uint8_t getBrightness(); //0-255
protected:
	uint8_t brightness; //0-255
};
