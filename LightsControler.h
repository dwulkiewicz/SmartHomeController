#ifndef __LightsControler_H__
#define __LightsControler_H__

#include "LightDeviceRgbEntity.h"

#define LIGHTS_BATH_OFF		   0
#define LIGHTS_BATH_VARIANT_1  1
#define LIGHTS_BATH_VARIANT_2  2
#define LIGHTS_BATH_VARIANT_3  3
#define LIGHTS_BATH_PIR_NIGHT 4
#define LIGHTS_BATH_PIR_DAY   5
#define LIGHTS_BATH_VARIANT_COUNT 6

class LightDeviceSet {
public:
	LightDeviceEntity main;
	LightDeviceEntity holder;
	LightDeviceEntity tapeWhite;
	LightDeviceRgbEntity tapeRgb;
	bool operator==(LightDeviceSet &n) {
		return main == n.main &&
			holder == n.holder &&
			tapeWhite == n.tapeWhite &&
			tapeRgb == n.tapeRgb;
	}
	void max(const LightDeviceSet &src) {
		main.max(src.main);
		holder.max(src.holder);
		tapeWhite.max(src.tapeWhite);
		tapeRgb.max(src.tapeRgb);
	}
};

class LightsControler
{
public:
	LightsControler();
public:
	void init();
	void loop();
	void onSwitchChange(uint8_t src, uint8_t switchId, uint8_t switchState);
	void onLightValueChanged(uint8_t idx, uint8_t value);
private:
	SwitchDevice swBath1;
	SwitchDevice swBath2;
	SwitchDevice swBath3;
	SwitchDevice swBathPIR;
private:
	LightDeviceSet curr;
	LightDeviceSet last;
	LightDeviceSet active;
	LightDeviceSet variants[LIGHTS_BATH_VARIANT_COUNT];
	uint8_t activeVariant;
	uint8_t stepCount;
	uint32_t lastLoopTime;
private:
	uint8_t calculateStep(uint8_t from, uint8_t curr, uint8_t to);
	void updateLights();
	void setLights(uint8_t mainVal, uint8_t holderVal, uint8_t tapeWhite, uint8_t tapeRed, uint8_t tapeGreen, uint8_t tapeBlue);
};
extern LightsControler lightsControler;
#endif /* #ifndef __LightsControler_H__ */