#pragma once

#include "RGBLightDevice.h"

class LightsControler
{
public:
	LightsControler();
	~LightsControler();
public:
	void init();
	void loop();
//private:	 
	LightDevice bathroomMainLight;
	LightDevice bathroomAdditionalLight;
	LightDevice bathroomTapeLight;
	RGBLightDevice bathroomRGBTapeLight;
};


extern LightsControler lightsControler;
