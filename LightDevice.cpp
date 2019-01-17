#include "LightDevice.h"



LightDevice::LightDevice()
	: OnOffDevice()
{
	brightness = 0;
}

LightDevice::~LightDevice()
{
}

void LightDevice::setBrightnessPercent(uint8_t brightness) {
	if (brightness >= 100)
		this->brightness = 255;
	else
		this->brightness = round((float)brightness * 2.55);
}

uint8_t LightDevice::getBrightnessPercent() {
	return round((float)brightness / 2.55);
}

void LightDevice::setBrightness(uint8_t brightness) {
	this->brightness = brightness;
}

uint8_t LightDevice::getBrightness() {
	return brightness;
}
