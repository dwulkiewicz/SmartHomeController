#include "RGBLightDevice.h"



RGBLightDevice::RGBLightDevice()
	: LightDevice()
{
	hue = 255;
	saturation = 255;
}

RGBLightDevice::~RGBLightDevice()
{
}

void RGBLightDevice::setHuePercent(uint8_t hue) {
	if (hue >= 100)
		this->hue = 255;
	else
		this->hue = round((float)hue * 2.55);
}

uint8_t RGBLightDevice::getHuePercent(){
	return round((float)hue / 2.55);
}

void RGBLightDevice::setSaturationPercent(uint8_t saturation) {
	if (saturation >= 100)
		this->saturation = 255;
	else
		this->saturation = round((float)saturation * 2.55);
}

uint8_t RGBLightDevice::getSaturationPercent() {
	return round((float)saturation / 2.55);
}

void RGBLightDevice::setHue(uint8_t hue) {
	this->hue = hue;
}

uint8_t RGBLightDevice::getHue() {
	return hue;
}

void RGBLightDevice::setSaturation(uint8_t saturation) {
	this->saturation = saturation;
}

uint8_t RGBLightDevice::getSaturation() {
	return saturation;
}

RgbColor RGBLightDevice::getRGBColor() {
	HsvColor hsv;
	hsv.h = hue;
	hsv.s = saturation;
	hsv.v = brightness;

	return ColorUtils::HsvToRgb(hsv);
}
