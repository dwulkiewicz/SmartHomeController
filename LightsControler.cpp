
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
	//TODO:
	//wczytać konfigurację
  bathroomMainLight.setBrightness(configuration.getParam(0));
  bathroomAdditionalLight.setBrightness(configuration.getParam(1));
  bathroomTapeLight.setBrightness(configuration.getParam(2));
  bathroomRGBTapeLight.setBrightness(configuration.getParam(3));
  bathroomRGBTapeLight.setHue(configuration.getParam(4)); 
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
	if(swBath1.getState() == SW_ON){
		RgbColor color = bathroomRGBTapeLight.getRGBColor();
		setPWM(bathroomMainLight.getBrightness(), bathroomAdditionalLight.getBrightness(), bathroomTapeLight.getBrightness(), color.r, color.g, color.b);
	}
	else
	{
		setPWM(0, 0, 0, 0, 0, 0);
	}
}

LightsControler lightsControler;
