#include "LightDeviceRgbEntity.h"



LightDeviceRgbEntity::LightDeviceRgbEntity()
	: LightDeviceEntity(){
	hue = 255;
	saturation = 255;
}

LightDeviceRgbEntity::~LightDeviceRgbEntity(){
}

void LightDeviceRgbEntity::setHuePercent(uint8_t hue) {
	if (hue >= 100)
		this->hue = 255;
	else
		this->hue = round((float)hue * 2.55);
}

uint8_t LightDeviceRgbEntity::getHuePercent(){
	return round((float)hue / 2.55);
}

void LightDeviceRgbEntity::setSaturationPercent(uint8_t saturation) {
	if (saturation >= 100)
		this->saturation = 255;
	else
		this->saturation = round((float)saturation * 2.55);
}

uint8_t LightDeviceRgbEntity::getSaturationPercent() {
	return round((float)saturation / 2.55);
}

void LightDeviceRgbEntity::setHue(uint8_t hue) {
	this->hue = hue;
}

uint8_t LightDeviceRgbEntity::getHue() {
	return hue;
}

void LightDeviceRgbEntity::setSaturation(uint8_t saturation) {
	this->saturation = saturation;
}

uint8_t LightDeviceRgbEntity::getSaturation() {
	return saturation;
}

RgbColor LightDeviceRgbEntity::getColorRgb() {
	HsvColor hsv;
	hsv.h = hue;
	hsv.s = saturation;
	hsv.v = brightness;

	return ColorUtils::HsvToRgb(hsv);
}
