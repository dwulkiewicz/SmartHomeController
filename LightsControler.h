#ifndef __LightsControler_H__
#define __LightsControler_H__

#include "RGBLightDevice.h"

class LightsControler
{
public:
	LightsControler();
public:
	void init();
	void loop();
	void onSwitchChanged(uint8_t switchId, uint8_t switchState);
private:
	SwitchDevice swBath1;
	SwitchDevice swBath2;
	SwitchDevice swBath3;
public:	 
	LightDevice bathroomMainLight; //TODO: do przerobienia
	LightDevice bathroomAdditionalLight; //TODO: do przerobienia
	LightDevice bathroomTapeLight; //TODO: do przerobienia
	RGBLightDevice bathroomRGBTapeLight; //TODO: do przerobienia
private:
	void setPWM(uint8_t mainLigt, uint8_t additionalLight, uint8_t tapeWhite, uint8_t tapeRed, uint8_t tapeGreen, uint8_t tapeBlue);
private:
	uint8_t currentMainLigt;
	uint8_t currentAdditionalLight;
	uint8_t currentTapeWhite;
	uint8_t currentTapeRed;
	uint8_t currentTapeGreen;
	uint8_t currentTapeBlue;
};
extern LightsControler lightsControler;
#endif /* #ifndef __LightsControler_H__ */