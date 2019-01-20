#pragma once
#include "LightDevice.h"
#include "ColorUtils.h"

class RGBLightDevice :
	public LightDevice
{
public:
	RGBLightDevice();
	~RGBLightDevice();

	void setHuePercent(uint8_t hue);
	uint8_t getHuePercent();
	void setSaturationPercent(uint8_t saturation);
	uint8_t getSaturationPercent();

	void setHue(uint8_t hue);
	uint8_t getHue();
	void setSaturation(uint8_t saturation);
	uint8_t getSaturation();

	RgbColor getRGBColor();
private:
	uint8_t hue;    
	uint8_t saturation;
};
