#include "LightDeviceEntity.h"

LightDeviceEntity::LightDeviceEntity(){
	brightness = 0;
}

LightDeviceEntity::~LightDeviceEntity(){
}

void LightDeviceEntity::setBrightnessPercent(uint8_t brightness) {
	if (brightness >= 100)
		this->brightness = 255;
	else
		this->brightness = round((float)brightness * 2.55);
}

uint8_t LightDeviceEntity::getBrightnessPercent() {
	return round((float)brightness / 2.55);
}

void LightDeviceEntity::setBrightness(uint8_t brightness) {
	this->brightness = brightness;
}

uint8_t LightDeviceEntity::getBrightness() {
	return brightness;
}
