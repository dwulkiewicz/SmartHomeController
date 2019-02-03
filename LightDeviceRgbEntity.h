#pragma once
#include "LightDeviceEntity.h"
#include "ColorUtils.h"

class LightDeviceRgbEntity :
	public LightDeviceEntity
{
public:
	LightDeviceRgbEntity();
	~LightDeviceRgbEntity();
	bool operator==(const LightDeviceRgbEntity &n) {
		return brightness == n.brightness &&
			hue == n.hue &&
			saturation == n.saturation;
	}
	void setHuePercent(uint8_t hue);
	uint8_t getHuePercent();
	void setSaturationPercent(uint8_t saturation);
	uint8_t getSaturationPercent();

	void setHue(uint8_t hue);
	uint8_t getHue();
	void setSaturation(uint8_t saturation);
	uint8_t getSaturation();

	RgbColor getColorRgb();
private:
	uint8_t hue;    
	uint8_t saturation;
};
