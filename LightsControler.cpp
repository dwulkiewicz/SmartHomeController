
#include "RGBdriver.h"
#include "Constants.h"
#include "Configuration.h"
#include "LightsControler.h"
#include "EventsHandler.h"

RGBdriver Driver(GPIO_CLK, GPIO_DIO);

LightsControler::LightsControler() {
}
//----------------------------------------------------------------------------------------
void LightsControler::init() {
	currentMainLigt = 0xFF;
	currentAdditionalLight = 0xFF;
	currentTapeWhite = 0xFF;
	currentTapeRed = 0xFF;
	currentTapeGreen = 0xFF;
	currentTapeBlue = 0xFF;
	setPWM(0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
	pinMode(GPIO_SW_1, INPUT);
	pinMode(GPIO_SW_2, INPUT);
	pinMode(GPIO_SW_3, INPUT);
	pinMode(GPIO_SW_4, INPUT);

	lightsBath[LIGHTS_BATH_SCENARIO_1].main.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_MAIN_1_IDX));
	lightsBath[LIGHTS_BATH_SCENARIO_1].holder.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_HOLDER_1_IDX));
	lightsBath[LIGHTS_BATH_SCENARIO_1].tapeWhite.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_TAPE_1_IDX));
	lightsBath[LIGHTS_BATH_SCENARIO_1].tapeRgb.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_RGB_V_1_IDX));
	lightsBath[LIGHTS_BATH_SCENARIO_1].tapeRgb.setHue(configuration.getLightsBathValue(LIGHT_BATH_RGB_H_1_IDX));

	lightsBath[LIGHTS_BATH_SCENARIO_2].main.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_MAIN_2_IDX));
	lightsBath[LIGHTS_BATH_SCENARIO_2].holder.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_HOLDER_2_IDX));
	lightsBath[LIGHTS_BATH_SCENARIO_2].tapeWhite.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_TAPE_2_IDX));
	lightsBath[LIGHTS_BATH_SCENARIO_2].tapeRgb.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_RGB_V_2_IDX));
	lightsBath[LIGHTS_BATH_SCENARIO_2].tapeRgb.setHue(configuration.getLightsBathValue(LIGHT_BATH_RGB_H_2_IDX));

	lightsBath[LIGHTS_BATH_SCENARIO_3].main.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_MAIN_2_IDX));
	lightsBath[LIGHTS_BATH_SCENARIO_3].holder.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_HOLDER_2_IDX));
	lightsBath[LIGHTS_BATH_SCENARIO_3].tapeWhite.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_TAPE_2_IDX));
	lightsBath[LIGHTS_BATH_SCENARIO_3].tapeRgb.setBrightness(configuration.getLightsBathValue(LIGHT_BATH_RGB_V_2_IDX));
	lightsBath[LIGHTS_BATH_SCENARIO_3].tapeRgb.setHue(configuration.getLightsBathValue(LIGHT_BATH_RGB_H_2_IDX));
}
//----------------------------------------------------------------------------------------
void LightsControler::onSwitchChanged(uint8_t switchId, uint8_t switchState) {
	switch (switchId) {
	case SWITCH_BATH_1_ID:
		swBath1.setState(switchState);
		break;
	case SWITCH_BATH_2_ID:
		swBath2.setState(switchState);
		break;
	case SWITCH_BATH_3_ID:
		swBath3.setState(switchState);
		break;
	}
}
//----------------------------------------------------------------------------------------
void LightsControler::onLightValueChange(uint8_t idx, uint8_t value) {
	logger.log(debug,"LightsControler::onLightValueChange() idx: %d val: %d\r\n", idx, value);
	configuration.setLightsBathValue(idx,value);
	switch(idx){
		case LIGHT_BATH_MAIN_1_IDX: lightsBath[LIGHTS_BATH_SCENARIO_1].main.setBrightness(value); break;
		case LIGHT_BATH_HOLDER_1_IDX: lightsBath[LIGHTS_BATH_SCENARIO_1].holder.setBrightness(value); break;
		case LIGHT_BATH_TAPE_1_IDX: lightsBath[LIGHTS_BATH_SCENARIO_1].tapeWhite.setBrightness(value); break;
		case LIGHT_BATH_RGB_V_1_IDX: lightsBath[LIGHTS_BATH_SCENARIO_1].tapeRgb.setBrightness(value); break;
		case LIGHT_BATH_RGB_H_1_IDX: lightsBath[LIGHTS_BATH_SCENARIO_1].tapeRgb.setHue(value); break;
		case LIGHT_BATH_MAIN_2_IDX: lightsBath[LIGHTS_BATH_SCENARIO_2].main.setBrightness(value); break;
		case LIGHT_BATH_HOLDER_2_IDX: lightsBath[LIGHTS_BATH_SCENARIO_2].holder.setBrightness(value); break;
		case LIGHT_BATH_TAPE_2_IDX: lightsBath[LIGHTS_BATH_SCENARIO_2].tapeWhite.setBrightness(value); break;
		case LIGHT_BATH_RGB_V_2_IDX: lightsBath[LIGHTS_BATH_SCENARIO_2].tapeRgb.setBrightness(value); break;
		case LIGHT_BATH_RGB_H_2_IDX: lightsBath[LIGHTS_BATH_SCENARIO_2].tapeRgb.setHue(value); break;
		case LIGHT_BATH_MAIN_3_IDX: lightsBath[LIGHTS_BATH_SCENARIO_3].main.setBrightness(value); break;
		case LIGHT_BATH_HOLDER_3_IDX: lightsBath[LIGHTS_BATH_SCENARIO_3].holder.setBrightness(value); break;
		case LIGHT_BATH_TAPE_3_IDX: lightsBath[LIGHTS_BATH_SCENARIO_3].tapeWhite.setBrightness(value); break;
		case LIGHT_BATH_RGB_V_3_IDX: lightsBath[LIGHTS_BATH_SCENARIO_3].tapeRgb.setBrightness(value); break;
		case LIGHT_BATH_RGB_H_3_IDX: lightsBath[LIGHTS_BATH_SCENARIO_3].tapeRgb.setHue(value); break;
	}
}
//----------------------------------------------------------------------------------------
void LightsControler::setPWM(uint8_t mainLigt, uint8_t additionalLight, uint8_t tapeWhite, uint8_t tapeRed, uint8_t tapeGreen, uint8_t tapeBlue) {
	if (mainLigt != currentMainLigt || additionalLight != currentAdditionalLight || tapeWhite != currentTapeWhite ||
		tapeRed != currentTapeRed || tapeGreen != currentTapeGreen || tapeBlue != currentTapeBlue) {

		Serial.printf("LightsControler::setPWM(): mainLigt %d, additionalLight %d, tapeWhite %d,  tapeRed %d, tapeGreen %d, tapeBlue %d \r\n", mainLigt, additionalLight, tapeWhite, tapeRed, tapeGreen, tapeBlue);

		Driver.begin(); // begin
		Driver.SetColor(tapeRed, tapeGreen, tapeBlue); //Blue. first node data    
		Driver.SetColor(mainLigt, tapeWhite, additionalLight); //Blue. second node data 
		Driver.end();

		currentMainLigt = mainLigt;
		currentAdditionalLight = additionalLight;
		currentTapeWhite = tapeWhite;
		currentTapeRed = tapeRed;
		currentTapeGreen = tapeGreen;
		currentTapeBlue = tapeBlue;
	}
}
//----------------------------------------------------------------------------------------
void LightsControler::loop() {
	//if (digitalRead(GPIO_SW_1) || digitalRead(GPIO_SW_2) || digitalRead(GPIO_SW_3) || digitalRead(GPIO_SW_4)) {

	uint8_t scenario;
	if (swBath1.getState() == SW_ON && swBath2.getState() == SW_OFF)
		scenario = LIGHTS_BATH_SCENARIO_1;
	else if (swBath1.getState() == SW_OFF && swBath2.getState() == SW_ON)
		scenario = LIGHTS_BATH_SCENARIO_2;
	else if (swBath1.getState() == SW_ON && swBath2.getState() == SW_ON)
		scenario = LIGHTS_BATH_SCENARIO_3;
	else {
		//TODO: wymagana optymalizacja
		setPWM(0, 0, 0, 0, 0, 0);
		return;
	}
	RgbColor color = lightsBath[scenario].tapeRgb.getRGBColor();
	setPWM(lightsBath[scenario].main.getBrightness(), lightsBath[scenario].holder.getBrightness(), lightsBath[scenario].tapeWhite.getBrightness(), color.r, color.g, color.b);
}

LightsControler lightsControler;
