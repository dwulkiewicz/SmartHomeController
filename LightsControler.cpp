
#include "RGBdriver.h"
#include "Constants.h"
#include "Configuration.h"
#include "LightsControler.h"
#include "EventDispatcher.h"

RGBdriver Driver(GPIO_CLK, GPIO_DIO);

LightsControler::LightsControler() {
	pinMode(GPIO_SW_1, INPUT);
	pinMode(GPIO_SW_2, INPUT);
	pinMode(GPIO_SW_3, INPUT);
	pinMode(GPIO_SW_4, INPUT);
}
//----------------------------------------------------------------------------------------
void LightsControler::init() {
	//TODO: w zadzie to jest sta³e wiêc trzeba przenieœæ do konfiguracji
	stepCount = ceil(LIGHTS_DIMING_TIME / TASK_LIGHTS_LOOP);

	//wy³¹czam wszystko, bo po za³¹czeniu zawsze tam s¹ jakieœ œmieci
	setLights(0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

	variants[LIGHTS_BATH_OFF].main.setBrightness(0);
	variants[LIGHTS_BATH_OFF].holder.setBrightness(0);
	variants[LIGHTS_BATH_OFF].tapeWhite.setBrightness(0);
	variants[LIGHTS_BATH_OFF].tapeRgb.setBrightness(0);
	variants[LIGHTS_BATH_OFF].tapeRgb.setHue(0);

	variants[LIGHTS_BATH_VARIANT_1].main.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_MAIN_1_IDX));
	variants[LIGHTS_BATH_VARIANT_1].holder.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_HOLDER_1_IDX));
	variants[LIGHTS_BATH_VARIANT_1].tapeWhite.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_TAPE_1_IDX));
	variants[LIGHTS_BATH_VARIANT_1].tapeRgb.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_RGB_V_1_IDX));
	variants[LIGHTS_BATH_VARIANT_1].tapeRgb.setHue(configuration.getLightsBathValue(LIGHT_BATH_RGB_H_1_IDX));

	variants[LIGHTS_BATH_VARIANT_2].main.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_MAIN_2_IDX));
	variants[LIGHTS_BATH_VARIANT_2].holder.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_HOLDER_2_IDX));
	variants[LIGHTS_BATH_VARIANT_2].tapeWhite.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_TAPE_2_IDX));
	variants[LIGHTS_BATH_VARIANT_2].tapeRgb.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_RGB_V_2_IDX));
	variants[LIGHTS_BATH_VARIANT_2].tapeRgb.setHue(configuration.getLightsBathValue(LIGHT_BATH_RGB_H_2_IDX));

	variants[LIGHTS_BATH_VARIANT_3].main.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_MAIN_3_IDX));
	variants[LIGHTS_BATH_VARIANT_3].holder.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_HOLDER_3_IDX));
	variants[LIGHTS_BATH_VARIANT_3].tapeWhite.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_TAPE_3_IDX));
	variants[LIGHTS_BATH_VARIANT_3].tapeRgb.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_RGB_V_3_IDX));
	variants[LIGHTS_BATH_VARIANT_3].tapeRgb.setHue(configuration.getLightsBathValue(LIGHT_BATH_RGB_H_3_IDX));

	variants[LIGHTS_BATH_AUTO_NIGHT].main.setBrightness(0);
	variants[LIGHTS_BATH_AUTO_NIGHT].holder.setBrightness(3);
	variants[LIGHTS_BATH_AUTO_NIGHT].tapeWhite.setBrightness(0);
	variants[LIGHTS_BATH_AUTO_NIGHT].tapeRgb.setBrightness(0);
	variants[LIGHTS_BATH_AUTO_NIGHT].tapeRgb.setHue(0);

	variants[LIGHTS_BATH_AUTO_DAY].main.setBrightness(255);
	variants[LIGHTS_BATH_AUTO_DAY].holder.setBrightness(0);
	variants[LIGHTS_BATH_AUTO_DAY].tapeWhite.setBrightness(0);
	variants[LIGHTS_BATH_AUTO_DAY].tapeRgb.setBrightness(0);
	variants[LIGHTS_BATH_AUTO_DAY].tapeRgb.setHue(0);

	curr = variants[LIGHTS_BATH_OFF];
	last = curr;
}
//----------------------------------------------------------------------------------------
void LightsControler::loop() {
	//do przeniesienia w inne miejsce
	swBath1.setState(digitalRead(GPIO_SW_1) == HIGH ? SW_ON : SW_OFF);


	uint8_t av = activeVariant;
	if (swBath1.getState() == SW_ON && swBath2.getState() == SW_OFF)
		av = LIGHTS_BATH_VARIANT_1;
	else if (swBath1.getState() == SW_OFF && swBath2.getState() == SW_ON)
		av = LIGHTS_BATH_VARIANT_2;
	else if (swBath1.getState() == SW_ON && swBath2.getState() == SW_ON)
		av = LIGHTS_BATH_VARIANT_3;
	else
		av = LIGHTS_BATH_OFF;

	if (av != activeVariant) {
		logger.log(debug, "LightsControler::loop() variant: %d -> %d\r\n", activeVariant, av);
		activeVariant = av;
		last = curr;
		lastLoopTime = millis();
	}
	updateLights();
}
//----------------------------------------------------------------------------------------
void LightsControler::onSwitchStateChanged(uint8_t idx, uint8_t state) {
	logger.log(debug, "LightsControler::onSwitchChanged() idx: %d state: %d\r\n", idx, state);
	switch (idx) {
	case SWITCH_BATH_1_IDX:
		swBath1.setState(state);
		break;
	case SWITCH_BATH_2_IDX:
		swBath2.setState(state);
		break;
	case SWITCH_BATH_3_IDX:
		swBath3.setState(state);
		break;
	}
}
//----------------------------------------------------------------------------------------
void LightsControler::onLightValueChanged(uint8_t idx, uint8_t value) {
	logger.log(debug, "LightsControler::onLightValueChange() idx: %d val: %d\r\n", idx, value);
	configuration.setLightsBathValue(idx, value);
	switch (idx) {
	case LIGHT_BATH_MAIN_1_IDX: variants[LIGHTS_BATH_VARIANT_1].main.setBrightness(value); break;
	case LIGHT_BATH_HOLDER_1_IDX: variants[LIGHTS_BATH_VARIANT_1].holder.setBrightness(value); break;
	case LIGHT_BATH_TAPE_1_IDX: variants[LIGHTS_BATH_VARIANT_1].tapeWhite.setBrightness(value); break;
	case LIGHT_BATH_RGB_V_1_IDX: variants[LIGHTS_BATH_VARIANT_1].tapeRgb.setBrightness(value); break;
	case LIGHT_BATH_RGB_H_1_IDX: variants[LIGHTS_BATH_VARIANT_1].tapeRgb.setHue(value); break;

	case LIGHT_BATH_MAIN_2_IDX: variants[LIGHTS_BATH_VARIANT_2].main.setBrightness(value); break;
	case LIGHT_BATH_HOLDER_2_IDX: variants[LIGHTS_BATH_VARIANT_2].holder.setBrightness(value); break;
	case LIGHT_BATH_TAPE_2_IDX: variants[LIGHTS_BATH_VARIANT_2].tapeWhite.setBrightness(value); break;
	case LIGHT_BATH_RGB_V_2_IDX: variants[LIGHTS_BATH_VARIANT_2].tapeRgb.setBrightness(value); break;
	case LIGHT_BATH_RGB_H_2_IDX: variants[LIGHTS_BATH_VARIANT_2].tapeRgb.setHue(value); break;

	case LIGHT_BATH_MAIN_3_IDX: variants[LIGHTS_BATH_VARIANT_3].main.setBrightness(value); break;
	case LIGHT_BATH_HOLDER_3_IDX: variants[LIGHTS_BATH_VARIANT_3].holder.setBrightness(value); break;
	case LIGHT_BATH_TAPE_3_IDX: variants[LIGHTS_BATH_VARIANT_3].tapeWhite.setBrightness(value); break;
	case LIGHT_BATH_RGB_V_3_IDX: variants[LIGHTS_BATH_VARIANT_3].tapeRgb.setBrightness(value); break;
	case LIGHT_BATH_RGB_H_3_IDX: variants[LIGHTS_BATH_VARIANT_3].tapeRgb.setHue(value); break;
	}
}
//----------------------------------------------------------------------------------------
void LightsControler::updateLights() {
	if (!(curr == variants[activeVariant])) {
		curr.main.setBrightness(calculateStep(last.main.getBrightness(), curr.main.getBrightness(), variants[activeVariant].main.getBrightness()));
		curr.holder.setBrightness(calculateStep(last.holder.getBrightness(), curr.holder.getBrightness(), variants[activeVariant].holder.getBrightness()));
		curr.tapeWhite.setBrightness(calculateStep(last.tapeWhite.getBrightness(), curr.tapeWhite.getBrightness(), variants[activeVariant].tapeWhite.getBrightness()));
		curr.tapeRgb.setBrightness(calculateStep(last.tapeRgb.getBrightness(), curr.tapeRgb.getBrightness(), variants[activeVariant].tapeRgb.getBrightness()));
		//je¿eli œciemniamy i wy³¹czamy RGB to na ten czas utrzymujemy stary kolor
		if (variants[activeVariant].tapeRgb.getBrightness() == 0 && curr.tapeRgb.getBrightness() > 0)
			curr.tapeRgb.setHue(last.tapeRgb.getHue());
		else
			curr.tapeRgb.setHue(variants[activeVariant].tapeRgb.getHue());
		/*
		uint32_t t = millis() - lastLoopTime;
		lastLoopTime = millis();

		logger.log(debug, "LightsControler::updateLights(): curr time %d, main %d, holder %d, tapeWhite %d, tapeRgb %d, tapeRgbH %d\r\n",
			t,
			curr.main.getBrightness(),
			curr.holder.getBrightness(),
			curr.tapeWhite.getBrightness(),
			curr.tapeRgb.getBrightness(),
			curr.tapeRgb.getHue()
		);

		logger.log(debug, "LightsControler::updateLights(): active tome %d, main %d, holder %d, tapeWhite %d, tapeRgb, tapeRgbH %d %d\r\n",
			t,
			variants[activeVariant].main.getBrightness(),
			variants[activeVariant].holder.getBrightness(),
			variants[activeVariant].tapeWhite.getBrightness(),
			variants[activeVariant].tapeRgb.getBrightness(),
			variants[activeVariant].tapeRgb.getHue()
		);
		*/
		RgbColor color = curr.tapeRgb.getColorRgb();
		setLights(curr.main.getBrightness(),
			curr.holder.getBrightness(),
			curr.tapeWhite.getBrightness(), color.r, color.g, color.b);
	}
}
//----------------------------------------------------------------------------------------
uint8_t LightsControler::calculateStep(uint8_t from, uint8_t curr, uint8_t to) {
	uint8_t ret = curr;
	int a = abs((int)to - (int)from);
	uint8_t stepSize = ceil((float)a / (float)stepCount);
	if (curr < to)
		ret = MIN(curr + stepSize, to);
	else if (curr > to)
		ret = MAX(curr - stepSize, to);
	else
		ret = to;

	//if (curr != to)
	//	logger.log(debug, "LightsControler::calculateStep(): from %d, curr %d, to %d, count %d, step %d ret %d\r\n", from, curr, to, stepCount, stepSize, ret);
	return ret;
}
//----------------------------------------------------------------------------------------
void LightsControler::setLights(uint8_t mainVal, uint8_t holderVal, uint8_t tapeWhiteVal, uint8_t tapeRed, uint8_t tapeGreen, uint8_t tapeBlue) {
	logger.log(debug, "LightsControler::setLights(): main %d, holder %d, tapeW %d, tapeR %d, tapeG %d, tapeB %d\r\n", mainVal, holderVal, tapeWhiteVal, tapeRed, tapeGreen, tapeBlue);

	Driver.begin();
	Driver.SetColor(tapeRed, tapeGreen, tapeBlue); //first node data    
	Driver.SetColor(mainVal, tapeWhiteVal, holderVal); //second node data 
	Driver.end();
}
LightsControler lightsControler;
