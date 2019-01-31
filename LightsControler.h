#ifndef __LightsControler_H__
#define __LightsControler_H__

#include "RGBLightDevice.h"

#define LIGHTS_BATH_SCENARIO_COUNT 3

#define LIGHTS_BATH_SCENARIO_1 0
#define LIGHTS_BATH_SCENARIO_2 1
#define LIGHTS_BATH_SCENARIO_3 2


typedef struct {
	LightDevice main;
	LightDevice holder;
	LightDevice tapeWhite;
	RGBLightDevice tapeRgb; 
} 	lightBath_t;


class LightsControler
{
public:
	LightsControler();
public:
	void init();
	void loop();
	void onSwitchChanged(uint8_t switchId, uint8_t switchState);
	void onLightValueChange(uint8_t idx, uint8_t value);
private:
	SwitchDevice swBath1;
	SwitchDevice swBath2;
	SwitchDevice swBath3;	
	lightBath_t lightsBath[LIGHTS_BATH_SCENARIO_COUNT];
public:	 

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